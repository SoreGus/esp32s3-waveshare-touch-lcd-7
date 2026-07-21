#pragma once

#include "list_state.hpp"
#include "observable_object.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class PublishedList {
public:
    PublishedList() = default;
    PublishedList(const ObservableObject& owner, Array<T> values)
        : state_(std::move(values)), owner_(&owner) {}
    PublishedList(const ObservableObject& owner, std::initializer_list<T> values)
        : state_(values), owner_(&owner) {}

    [[nodiscard]] Array<T> values() const { return state_.values(); }
    void append(T value) const { state_.append(std::move(value)); changed(); }
    void insert(size_t index, T value) const { state_.insert(index, std::move(value)); changed(); }
    void removeAt(size_t index) const { state_.removeAt(index); changed(); }
    void replace(size_t index, T value) const { state_.replace(index, std::move(value)); changed(); }
    void move(size_t from, size_t to) const { state_.move(from, to); changed(); }
    void clear() const { state_.clear(); changed(); }
    void replaceAll(Array<T> values) const { state_.replaceAll(std::move(values)); changed(); }
    [[nodiscard]] Subscription subscribe(std::function<void(const ListChange<T>&)> callback) const
    { return state_.subscribe(std::move(callback)); }

private:
    void changed() const { if (owner_ != nullptr) owner_->notifyObjectWillChange(); }
    ListState<T> state_;
    const ObservableObject* owner_ = nullptr;
};

template <typename T>
auto ObservableObject::publishedList(Array<T> values) const -> PublishedList<T>
{
    return PublishedList<T>(*this, std::move(values));
}

} // namespace DeclarativeUI::Reactive
