#include "image.hpp"

#include <memory>
#include <atomic>
#include <vector>

#include "core/view_internal.hpp"
#include "platform/platform.hpp"

#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#define LODEPNG_NO_COMPILE_CPP
extern "C" {
#include "extra/libs/png/lodepng.h"
}
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

namespace DeclarativeUI {

View Image(const char* source, ImageSource type)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Image;
    node->image.source = source != nullptr ? source : "";
    node->image.remote = type == ImageSource::URL;
    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    if (type == ImageSource::Local) {
        return View(std::move(node));
    }

    return View(std::move(node));
}

namespace {

struct RemoteImageContext {
    lv_obj_t* root = nullptr;
    std::string url;
    lv_img_dsc_t image{};
    std::vector<uint8_t> encoded;
    std::vector<lv_color_t> pixels;
    int storageWidth = 0;
    int storageHeight = 0;
    std::atomic<bool> alive {true};
    std::atomic<bool> finished {false};
};

struct DownloadBuffer { std::vector<uint8_t> bytes; };
QueueHandle_t imageQueue = nullptr;

esp_err_t onHttpEvent(esp_http_client_event_t* event)
{
    if (event->event_id == HTTP_EVENT_ON_DATA && event->user_data != nullptr) {
        auto* buffer = static_cast<DownloadBuffer*>(event->user_data);
        const auto* data = static_cast<const uint8_t*>(event->data);
        buffer->bytes.insert(buffer->bytes.end(), data, data + event->data_len);
    }
    return ESP_OK;
}

void loadRemoteImage(RemoteImageContext* context)
{
    DownloadBuffer download;
    esp_http_client_config_t config = {};
    config.url = context->url.c_str();
    config.event_handler = onHttpEvent;
    config.user_data = &download;
    config.timeout_ms = 15000;
    config.crt_bundle_attach = esp_crt_bundle_attach;
    esp_http_client_handle_t client = esp_http_client_init(&config);
    const bool downloaded = client != nullptr && esp_http_client_perform(client) == ESP_OK
        && esp_http_client_get_status_code(client) == 200;
    if (client != nullptr) esp_http_client_cleanup(client);

    const bool isPng = downloaded && download.bytes.size() >= 8
        && download.bytes[0] == 0x89 && download.bytes[1] == 0x50
        && download.bytes[2] == 0x4E && download.bytes[3] == 0x47;
    if (isPng) {
        if (context->storageWidth > 0 && context->storageHeight > 0) {
            unsigned char* decoded = nullptr;
            unsigned sourceWidth = 0;
            unsigned sourceHeight = 0;
            const unsigned error = lodepng_decode32(
                &decoded, &sourceWidth, &sourceHeight,
                download.bytes.data(), download.bytes.size()
            );
            if (error == 0 && decoded != nullptr && sourceWidth > 0 && sourceHeight > 0) {
                const float scaleX = static_cast<float>(context->storageWidth) / sourceWidth;
                const float scaleY = static_cast<float>(context->storageHeight) / sourceHeight;
                const float scale = scaleX < scaleY ? scaleX : scaleY;
                const unsigned width = static_cast<unsigned>(sourceWidth * (scale < 1.0f ? scale : 1.0f));
                const unsigned height = static_cast<unsigned>(sourceHeight * (scale < 1.0f ? scale : 1.0f));
                context->pixels.resize(static_cast<size_t>(width) * height);
                for (unsigned y = 0; y < height; ++y) {
                    const unsigned sourceY = y * sourceHeight / height;
                    for (unsigned x = 0; x < width; ++x) {
                        const unsigned sourceX = x * sourceWidth / width;
                        const uint8_t* pixel = decoded + (sourceY * sourceWidth + sourceX) * 4;
                        context->pixels[static_cast<size_t>(y) * width + x] = lv_color_make(pixel[0], pixel[1], pixel[2]);
                    }
                }
                context->image.header.cf = LV_IMG_CF_TRUE_COLOR;
                context->image.header.w = width;
                context->image.header.h = height;
                context->image.data_size = context->pixels.size() * sizeof(lv_color_t);
                context->image.data = reinterpret_cast<const uint8_t*>(context->pixels.data());
            }
            free(decoded);
        } else {
            context->encoded = std::move(download.bytes);
            context->image.data_size = context->encoded.size();
            context->image.data = context->encoded.data();
        }

        if (context->image.data != nullptr && context->alive.load() && Platform::lock(-1)) {
            lv_obj_clean(context->root);
            lv_obj_t* image = lv_img_create(context->root);
            lv_img_set_src(image, &context->image);
            lv_obj_center(image);
            Platform::unlock();
        }
    }

    context->finished.store(true);
    if (!context->alive.load()) delete context;
}

void remoteImageWorker(void*)
{
    while (true) {
        RemoteImageContext* context = nullptr;
        if (xQueueReceive(imageQueue, &context, portMAX_DELAY) == pdTRUE && context != nullptr) {
            loadRemoteImage(context);
        }
    }
}

} // namespace

lv_obj_t* mountImage(lv_obj_t* parent, const ViewNode& node)
{
    if (!node.image.remote) {
        lv_obj_t* image = lv_img_create(parent);
        lv_img_set_src(image, node.image.localSource != nullptr ? node.image.localSource : node.image.source.c_str());
        return image;
    }

    lv_obj_t* root = lv_obj_create(parent);
    removeDefaultStyle(root);
    lv_obj_t* loading = lv_spinner_create(root, 900, 60);
    lv_obj_set_size(loading, 36, 36);
    lv_obj_set_style_arc_color(loading, toLVColor(Color::Cyan()), LV_PART_INDICATOR);
    lv_obj_center(loading);

    auto* context = new RemoteImageContext();
    context->root = root;
    context->url = node.image.source;
    context->storageWidth = node.image.storageWidth;
    context->storageHeight = node.image.storageHeight;
    lv_obj_add_event_cb(root, [](lv_event_t* event) {
        auto* image = static_cast<RemoteImageContext*>(lv_event_get_user_data(event));
        image->alive.store(false);
        if (image->finished.load()) {
            delete image;
        }
    }, LV_EVENT_DELETE, context);
    if (imageQueue == nullptr) {
        imageQueue = xQueueCreate(16, sizeof(RemoteImageContext*));
        if (imageQueue != nullptr) {
            if (xTaskCreate(remoteImageWorker, "remote_image_worker", 12288, nullptr, 4, nullptr) != pdPASS) {
                vQueueDelete(imageQueue);
                imageQueue = nullptr;
            }
        }
    }
    if (imageQueue != nullptr && xQueueSend(imageQueue, &context, 0) == pdTRUE) {
        return root;
    }
    context->finished.store(true);
    return root;
}

View Image(const lv_img_dsc_t* source)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Image;
    node->image.localSource = source;
    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;
    return View(std::move(node));
}

} // namespace DeclarativeUI
