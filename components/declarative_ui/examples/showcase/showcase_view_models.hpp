#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::Showcase {

class HomeViewModel final : public ObservableObject {
public:
    Published<int> launches = published(3);
    Published<bool> compactMode = published(false);

    void startDemo();
    void toggleCompactMode();
};

class ControlsViewModel final : public ObservableObject {
public:
    Published<int> brightness = published(65);
    Published<int> palette = published(0);
    Published<bool> enabled = published(true);
    Published<int> mode = published(1);

    void reset();
};

struct DemoItem {
    int id = 0;
    String name;
    String detail;
    Color accent = Color::Blue();
    bool complete = false;
};

class TasksViewModel final : public ObservableObject {
public:
    PublishedList<DemoItem> items = publishedList(Array<DemoItem>({
        {1, "Build the layout", "VStack, HStack and ZStack", Color::Cyan(), true},
        {2, "Tune controls", "Slider, Picker, Toggle and segments", Color::Purple(), false},
        {3, "Review navigation", "Open a screen and return safely", Color::Orange(), false}
    }));
    Published<int> completed = published(1);

    void completeNext();

private:
    int nextItemId_ = 4;
};

} // namespace DeclarativeUI::Examples::Showcase
