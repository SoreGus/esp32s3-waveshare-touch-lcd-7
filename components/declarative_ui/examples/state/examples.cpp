#include "examples.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::State {

View counter()
{
    DeclarativeUI::State<int> count(0);
    View content = VStack({
        Shared::sectionLabel("A State stores a value owned by this screen."),
        Shared::metric(Bind(count, [](int value) { return String::format("Count: %d", value); })),
        Shared::actionButton("Increment", [count] { count.update([](int value) { return value + 1; }); }),
        Shared::actionButton("Reset", [count] { count.set(0); })
    });
    content.fillWidth().spacing(16);
    return Shared::screen("State / Counter", Shared::fullCard(content));
}

View visibility()
{
    DeclarativeUI::State<bool> visible(true);
    View content = VStack({
        Shared::sectionLabel("The binding updates this view whenever the value changes."),
        Text("This text is controlled by State<bool>")
            .foregroundColor(Color::Cyan())
            .padding(14)
            .background(Color::hex(0x243447))
            .cornerRadius(10)
            .hidden(Bind(visible, [](bool value) { return !value; })),
        Shared::actionButton("Toggle text", [visible] {
            visible.update([](bool value) { return !value; });
        })
    });
    content.fillWidth().spacing(16);
    return Shared::screen("State / Visibility", Shared::fullCard(content));
}

} // namespace DeclarativeUI::Examples::State
