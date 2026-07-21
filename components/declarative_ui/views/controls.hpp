#pragma once

#include <initializer_list>

#include "core/array.hpp"
#include "core/view.hpp"

namespace DeclarativeUI {

enum class SliderThumb {
    Circle,
    Pill
};

View Slider(const Reactive::Binding<int>& value, int minimum = 0, int maximum = 100);
View Picker(std::initializer_list<const char*> options, const Reactive::Binding<int>& selected);
View Picker(const Array<String>& options, const Reactive::Binding<int>& selected);
View Toggle(const Reactive::Binding<bool>& value);
View SegmentedControl(std::initializer_list<const char*> options, const Reactive::Binding<int>& selected);
View SegmentedControl(const Array<String>& options, const Reactive::Binding<int>& selected);

} // namespace DeclarativeUI
