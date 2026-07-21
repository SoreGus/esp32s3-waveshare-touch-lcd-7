#include "examples.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::ForEach {

namespace {
struct Sensor { int id; String name; int value; };
}

View sensors()
{
    DeclarativeUI::ListState<Sensor> values({
        {1, "Temperature", 24},
        {2, "Humidity", 68}
    });
    View content = VStack({
        Shared::sectionLabel("Live sensors"),
        ::DeclarativeUI::ForEach(values, [](const Sensor& sensor) { return sensor.id; }, [](const Sensor& sensor) {
            View details = VStack({
                Text(sensor.name).foregroundColor(Color::White()),
                Text("Live sensor").foregroundColor(Color::Gray().lighten(24))
            });
            details.spacing(3);

            View reading = Text(String::format("%d", sensor.value));
            reading.foregroundColor(Color::Cyan());

            View row = HStack({
                details, Spacer(), reading
            });
            row.fillWidth().alignment(Alignment::Center);
            return Shared::row(row, Color::Cyan());
        }).spacing(12)
    });
    content.fillWidth().spacing(12);

    View actions = HStack({
        Shared::bottomActionButton("Add pressure", [values] {
            const int id = static_cast<int>(values.values().size()) + 10;
            values.append({id, "Pressure", 1013});
        })
    });
    actions.fillWidth();

    return Shared::collectionScreen(Shared::card(content), actions);
}

View cards()
{
    DeclarativeUI::ListState<int> values({1, 2, 3});
    View content = VStack({
        Shared::sectionLabel("Generated cards"),
        ::DeclarativeUI::ForEach(values, [](int value) { return value; }, [](int value) {
            View marker = Circle();
            marker.frame(34, 34)
                .fill(Color::Blue().lighten(static_cast<uint8_t>(value * 8)));

            View label = Text(String::format("Generated card %d", value));
            label.foregroundColor(Color::White());

            View row = HStack({marker, label, Spacer(), Text("Ready").foregroundColor(Color::Teal())});
            row.fillWidth().alignment(Alignment::Center).spacing(12);
            return Shared::row(row, Color::Blue());
        }).spacing(12)
    });
    content.fillWidth().spacing(12);

    View actions = HStack({
        Shared::bottomActionButton("Add card", [values] {
            values.append(static_cast<int>(values.values().size()) + 1);
        })
    });
    actions.fillWidth();

    return Shared::collectionScreen(Shared::card(content), actions);
}

} // namespace DeclarativeUI::Examples::ForEach
