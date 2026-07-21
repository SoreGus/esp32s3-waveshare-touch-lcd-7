#include "bluetooth_keyboard_service.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>

extern "C" {
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_hidh.h"
#include "esp_hidh_bluedroid.h"
#include "nvs_flash.h"
}

namespace DeclarativeUI::Examples::BluetoothKeyboard {

namespace {

constexpr uint16_t hidServiceUuid = 0x1812;

bool advertisesHidService(const uint8_t* data, size_t length)
{
    for (size_t offset = 0; offset < length;) {
        const uint8_t fieldLength = data[offset];
        if (fieldLength == 0 || offset + fieldLength >= length + 1) break;

        const uint8_t type = data[offset + 1];
        const size_t end = offset + fieldLength + 1;
        if (type == ESP_BLE_AD_TYPE_16SRV_PART || type == ESP_BLE_AD_TYPE_16SRV_CMPL) {
            for (size_t uuidOffset = offset + 2; uuidOffset + 1 < end; uuidOffset += 2) {
                const uint16_t uuid = static_cast<uint16_t>(data[uuidOffset]) |
                                      (static_cast<uint16_t>(data[uuidOffset + 1]) << 8);
                if (uuid == hidServiceUuid) return true;
            }
        }
        offset = end;
    }
    return false;
}

uint16_t advertisedAppearance(const uint8_t* data, size_t length)
{
    for (size_t offset = 0; offset < length;) {
        const uint8_t fieldLength = data[offset];
        if (fieldLength == 0 || offset + fieldLength >= length + 1) break;

        if (data[offset + 1] == ESP_BLE_AD_TYPE_APPEARANCE && fieldLength >= 3) {
            return static_cast<uint16_t>(data[offset + 2]) |
                   (static_cast<uint16_t>(data[offset + 3]) << 8);
        }
        offset += fieldLength + 1;
    }
    return 0;
}

} // namespace

BluetoothKeyboardService* BluetoothKeyboardService::instance_ = nullptr;

void BluetoothKeyboardService::setCallbacks(DevicesCallback devices, ConnectionCallback connection, TextCallback text, StatusCallback status)
{
    devicesCallback_ = std::move(devices);
    connectionCallback_ = std::move(connection);
    textCallback_ = std::move(text);
    statusCallback_ = std::move(status);
}

bool BluetoothKeyboardService::initialize()
{
    if (initialized_) return true;
    instance_ = this;

    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        result = nvs_flash_init();
    }
    if (result != ESP_OK) {
        publishStatus("Nao foi possivel inicializar a memoria Bluetooth.");
        return false;
    }

    esp_bt_controller_config_t controllerConfig = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if (esp_bt_controller_init(&controllerConfig) != ESP_OK ||
        esp_bt_controller_enable(ESP_BT_MODE_BLE) != ESP_OK ||
        esp_bluedroid_init() != ESP_OK || esp_bluedroid_enable() != ESP_OK ||
        esp_ble_gap_register_callback(reinterpret_cast<esp_gap_ble_cb_t>(gapCallback)) != ESP_OK ||
        esp_ble_gattc_register_callback(esp_hidh_gattc_event_handler) != ESP_OK) {
        publishStatus("Nao foi possivel iniciar o Bluetooth BLE.");
        return false;
    }

    esp_hidh_config_t hidConfig {};
    hidConfig.callback = reinterpret_cast<esp_event_handler_t>(hidCallback);
    hidConfig.event_stack_size = 4096;
    if (esp_hidh_init(&hidConfig) != ESP_OK) {
        publishStatus("Nao foi possivel iniciar o host HID.");
        return false;
    }

    initialized_ = true;
    return true;
}

void BluetoothKeyboardService::startScan()
{
    if (!initialize()) return;
    devices_.clear();
    publishDevices();

    if (configuringScan_) {
        publishStatus("A busca ja esta sendo preparada.");
        return;
    }

    if (scanning_) {
        restartScanAfterStop_ = true;
        publishStatus("Reiniciando a busca de dispositivos...");
        if (esp_ble_gap_stop_scanning() != ESP_OK) {
            scanning_ = false;
            restartScanAfterStop_ = false;
            configureAndStartScan();
        }
        return;
    }

    configureAndStartScan();
}

