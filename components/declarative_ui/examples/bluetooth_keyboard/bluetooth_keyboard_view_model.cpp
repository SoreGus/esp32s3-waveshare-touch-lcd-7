#include "bluetooth_keyboard_view_model.hpp"

namespace DeclarativeUI::Examples::BluetoothKeyboard {

BluetoothKeyboardViewModel::BluetoothKeyboardViewModel()
    : service_(std::make_shared<BluetoothKeyboardService>())
{
    service_->setCallbacks(
        [this](const std::vector<BluetoothDevice>& found) {
            devices.replaceAll(Array<BluetoothDevice>(found));
        },
        [this](bool connected, const String& name) {
            connectedDevice.set(connected ? name : String());
        },
        [this](const String& text) { appendText(text); },
        [this](const String& value) { status.set(value); }
    );
}

void BluetoothKeyboardViewModel::scan()
{
    service_->startScan();
}

void BluetoothKeyboardViewModel::connect(const BluetoothDevice& device)
{
    service_->connect(device);
}

void BluetoothKeyboardViewModel::appendText(const String& text)
{
    std::string current = typedText.get().std();
    if (text.std() == "\b") {
        if (!current.empty()) current.pop_back();
    } else {
        current += text.std();
    }
    typedText.set(String(std::move(current)));
}

} // namespace DeclarativeUI::Examples::BluetoothKeyboard
