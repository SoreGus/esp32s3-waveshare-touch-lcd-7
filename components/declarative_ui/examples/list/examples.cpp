#include "examples.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::List {

View operations()
{
    DeclarativeUI::ListState<String> messages({"First item", "Second item"});
    DeclarativeUI::State<String> status("2 items");
    View content = VStack({
        Shared::sectionLabel("Reactive message list"),
        Shared::metric(Bind(status)),
        ForEach(messages, [](const String& value) { return value.std(); }, [](const String& value) {
            View label = Text(value);
            label.foregroundColor(Color::White());
            return Shared::row(label, Color::Teal());
        }).spacing(12)
    });
    content.fillWidth().spacing(12);

    View actions = HStack({
        Shared::bottomActionButton("Add", [messages, status] {
            messages.append(String::format("Item %d", static_cast<int>(messages.values().size() + 1)));
            status.set(String::format("%d items", static_cast<int>(messages.values().size())));
        }),
        Shared::bottomActionButton("Remove first", [messages, status] {
            if (!messages.values().empty()) messages.removeAt(0);
            status.set(String::format("%d items", static_cast<int>(messages.values().size())));
        })
    });
    actions.fillWidth().spacing(12);

    return Shared::collectionScreen(Shared::card(content), actions);
}

View replaceAndMove()
{
    DeclarativeUI::ListState<String> values({"One", "Two", "Three"});
    View content = VStack({
        Shared::sectionLabel("Replace and reorder items"),
        ForEach(values, [](const String& value) { return value.std(); }, [](const String& value) {
            View label = Text(value);
            label.foregroundColor(Color::White());
            return Shared::row(label, Color::Purple());
        }).spacing(12)
    });
    content.fillWidth().spacing(12);

    View actions = HStack({
        Shared::bottomActionButton("Replace first", [values] {
            values.replace(0, String("Updated"));
        }),
        Shared::bottomActionButton("Move to first", [values] {
            if (values.values().size() > 1) values.move(values.values().size() - 1, 0);
        })
    });
    actions.fillWidth().spacing(12);

    return Shared::collectionScreen(Shared::card(content), actions);
}

} // namespace DeclarativeUI::Examples::List
