#include "examples.hpp"
#include "examples/shared/example_layout.hpp"
#include "dashboard_view_model.hpp"

namespace DeclarativeUI::Examples::StateObject {

View dashboard()
{
    DeclarativeUI::StateObject<DashboardViewModel> model;

    View content = VStack({
        Shared::sectionLabel("The View observes Published values and calls ViewModel actions."),
        Text(Bind(model->title))
            .foregroundColor(Color::Cyan()),
        Shared::metric(Bind(model->temperature, [](int value) {
            return String::format("%d °C", value);
        })),
        Text(Bind(model->online, [](bool value) { return String(value ? "CONNECTED" : "DISCONNECTED"); }))
            .foregroundColor(Bind(model->online, [](bool value) {
                return value ? Color::Green() : Color::Red();
            })),
        Shared::actionButton("Increase temperature", [model] {
            model->increaseTemperature();
        }),
        Shared::actionButton("Reset temperature", [model] {
            model->resetTemperature();
        }),
        Shared::actionButton("Toggle connection", [model] {
            model->toggleConnection();
        })
    });
    content.fillWidth().spacing(16);

    return Shared::screen("StateObject / Dashboard", Shared::fullCard(content));
}

} // namespace DeclarativeUI::Examples::StateObject
