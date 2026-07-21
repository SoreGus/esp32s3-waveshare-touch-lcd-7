#pragma once

#include "core/view.hpp"
#include "core/string.hpp"
#include "core/array.hpp"

#include "reactive/state.hpp"
#include "reactive/binding.hpp"
#include "reactive/observable_object.hpp"
#include "reactive/published.hpp"
#include "reactive/state_object.hpp"
#include "reactive/list_state.hpp"
#include "reactive/published_list.hpp"
#include "reactive/transaction.hpp"
#include "reactive/scheduler.hpp"

#include "layout/stack.hpp"
#include "layout/scroll_view.hpp"
#include "layout/for_each.hpp"

#include "views/text.hpp"
#include "views/shape.hpp"
#include "views/button.hpp"
#include "views/text_field.hpp"
#include "views/controls.hpp"

namespace DeclarativeUI {

using Reactive::Bind;
using Reactive::ListState;
using Reactive::ObservableObject;
using Reactive::Published;
using Reactive::PublishedList;
using Reactive::State;
using Reactive::StateObject;

class UI {
public:
    static void initialize();

    static void render(const View& root);

    static void run(const View& root);

    static void clear();
};

} // namespace DeclarativeUI
