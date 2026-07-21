#pragma once

#include <array>
#include <functional>
#include <vector>

#include "core/string.hpp"

namespace DeclarativeUI::Examples::BluetoothKeyboard {

struct BluetoothDevice {
    String name;
    String address;
    String profile;
    std::array<uint8_t, 6> bda {};
    uint8_t addressType = 0;
    int rssi = 0;
};

/** BLE HID host. ESP32-S3 supports BLE keyboards, not Bluetooth Classic-only ones. */
class BluetoothKeyboardService {
public:
    using DevicesCallback = std::function<void(const std::vector<BluetoothDevice>&)>;
    using ConnectionCallback = std::function<void(bool connected, const String& name)>;
    using TextCallback = std::function<void(const String& text)>;
    using StatusCallback = std::function<void(const String& status)>;

    BluetoothKeyboardService() = default;
    ~BluetoothKeyboardService() = default;

    void setCallbacks(DevicesCallback devices, ConnectionCallback connection, TextCallback text, StatusCallback status);
    void startScan();
    void connect(const BluetoothDevice& device);

private:
    static void gapCallback(int event, void* parameter);
    static void hidCallback(void* handlerArgs, void* base, int32_t id, void* eventData);
    void onGapEvent(int event, void* parameter);
    void onHidEvent(int32_t id, void* eventData);
    bool initialize();
    void configureAndStartScan();
    void publishDevices();
    void publishStatus(const char* status);
    void publishStatus(const String& status);
    static String keyToText(uint8_t modifier, uint8_t key);

    std::vector<BluetoothDevice> devices_;
    DevicesCallback devicesCallback_;
    ConnectionCallback connectionCallback_;
    TextCallback textCallback_;
    StatusCallback statusCallback_;
    bool initialized_ = false;
    bool configuringScan_ = false;
    bool scanning_ = false;
    bool restartScanAfterStop_ = false;
    String connectingName_;

    static BluetoothKeyboardService* instance_;
};

} // namespace DeclarativeUI::Examples::BluetoothKeyboard
