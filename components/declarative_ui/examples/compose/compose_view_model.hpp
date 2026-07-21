#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::Compose {

struct ComposeTask {
    int id = 0;
    String title;
    String detail;
    Color accent = Color::Blue();
    bool completed = false;
};

class ComposeViewModel final : public ObservableObject {
public:
    Published<String> title = published(String("Today at a glance"));
    Published<int> completed = published(1);
    Published<bool> focusMode = published(false);
    ListState<ComposeTask> tasks {
        {1, "Project review", "Prepare the visual prototype", Color::Cyan(), true},
        {2, "Display layout", "Tune spacing for the 7 inch panel", Color::Purple(), false},
        {3, "Reactive API", "Test ListState updates", Color::Orange(), false}
    };

    void addTask();
    void completeNextTask();
    void toggleFocusMode();

private:
    int nextTaskId_ = 4;
};

} // namespace DeclarativeUI::Examples::Compose
