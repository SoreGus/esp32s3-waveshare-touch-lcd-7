#pragma once

#include <utility>

#include "observable_object.hpp"
#include "state.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class Published {
public:
    Published() = default;
    Published(const ObservableObject& owner, T value)
        : state_(std::move(value)), owner_(&owner)
    {
    }

    [[nodiscard]] T get() const { return state_.get(); }
    void set(T value) const
    {
        state_.set(std::move(value));
        if (owner_ != nullptr) owner_->notifyObjectWillChange();
    }
    template <typename Update>
    void update(Update update) const { set(update(get())); }
    [[nodiscard]] Binding<T> binding() const { return state_.binding(); }

private:
    State<T> state_;
    const ObservableObject* owner_ = nullptr;
};

template <typename T>
auto ObservableObject::published(T&& value) const -> Published<std::decay_t<T>>
{
    return Published<std::decay_t<T>>(*this, std::forward<T>(value));
}

} // namespace DeclarativeUI::Reactive
