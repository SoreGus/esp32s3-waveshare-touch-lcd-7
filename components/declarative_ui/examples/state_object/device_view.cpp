#include "examples.hpp"
#include "examples/shared/example_layout.hpp"
#include "device_view_model.hpp"

namespace DeclarativeUI::Examples::StateObject {

View device()
{
    DeclarativeUI::StateObject<DeviceViewModel> model;

    View content = VStack({
        Shared::sectionLabel("Actions and validation belong to the ViewModel implementation."),
        Text(Bind(model->name))
            .foregroundColor(Color::Cyan()),
        Shared::metric(Bind(model->brightness, [](int value) {
            return String::format("Brightness: %d%%", value);
        })),
        Shared::actionButton("Increase brightness", [model] {
            model->increaseBrightness();
        }),
        Shared::actionButton("Decrease brightness", [model] {
            model->decreaseBrightness();
        }),
        Shared::actionButton("Reset brightness", [model] {
            model->resetBrightness();
        })
    });
    content.fillWidth().spacing(16);

    return Shared::screen("StateObject / Device", Shared::fullCard(content));
}

} // namespace DeclarativeUI::Examples::StateObject
