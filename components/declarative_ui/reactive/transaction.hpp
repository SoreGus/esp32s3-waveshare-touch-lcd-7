#pragma once

#include <functional>
#include <vector>

namespace DeclarativeUI::Reactive {

inline thread_local int transactionDepth = 0;
inline thread_local std::vector<std::function<void()>> deferredNotifications;

inline void notify(std::function<void()> callback)
{
    if (transactionDepth > 0) {
        deferredNotifications.push_back(std::move(callback));
        return;
    }
    callback();
}

template <typename Callback>
void transaction(Callback callback)
{
    ++transactionDepth;
    callback();
    --transactionDepth;

    if (transactionDepth != 0) return;

    std::vector<std::function<void()>> notifications = std::move(deferredNotifications);
    deferredNotifications.clear();
    for (const auto& notification : notifications) notification();
}

} // namespace DeclarativeUI::Reactive
