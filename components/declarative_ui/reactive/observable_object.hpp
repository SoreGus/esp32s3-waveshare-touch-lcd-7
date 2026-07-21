#pragma once

#include <type_traits>
#include <utility>

#include "core/array.hpp"
#include "publisher.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class Published;

template <typename T>
class PublishedList;

class ObservableObject {
public:
    virtual ~ObservableObject() = default;
    [[nodiscard]] const Signal& objectWillChange() const { return objectWillChange_; }
    void notifyObjectWillChange() const { objectWillChange_.publish(); }

protected:
    template <typename T>
    auto published(T&& value) const -> Published<std::decay_t<T>>;

    template <typename T>
    auto publishedList(Array<T> values) const -> PublishedList<T>;

private:
    Signal objectWillChange_;
};

} // namespace DeclarativeUI::Reactive
