#pragma once

#include <memory>
#include <mutex>
#include <utility>

#include "binding.hpp"
#include "publisher.hpp"
#include "scheduler.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class State {
public:
    State() : storage_(std::make_shared<Storage>()) {}
    explicit State(T value) : storage_(std::make_shared<Storage>(std::move(value))) {}

    [[nodiscard]] T get() const
    {
        std::lock_guard<std::mutex> lock(storage_->mutex);
        return storage_->value;
    }

    void set(T value) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            storage_->value = std::move(value);
        }
        const T current = get();
        Scheduler::notify([publisher = storage_->publisher, current] {
            publisher.publish(current);
        });
    }

    template <typename Update>
    void update(Update update) const
    {
        set(update(get()));
    }

    [[nodiscard]] Binding<T> binding() const
    {
        const State<T> state = *this;
        return Binding<T>(
            [state] { return state.get(); },
            [state](std::function<void()> callback) {
                return state.storage_->publisher.subscribe(
                    [callback](const T&) { callback(); }
                );
            },
            [state](T value) { state.set(std::move(value)); }
        );
    }

private:
    struct Storage {
        Storage() = default;
        explicit Storage(T initialValue) : value(std::move(initialValue)) {}
        mutable std::mutex mutex;
        T value{};
        Publisher<T> publisher;
    };
    std::shared_ptr<Storage> storage_;
};

} // namespace DeclarativeUI::Reactive
