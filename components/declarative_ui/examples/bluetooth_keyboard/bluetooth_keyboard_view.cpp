#include "examples.hpp"

#include "bluetooth_keyboard_view_model.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::BluetoothKeyboard {

View showcase()
{
    StateObject<BluetoothKeyboardViewModel> model;

    View status = Text(Bind(model->status))
        .fillWidth()
        .foregroundColor(Color::LightGray())
        .textAlignment(TextAlignment::Center);

    View connected = Text(Bind(model->connectedDevice, [](const String& name) {
        return name.empty() ? String("Nenhum teclado conectado") : String(std::string("Conectado: ") + name.std());
    }))
        .fillWidth()
        .foregroundColor(Color::Cyan())
        .textAlignment(TextAlignment::Center);

    View devices = ForEach(model->devices, [](const BluetoothDevice& device) { return device.address; }, [model](const BluetoothDevice& device) {
        return HStack({
            VStack({
                Text(device.name).foregroundColor(Color::White()),
                Text(String::format("%s  %s  %d dBm", device.address.c_str(), device.profile.c_str(), device.rssi)).foregroundColor(Color::LightGray())
            }).grow().spacing(4),
            Button("Conectar").onTap([model, device] { model->connect(device); })
        }).fillWidth().alignment(Alignment::Center).spacing(10)
          .padding(12).background(Color::hex(0x223246)).cornerRadius(12);
    });
    devices.fillWidth().spacing(10);

    View content = VStack({
        Shared::actionButton("Procurar dispositivos", [model] { model->scan(); }),
        status,
        connected,
        Text("DISPOSITIVOS ENCONTRADOS").foregroundColor(Color::Gray().lighten(28)),
        devices,
        Text("TEXTO RECEBIDO").foregroundColor(Color::Gray().lighten(28)),
        TextField(Bind(model->typedText), "Conecte um teclado BLE para digitar aqui...")
    });
    content.fillWidth().spacing(14);
    return Shared::scrollingScreen("Teclado Bluetooth BLE", Shared::card(content));
}

} // namespace DeclarativeUI::Examples::BluetoothKeyboard
