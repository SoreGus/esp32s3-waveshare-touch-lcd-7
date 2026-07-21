#pragma once

#include "core/view.hpp"

namespace DeclarativeUI {

/** A multi-line text field synchronized with a reactive String binding. */
View TextField(const Reactive::Binding<String>& value, const char* placeholder = nullptr);

} // namespace DeclarativeUI
