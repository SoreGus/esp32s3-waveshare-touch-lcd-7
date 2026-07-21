#pragma once

#include "core/view.hpp"

#include "layout/stack.hpp"
#include "layout/scroll_view.hpp"

#include "views/text.hpp"
#include "views/shape.hpp"
#include "views/button.hpp"

namespace DeclarativeUI {

class UI {
public:
    static void initialize();

    static void render(const View& root);

    static void run(const View& root);

    static void clear();
};

} // namespace DeclarativeUI