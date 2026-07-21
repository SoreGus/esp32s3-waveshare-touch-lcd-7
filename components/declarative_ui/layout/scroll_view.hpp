#pragma once

#include <initializer_list>

#include "core/view.hpp"

namespace DeclarativeUI {

View ScrollView(
    std::initializer_list<View> children
);

View ScrollView(
    ScrollAxis axis,
    std::initializer_list<View> children
);

} // namespace DeclarativeUI