void BluetoothKeyboardService::configureAndStartScan()
{
    configuringScan_ = true;
    publishStatus("Preparando busca de dispositivos BLE...");

    esp_ble_scan_params_t params {};
    params.scan_type = BLE_SCAN_TYPE_ACTIVE;
    params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
    params.scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL;
    params.scan_interval = 0x50;
    params.scan_window = 0x30;
    params.scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE;
    if (esp_ble_gap_set_scan_params(&params) != ESP_OK) {
        configuringScan_ = false;
        publishStatus("Nao foi possivel iniciar a busca.");
    }
}

void BluetoothKeyboardService::connect(const BluetoothDevice& device)
{
    if (!initialized_) return;
    connectingName_ = device.name;
    publishStatus("Conectando a " + device.name.std() + "...");
    if (esp_hidh_dev_open(const_cast<uint8_t*>(device.bda.data()), ESP_HID_TRANSPORT_BLE, device.addressType) == nullptr) {
        publishStatus("Nao foi possivel abrir este dispositivo HID.");
    }
}

void BluetoothKeyboardService::gapCallback(int event, void* parameter)
{
    if (instance_ != nullptr) instance_->onGapEvent(event, parameter);
}

void BluetoothKeyboardService::hidCallback(void*, void*, int32_t id, void* eventData)
{
    if (instance_ != nullptr) instance_->onHidEvent(id, eventData);
}

void BluetoothKeyboardService::onGapEvent(int event, void* parameter)
{
    const auto callbackEvent = static_cast<esp_gap_ble_cb_event_t>(event);
    auto* param = static_cast<esp_ble_gap_cb_param_t*>(parameter);
    if (callbackEvent == ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT) {
        configuringScan_ = false;
        if (param->scan_param_cmpl.status == ESP_BT_STATUS_SUCCESS) {
            publishStatus("Procurando dispositivos por 8 segundos...");
            if (esp_ble_gap_start_scanning(8) == ESP_OK) {
                scanning_ = true;
            } else {
                publishStatus("Nao foi possivel iniciar a varredura BLE.");
            }
        } else {
            publishStatus("Nao foi possivel configurar a busca.");
        }
        return;
    }
    if (callbackEvent == ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT) {
        scanning_ = false;
        if (restartScanAfterStop_) {
            restartScanAfterStop_ = false;
            configureAndStartScan();
        }
        return;
    }
    if (callbackEvent == ESP_GAP_BLE_SEC_REQ_EVT) {
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        return;
    }
    if (callbackEvent == ESP_GAP_BLE_AUTH_CMPL_EVT && !param->ble_security.auth_cmpl.success) {
        publishStatus("Nao foi possivel parear com o teclado BLE.");
        return;
    }
    if (callbackEvent != ESP_GAP_BLE_SCAN_RESULT_EVT) return;

    if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
        const auto& result = param->scan_rst;
        const size_t advertisementLength = result.adv_data_len + result.scan_rsp_len;
        const bool hasHidService = advertisesHidService(result.ble_adv, advertisementLength);
        const uint16_t appearance = advertisedAppearance(result.ble_adv, advertisementLength);
        const bool isKeyboard = appearance == ESP_BLE_APPEARANCE_HID_KEYBOARD;

        // BLE scanning sees every nearby advertiser. Keep only standard HID devices,
        // with a precise keyboard appearance whenever the device provides one.
        if (!hasHidService && !isKeyboard) return;

        uint8_t nameLength = 0;
        uint8_t* name = esp_ble_resolve_adv_data_by_type(
            const_cast<uint8_t*>(result.ble_adv), advertisementLength, ESP_BLE_AD_TYPE_NAME_CMPL, &nameLength
        );
        if (name == nullptr || nameLength == 0) {
            name = esp_ble_resolve_adv_data_by_type(
                const_cast<uint8_t*>(result.ble_adv), advertisementLength, ESP_BLE_AD_TYPE_NAME_SHORT, &nameLength
            );
        }
        BluetoothDevice device;
        device.bda = {result.bda[0], result.bda[1], result.bda[2], result.bda[3], result.bda[4], result.bda[5]};
        device.addressType = result.ble_addr_type;
        device.rssi = result.rssi;
        device.address = String::format("%02X:%02X:%02X:%02X:%02X:%02X", result.bda[0], result.bda[1], result.bda[2], result.bda[3], result.bda[4], result.bda[5]);
        device.name = name != nullptr && nameLength > 0
            ? String(std::string(reinterpret_cast<char*>(name), nameLength))
            : String("Dispositivo BLE sem nome");
        device.profile = isKeyboard ? String("Teclado HID") : String("Dispositivo HID");
        const auto duplicate = std::find_if(devices_.begin(), devices_.end(), [&device](const BluetoothDevice& current) {
            return current.address == device.address;
        });
        if (duplicate == devices_.end()) {
            devices_.push_back(device);
            publishDevices();
        }
    } else if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT) {
        scanning_ = false;
        publishStatus(devices_.empty() ? "Nenhum teclado HID BLE foi encontrado." : "Selecione um teclado para conectar.");
    }
}

