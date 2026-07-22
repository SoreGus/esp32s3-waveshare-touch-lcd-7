#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::Navigation {

struct Device {
    int id;
    String name;
    String room;
    String status;
};

class NavigationViewModel final : public ObservableObject {
public:
    PublishedList<Device> devices = publishedList(Array<Device>({
        {1, "Living room", "Main floor", "Online"},
        {2, "Kitchen", "Main floor", "Online"},
        {3, "Studio", "Upper floor", "Paused"}
    }));
    Published<int> refreshes = published(0);

    void refresh() const
    {
        refreshes.update([](int value) { return value + 1; });
    }
};

} // namespace DeclarativeUI::Examples::Navigation
