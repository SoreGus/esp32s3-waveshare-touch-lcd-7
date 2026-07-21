#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::StateObject {

class DeviceViewModel final : public ObservableObject {
public:
    Published<String> name = published(String("ESP32-S3 Touch LCD 7"));
    Published<int> brightness = published(80);

    void increaseBrightness();
    void decreaseBrightness();
    void resetBrightness();
};

} // namespace DeclarativeUI::Examples::StateObject
