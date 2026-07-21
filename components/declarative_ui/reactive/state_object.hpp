#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace DeclarativeUI::Reactive {

template <typename T>
class StateObject {
public:
    StateObject(const StateObject&) = default;
    StateObject(StateObject&&) noexcept = default;
    StateObject& operator=(const StateObject&) = default;
    StateObject& operator=(StateObject&&) noexcept = default;

    StateObject() : object_(std::make_shared<T>()) {}

    template <
        typename First,
        typename... Rest,
        std::enable_if_t<!std::is_same_v<std::decay_t<First>, StateObject>, int> = 0
    >
    explicit StateObject(First&& first, Rest&&... rest)
        : object_(std::make_shared<T>(
            std::forward<First>(first),
            std::forward<Rest>(rest)...
        )) {}

    [[nodiscard]] T* operator->() const { return object_.get(); }
    [[nodiscard]] T& get() const { return *object_; }

private:
    std::shared_ptr<T> object_;
};

} // namespace DeclarativeUI::Reactive
