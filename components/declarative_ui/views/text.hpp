#pragma once

#include "core/view.hpp"

namespace DeclarativeUI {

View Text(const char* value);

View Text(const String& value);

View Text(const Reactive::Binding<String>& value);

} // namespace DeclarativeUI
