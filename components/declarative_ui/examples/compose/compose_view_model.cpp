#include "compose_view_model.hpp"

namespace DeclarativeUI::Examples::Compose {

void ComposeViewModel::addTask()
{
    tasks.append({
        nextTaskId_++,
        String("New idea"),
        String("Created from the fixed action bar"),
        Color::Teal(),
        false
    });
}

void ComposeViewModel::completeNextTask()
{
    const Array<ComposeTask> values = tasks.values();
    for (size_t index = 0; index < values.size(); ++index) {
        if (values[index].completed) {
            continue;
        }

        ComposeTask task = values[index];
        task.completed = true;
        tasks.replace(index, std::move(task));
        completed.update([](int value) { return value + 1; });
        return;
    }
}

void ComposeViewModel::toggleFocusMode()
{
    focusMode.update([](bool value) { return !value; });
}

} // namespace DeclarativeUI::Examples::Compose