void BluetoothKeyboardService::onHidEvent(int32_t id, void* eventData)
{
    const auto event = static_cast<esp_hidh_event_t>(id);
    auto* data = static_cast<esp_hidh_event_data_t*>(eventData);
    if (event == ESP_HIDH_OPEN_EVENT) {
        if (data->open.status == ESP_OK) {
            const char* name = esp_hidh_dev_name_get(data->open.dev);
            const String connectedName(name != nullptr ? name : connectingName_.c_str());
            if (connectionCallback_) connectionCallback_(true, connectedName);
            publishStatus("Teclado conectado. Voce ja pode digitar.");
        } else {
            publishStatus("A conexao HID falhou. Verifique se o dispositivo e um teclado BLE.");
        }
        return;
    }
    if (event == ESP_HIDH_CLOSE_EVENT) {
        if (connectionCallback_) connectionCallback_(false, String());
        publishStatus("Teclado desconectado.");
        return;
    }
    if (event != ESP_HIDH_INPUT_EVENT || data->input.usage != ESP_HID_USAGE_KEYBOARD || data->input.length < 3) return;

    const uint8_t modifier = data->input.data[0];
    for (uint16_t index = 2; index < data->input.length; ++index) {
        const String text = keyToText(modifier, data->input.data[index]);
        if (!text.empty() && textCallback_) textCallback_(text);
    }
}

void BluetoothKeyboardService::publishDevices()
{
    if (devicesCallback_) devicesCallback_(devices_);
}

void BluetoothKeyboardService::publishStatus(const char* status)
{
    if (statusCallback_) statusCallback_(String(status));
}

void BluetoothKeyboardService::publishStatus(const String& status)
{
    if (statusCallback_) statusCallback_(status);
}

String BluetoothKeyboardService::keyToText(uint8_t modifier, uint8_t key)
{
    const bool shift = (modifier & 0x22) != 0;
    if (key >= 0x04 && key <= 0x1d) {
        const char value = static_cast<char>((shift ? 'A' : 'a') + key - 0x04);
        return String(std::string(1, value));
    }
    if (key >= 0x1e && key <= 0x27) {
        static constexpr char normal[] = "1234567890";
        static constexpr char shifted[] = "!@#$%^&*()";
        return String(std::string(1, (shift ? shifted : normal)[key - 0x1e]));
    }
    switch (key) {
        case 0x28: return String("\n");
        case 0x2a: return String("\b");
        case 0x2c: return String(" ");
        case 0x2d: return String(shift ? "_" : "-");
        case 0x2e: return String(shift ? "+" : "=");
        case 0x2f: return String(shift ? "{" : "[");
        case 0x30: return String(shift ? "}" : "]");
        case 0x31: return String(shift ? "|" : "\\");
        case 0x33: return String(shift ? ":" : ";");
        case 0x34: return String(shift ? "\"" : "'");
        case 0x35: return String(shift ? "~" : "`");
        case 0x36: return String(shift ? "<" : ",");
        case 0x37: return String(shift ? ">" : ".");
        case 0x38: return String(shift ? "?" : "/");
        default: return String();
    }
}

} // namespace DeclarativeUI::Examples::BluetoothKeyboard
