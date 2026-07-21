#include "examples.hpp"

#include "examples/gadgets/gadgets_view_model.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::Gadgets {

View showcase()
{
    StateObject<GadgetsViewModel> model;

    View brightness = VStack({
        Text(Bind(model->brightness, [](int value) { return String::format("Brightness  %d%%", value); }))
            .foregroundColor(Color::White()),
        Slider(Bind(model->brightness), 0, 100)
            .sliderFilledColor(Color::Cyan())
            .sliderEmptyColor(Color::hex(0x334155))
            .sliderThumbColor(Color::White())
            .sliderBarHeight(10)
            .sliderThumb(SliderThumb::Pill)
    }).fillWidth().spacing(12);

    View picker = VStack({
        Text("Color theme").foregroundColor(Color::White()),
        Picker({"Ocean", "Sunset", "Forest", "Mono"}, Bind(model->theme))
            .pickerColors(Color::hex(0x223246), Color::White(), Color::Indigo())
    }).fillWidth().spacing(12);

    View toggle = HStack({
        VStack({
            Text("Notifications").foregroundColor(Color::White()),
            Text(Bind(model->notifications, [](bool enabled) { return String(enabled ? "Enabled" : "Paused"); }))
                .foregroundColor(Color::LightGray())
        }).grow(),
        Toggle(Bind(model->notifications))
            .toggleColors(Color::Teal(), Color::hex(0x475569))
            .toggleAnimation(220)
    }).fillWidth().alignment(Alignment::Center);

    View segmented = VStack({
        Text("Operating mode").foregroundColor(Color::White()),
        SegmentedControl({"Eco", "Normal", "Boost"}, Bind(model->mode))
            .segmentedColors(Color::hex(0x223246), Color::Purple())
            .segmentedAnimation(220)
    }).fillWidth().spacing(12);

    View content = VStack({
        Shared::sectionLabel("SLIDER"), Shared::row(brightness, Color::Cyan()),
        Shared::sectionLabel("PICKER"), Shared::row(picker, Color::Indigo()),
        Shared::sectionLabel("TOGGLE"), Shared::row(toggle, Color::Teal()),
        Shared::sectionLabel("SEGMENTED CONTROL"), Shared::row(segmented, Color::Purple())
    });
    content.fillWidth().spacing(16);

    return Shared::scrollingScreen("Gadgets", Shared::card(content));
}

} // namespace DeclarativeUI::Examples::Gadgets
