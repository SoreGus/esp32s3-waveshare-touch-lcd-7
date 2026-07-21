#pragma once

#include <initializer_list>

#include "core/view.hpp"

namespace DeclarativeUI {

View HStack(std::initializer_list<View> children);

View VStack(std::initializer_list<View> children);

View ZStack(std::initializer_list<View> children);

View Spacer();

} // namespace DeclarativeUI
