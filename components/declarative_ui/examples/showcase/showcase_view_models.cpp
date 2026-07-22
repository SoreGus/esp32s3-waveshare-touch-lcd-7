#include "showcase_view_models.hpp"

namespace DeclarativeUI::Examples::Showcase {

void HomeViewModel::startDemo()
{
    launches.update([](int value) { return value + 1; });
}

void HomeViewModel::toggleCompactMode()
{
    compactMode.update([](bool value) { return !value; });
}

void ControlsViewModel::reset()
{
    brightness.set(65);
    palette.set(0);
    enabled.set(true);
    mode.set(1);
}

void TasksViewModel::completeNext()
{
    auto values = items.values();
    for (size_t index = 0; index < values.size(); ++index) {
        auto& item = values[index];
        if (!item.complete) {
            item.complete = true;
            items.replaceAll(values);
            completed.update([](int value) { return value + 1; });
            return;
        }
    }

    items.append({nextItemId_++, "New offline task", "Added without Wi-Fi or external services", Color::Teal(), false});
}

} // namespace DeclarativeUI::Examples::Showcase
