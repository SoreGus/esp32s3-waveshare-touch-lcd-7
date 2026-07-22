#include "api/zelda_view_model.hpp"

#include <string>
#include <vector>

#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"

using namespace DeclarativeUI;

namespace App::Zelda {
namespace {

constexpr EventBits_t Connected = BIT0;
EventGroupHandle_t wifiEvents = nullptr;

void wifiEvent(void*, esp_event_base_t base, int32_t id, void*)
{
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) esp_wifi_connect();
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) esp_wifi_connect();
    if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) xEventGroupSetBits(wifiEvents, Connected);
}

bool connectWiFi(const char* ssid, const char* password)
{
    if (wifiEvents == nullptr) {
        wifiEvents = xEventGroupCreate();
        nvs_flash_init();
        esp_netif_init();
        esp_event_loop_create_default();
        esp_netif_create_default_wifi_sta();
        wifi_init_config_t init = WIFI_INIT_CONFIG_DEFAULT();
        esp_wifi_init(&init);
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifiEvent, nullptr, nullptr);
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifiEvent, nullptr, nullptr);
        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_start();
    }
    wifi_config_t config = {};
    snprintf(reinterpret_cast<char*>(config.sta.ssid), sizeof(config.sta.ssid), "%s", ssid != nullptr ? ssid : "");
    snprintf(reinterpret_cast<char*>(config.sta.password), sizeof(config.sta.password), "%s", password != nullptr ? password : "");
    esp_wifi_set_config(WIFI_IF_STA, &config);
    esp_wifi_connect();
    return (xEventGroupWaitBits(wifiEvents, Connected, pdFALSE, pdTRUE, pdMS_TO_TICKS(20000)) & Connected) != 0;
}

struct HttpBuffer { std::vector<char> bytes; };

esp_err_t httpEvent(esp_http_client_event_t* event)
{
    if (event->event_id == HTTP_EVENT_ON_DATA && event->user_data != nullptr) {
        auto* response = static_cast<HttpBuffer*>(event->user_data);
        const char* first = static_cast<const char*>(event->data);
        response->bytes.insert(response->bytes.end(), first, first + event->data_len);
    }
    return ESP_OK;
}

String join(cJSON* values)
{
    if (!cJSON_IsArray(values)) return String("Unknown");
    std::string result;
    cJSON* value = nullptr;
    cJSON_ArrayForEach(value, values) {
        if (!cJSON_IsString(value)) continue;
        if (!result.empty()) result += " • ";
        result += value->valuestring;
    }
    return String(result.empty() ? "Unknown" : result);
}

struct LoadRequest { CompendiumViewModel* model; String ssid; String password; };

void requestTask(void* parameter)
{
    auto* request = static_cast<LoadRequest*>(parameter);
    auto* model = request->model;
    if (!connectWiFi(request->ssid.c_str(), request->password.c_str())) {
        model->status.set("Could not connect to Wi-Fi");
        model->loading.set(false);
        delete request;
        vTaskDelete(nullptr);
        return;
    }
    model->status.set("Opening the Hyrule Compendium...");
    HttpBuffer response;
    esp_http_client_config_t config = {};
    config.url = "https://api.hyrule-compendium.com/v3/compendium/category/monsters";
    config.event_handler = httpEvent;
    config.user_data = &response;
    config.timeout_ms = 20000;
    config.crt_bundle_attach = esp_crt_bundle_attach;
    esp_http_client_handle_t client = esp_http_client_init(&config);
    const bool success = client != nullptr && esp_http_client_perform(client) == ESP_OK
        && esp_http_client_get_status_code(client) == 200;
    if (client != nullptr) esp_http_client_cleanup(client);
    if (!success) {
        model->status.set("The Compendium is unavailable");
        model->loading.set(false);
        delete request;
        vTaskDelete(nullptr);
        return;
    }
    response.bytes.push_back('\0');
    cJSON* root = cJSON_Parse(response.bytes.data());
    cJSON* data = root != nullptr ? cJSON_GetObjectItem(root, "data") : nullptr;
    Array<CompendiumEntry> entries;
    cJSON* item = nullptr;
    int count = 0;
    cJSON_ArrayForEach(item, data) {
        if (count++ == 12) break;
        cJSON* name = cJSON_GetObjectItem(item, "name");
        cJSON* image = cJSON_GetObjectItem(item, "image");
        if (!cJSON_IsString(name) || !cJSON_IsString(image)) continue;
        entries.values().push_back({
            cJSON_GetObjectItem(item, "id")->valueint,
            String(name->valuestring), String("Monsters"),
            String(cJSON_GetObjectItem(item, "description")->valuestring), String(image->valuestring),
            join(cJSON_GetObjectItem(item, "common_locations")), join(cJSON_GetObjectItem(item, "drops"))
        });
    }
    if (root != nullptr) cJSON_Delete(root);
    model->entries.replaceAll(std::move(entries));
    model->status.set("12 entries recovered from Hyrule");
    model->loading.set(false);
    delete request;
    vTaskDelete(nullptr);
}

} // namespace

void CompendiumViewModel::load(const char* ssid, const char* password) const
{
    auto* request = new LoadRequest { const_cast<CompendiumViewModel*>(this), String(ssid), String(password) };
    xTaskCreate(requestTask, "zelda_api", 12288, request, 4, nullptr);
}

} // namespace App::Zelda
