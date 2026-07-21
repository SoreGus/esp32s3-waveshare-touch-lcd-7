#pragma once

#include <memory>
#include <vector>

#include "bluetooth_keyboard_service.hpp"
#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::BluetoothKeyboard {

class BluetoothKeyboardViewModel final : public ObservableObject {
public:
    BluetoothKeyboardViewModel();

    Published<String> status = published(String("Toque em procurar dispositivos para iniciar."));
    Published<String> connectedDevice = published(String());
    Published<String> typedText = published(String());
    PublishedList<BluetoothDevice> devices = publishedList<BluetoothDevice>({});

    void scan();
    void connect(const BluetoothDevice& device);

private:
    void appendText(const String& text);

    std::shared_ptr<BluetoothKeyboardService> service_;
};

} // namespace DeclarativeUI::Examples::BluetoothKeyboard
