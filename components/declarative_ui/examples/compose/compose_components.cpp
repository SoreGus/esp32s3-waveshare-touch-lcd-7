#include "compose_components.hpp"

#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::Compose::Components {

View hero(const StateObject<ComposeViewModel>& model)
{
    View backdrop = RoundedRectangle(22);
    backdrop.fillWidth().height(154).fill(Color::hex(0x254B78));

    View content = VStack({
        Text(Bind(model->title)).foregroundColor(Color::White()),
        Text(Bind(model->focusMode, [](bool enabled) {
            return String(enabled ? "FOCUS MODE  •  distractions muted" : "YOUR COLORFUL WORKSPACE");
        })).foregroundColor(Bind(model->focusMode, [](bool enabled) {
            return enabled ? Color::Yellow() : Color::Cyan();
        }))
    });
    content.fill().padding(22).justify(Justify::SpaceBetween);

    return ZStack({backdrop, content})
        .fillWidth()
        .height(154);
}

View progress(const StateObject<ComposeViewModel>& model)
{
    View content = HStack({
        VStack({
            Text("PROGRESS").foregroundColor(Color::Gray().lighten(28)),
            Text(Bind(model->completed, [](int value) {
                return String::format("%d tasks completed", value);
            })).foregroundColor(Color::White())
        }).spacing(4),
        Spacer(),
        Text(Bind(model->focusMode, [](bool enabled) {
            return String(enabled ? "FOCUS" : "READY");
        })).foregroundColor(Bind(model->focusMode, [](bool enabled) {
            return enabled ? Color::Yellow() : Color::Teal();
        }))
    });
    content.fillWidth().alignment(Alignment::Center);

    return Shared::row(content, Color::Teal());
}

View taskRow(const ComposeTask& task)
{
    View marker = Circle();
    marker.frame(38, 38).fill(task.accent);

    View details = VStack({
        Text(task.title).foregroundColor(Color::White()),
        Text(task.detail).foregroundColor(Color::Gray().lighten(22))
    });
    details.spacing(4);

    View status = Text(task.completed ? "DONE" : "NEXT");
    status.foregroundColor(task.completed ? Color::Green() : task.accent);

    View content = HStack({marker, details, Spacer(), status});
    content.fillWidth().alignment(Alignment::Center).spacing(12);

    return Shared::row(content, task.accent);
}

} // namespace DeclarativeUI::Examples::Compose::Components
