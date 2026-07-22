#include "examples.hpp"

#include "examples/navigation/navigation_view_model.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::Navigation {

namespace {

View deviceDetails(const Device& device, StateObject<NavigationViewModel> model)
{
    View details = VStack({
        Text(device.name).foregroundColor(Color::White()),
        Text(device.room).foregroundColor(Color::LightGray()),
        Rectangle().fill(device.status == String("Online") ? Color::Teal() : Color::Orange()).frame(80, 5).cornerRadius(3),
        Text(device.status).foregroundColor(Color::Cyan()),
        Text(Bind(model->refreshes, [](int value) {
            return String::format("Synchronized %d time(s)", value);
        })).foregroundColor(Color::Gray().lighten(22))
    }).fillWidth().spacing(14);

    return VStack({
        ToolBar({
            NavigationBackLink(),
            Title(device.name),
            Spacer(),
            ToolBarItem("Refresh", [model] { model->refresh(); })
        }),
        Shared::card(details),
        Spacer()
    }).fill().padding(20).spacing(16).background(Color::hex(0x101820));
}

View devicesScreen(StateObject<NavigationViewModel> model)
{
    View list = ::DeclarativeUI::ForEach(
        model->devices,
        [](const Device& device) { return device.id; },
        [model](const Device& device) {
            return NavigationLink(
                String::format("%s  •  %s", device.name.c_str(), device.status.c_str()).c_str(),
                deviceDetails(device, model)
            )
                .fillWidth()
                .background(Color::hex(0x223246))
                .border(Color::hex(0x334155), 1)
                .padding(14, 12)
                .cornerRadius(14);
        }
    ).spacing(10);

    return VStack({
        ToolBar({
            Title("Devices"),
            Spacer(),
            ToolBarItem("Refresh", [model] { model->refresh(); })
        }),
        Text("Select a device to see its details.").foregroundColor(Color::LightGray()),
        ScrollView({list}).grow().fillWidth().paddingRight(6)
    }).fill().padding(20).spacing(16).background(Color::hex(0x101820));
}

} // namespace

View showcase()
{
    StateObject<NavigationViewModel> model;
    return NavigationStack({devicesScreen(model)});
}

} // namespace DeclarativeUI::Examples::Navigation
