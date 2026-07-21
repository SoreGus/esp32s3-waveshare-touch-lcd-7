#pragma once

#include <functional>

#include "transaction.hpp"

namespace DeclarativeUI::Reactive {

class Scheduler {
public:
    static void notify(std::function<void()> callback)
    {
        Reactive::notify(std::move(callback));
    }
};

} // namespace DeclarativeUI::Reactive
