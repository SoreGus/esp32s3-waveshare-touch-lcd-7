#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::StateObject {

class DashboardViewModel final : public ObservableObject {
public:
    Published<String> title = published(String("Living room"));
    Published<int> temperature = published(24);
    Published<bool> online = published(true);

    void increaseTemperature();
    void resetTemperature();
    void toggleConnection();
};

} // namespace DeclarativeUI::Examples::StateObject
