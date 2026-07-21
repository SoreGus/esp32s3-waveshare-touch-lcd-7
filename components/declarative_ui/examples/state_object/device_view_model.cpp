#include "device_view_model.hpp"

namespace DeclarativeUI::Examples::StateObject {

void DeviceViewModel::increaseBrightness()
{
    brightness.update([](int value) { return value < 100 ? value + 10 : 100; });
}

void DeviceViewModel::decreaseBrightness()
{
    brightness.update([](int value) { return value > 0 ? value - 10 : 0; });
}

void DeviceViewModel::resetBrightness()
{
    brightness.set(80);
}

} // namespace DeclarativeUI::Examples::StateObject
