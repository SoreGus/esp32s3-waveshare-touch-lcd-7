#include "dashboard_view_model.hpp"

namespace DeclarativeUI::Examples::StateObject {

void DashboardViewModel::increaseTemperature()
{
    temperature.update([](int value) { return value < 35 ? value + 1 : value; });
}

void DashboardViewModel::resetTemperature()
{
    temperature.set(24);
}

void DashboardViewModel::toggleConnection()
{
    online.update([](bool value) { return !value; });
}

} // namespace DeclarativeUI::Examples::StateObject
