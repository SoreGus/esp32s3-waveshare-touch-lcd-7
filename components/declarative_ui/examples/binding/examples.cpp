#include "examples.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::Binding {

View status()
{
    DeclarativeUI::State<bool> online(true);
    View content = VStack({
        Shared::sectionLabel("One source drives both text and color."),
        Text(Bind(online, [](bool value) { return String(value ? "ONLINE" : "OFFLINE"); }))
            .foregroundColor(Bind(online, [](bool value) {
                return value ? Color::Green() : Color::Red();
            }))
            .padding(14)
            .background(Color::hex(0x243447))
            .cornerRadius(10),
        Shared::actionButton("Toggle status", [online] {
            online.update([](bool value) { return !value; });
        })
    });
    content.fillWidth().spacing(16);
    return Shared::screen("Binding / Status", Shared::fullCard(content));
}

View colors()
{
    DeclarativeUI::State<bool> active(true);
    View content = VStack({
        Shared::sectionLabel("A shape observes the same State through Bind."),
        Circle().frame(96, 96).fill(Bind(active, [](bool value) {
            return value ? Color::Teal() : Color::Purple();
        })),
        Shared::actionButton("Change color", [active] {
            active.update([](bool value) { return !value; });
        })
    });
    content.fillWidth().spacing(16);
    return Shared::screen("Binding / Color", Shared::fullCard(content));
}

} // namespace DeclarativeUI::Examples::Binding
