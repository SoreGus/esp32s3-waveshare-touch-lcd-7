#pragma once

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

#include "subscription.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class Binding {
public:
    using Getter = std::function<T()>;
    using Setter = std::function<void(T)>;
    using Subscriber = std::function<Subscription(std::function<void()>)>;

    Binding() = default;
    Binding(Getter getter, Subscriber subscriber, Setter setter = {})
        : getter_(std::move(getter)), setter_(std::move(setter)), subscriber_(std::move(subscriber)) {}

    [[nodiscard]] T get() const { return getter_(); }
    [[nodiscard]] bool writable() const { return setter_.has_value(); }
    void set(T value) const { if (setter_) (*setter_)(std::move(value)); }

    Subscription subscribe(std::function<void()> callback) const
    {
        return subscriber_(std::move(callback));
    }

    template <typename Transform>
    auto map(Transform transform) const
        -> Binding<std::decay_t<std::invoke_result_t<Transform, T>>>
    {
        using Result = std::decay_t<std::invoke_result_t<Transform, T>>;
        const Binding<T> source = *this;
        return Binding<Result>(
            [source, transform] { return transform(source.get()); },
            [source](std::function<void()> callback) { return source.subscribe(std::move(callback)); }
        );
    }

private:
    Getter getter_;
    std::optional<Setter> setter_;
    Subscriber subscriber_;
};

template <typename Source>
auto Bind(const Source& source) -> decltype(source.binding())
{
    return source.binding();
}

template <typename Source, typename Transform>
auto Bind(const Source& source, Transform transform)
    -> decltype(source.binding().map(std::move(transform)))
{
    return source.binding().map(std::move(transform));
}

} // namespace DeclarativeUI::Reactive
