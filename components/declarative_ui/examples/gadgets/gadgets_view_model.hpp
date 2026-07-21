#pragma once

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::Gadgets {

class GadgetsViewModel final : public ObservableObject {
public:
    Published<int> brightness = published(72);
    Published<int> theme = published(1);
    Published<bool> notifications = published(true);
    Published<int> mode = published(0);
};

} // namespace DeclarativeUI::Examples::Gadgets
