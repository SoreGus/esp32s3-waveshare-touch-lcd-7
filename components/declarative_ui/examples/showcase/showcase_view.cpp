#include "examples.hpp"

#include "examples/shared/example_layout.hpp"
#include "logo_lvgl.hpp"
#include "showcase_view_models.hpp"

namespace DeclarativeUI::Examples::Showcase {

namespace {

View homeScreen(StateObject<HomeViewModel> model);
View controlsScreen(StateObject<ControlsViewModel> model);
View tasksScreen(StateObject<TasksViewModel> model);

View navigationBar(const char* title)
{
    return ToolBar({NavigationBackLink(), Title(title), Spacer()});
}

View homeScreen(StateObject<HomeViewModel> model)
{
    View logo = Image(&Assets::logoLvgl);
    logo.width(300).height(95);

    View hero = VStack({
        logo,
        Text("DeclarativeUI offline showcase").foregroundColor(Color::White()),
        Text("Every interaction in this example runs locally on the device.")
            .foregroundColor(Color::LightGray())
    }).fillWidth().alignment(Alignment::Center).spacing(12);

    View status = Shared::row(HStack({
        VStack({
            Text("LOCAL SESSION").foregroundColor(Color::Gray().lighten(25)),
            Text(Bind(model->launches, [](int count) {
                return String::format("%d interactions", count);
            })).foregroundColor(Color::White())
        }).spacing(4),
        Spacer(),
        Toggle(Bind(model->compactMode))
            .toggleColors(Color::Teal(), Color::hex(0x475569))
    }).fillWidth().alignment(Alignment::Center), Color::Teal());

    View destinationControls = controlsScreen(StateObject<ControlsViewModel>());
    View destinationTasks = tasksScreen(StateObject<TasksViewModel>());

    View menu = VStack({
        NavigationLink("Controls", destinationControls).fillWidth().padding(14).background(Color::hex(0x223246)).cornerRadius(14),
        NavigationLink("Task board", destinationTasks).fillWidth().padding(14).background(Color::hex(0x223246)).cornerRadius(14),
        Button("Record local interaction").fillWidth().onTap([model] { model->startDemo(); }),
        Button("Toggle compact mode").fillWidth().onTap([model] { model->toggleCompactMode(); })
    }).fillWidth().spacing(10);

    return VStack({
        ToolBar({Title("Showcase"), Spacer()}),
        ScrollView({VStack({Shared::card(hero), status, menu}).fillWidth().spacing(16)}).grow().fillWidth()
    }).fill().padding(20).spacing(16).background(Color::hex(0x101820));
}

View controlsScreen(StateObject<ControlsViewModel> model)
{
    View content = VStack({
        Text(Bind(model->brightness, [](int value) { return String::format("Brightness  %d%%", value); })).foregroundColor(Color::White()),
        Slider(Bind(model->brightness), 0, 100).sliderFilledColor(Color::Cyan()).sliderThumb(SliderThumb::Pill),
        Picker({"Ocean", "Sunset", "Forest"}, Bind(model->palette)).pickerColors(Color::hex(0x223246), Color::White(), Color::Indigo()),
        Toggle(Bind(model->enabled)).toggleColors(Color::Teal(), Color::DarkGray()),
        SegmentedControl({"Eco", "Normal", "Boost"}, Bind(model->mode)).segmentedColors(Color::hex(0x223246), Color::Purple()),
        Button("Reset controls").fillWidth().onTap([model] { model->reset(); })
    }).fillWidth().spacing(16);

    return VStack({navigationBar("Controls"), ScrollView({Shared::card(content)}).grow().fillWidth()})
        .fill().padding(20).spacing(16).background(Color::hex(0x101820));
}

View tasksScreen(StateObject<TasksViewModel> model)
{
    View list = ForEach(model->items,
        [](const DemoItem& item) { return item.id; },
        [](const DemoItem& item) {
            return Shared::row(HStack({
                Circle().frame(18, 18).fill(item.accent),
                VStack({Text(item.name).foregroundColor(Color::White()), Text(item.detail).foregroundColor(Color::LightGray())}).grow().spacing(3),
                Text(item.complete ? "DONE" : "NEXT").foregroundColor(item.complete ? Color::Green() : item.accent)
            }).fillWidth().alignment(Alignment::Center).spacing(12), item.accent);
        }).spacing(10);

    View summary = Shared::row(HStack({
        Text(Bind(model->completed, [](int value) { return String::format("%d completed", value); })).foregroundColor(Color::White()),
        Spacer(),
        Button("Complete next").onTap([model] { model->completeNext(); })
    }).fillWidth().alignment(Alignment::Center), Color::Purple());

    return VStack({navigationBar("Task board"), summary, ScrollView({list}).grow().fillWidth()})
        .fill().padding(20).spacing(16).background(Color::hex(0x101820));
}

} // namespace

View app()
{
    return NavigationStack({homeScreen(StateObject<HomeViewModel>())});
}

} // namespace DeclarativeUI::Examples::Showcase
