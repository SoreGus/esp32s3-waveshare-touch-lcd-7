#pragma once

#include <initializer_list>

#include "core/view.hpp"

namespace DeclarativeUI {

View NavigationStack(std::initializer_list<View> children);

} // namespace DeclarativeUI
