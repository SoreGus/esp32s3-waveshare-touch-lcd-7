#pragma once

#include <functional>
#include <initializer_list>

#include "core/view.hpp"

namespace DeclarativeUI {

View ToolBar(std::initializer_list<View> items);
View ToolBarItem(const char* title, std::function<void()> action);

} // namespace DeclarativeUI
