#pragma once

#include "core/view.hpp"

namespace DeclarativeUI {

View NavigationLink(const char* title, View destination);
View NavigationLink(View label, View destination);
View NavigationBackLink(const char* title = "Back");

} // namespace DeclarativeUI
