#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <utility>

#include "core/array.hpp"
#include "publisher.hpp"
#include "scheduler.hpp"

namespace DeclarativeUI::Reactive {

enum class ListChangeType { Insert, Remove, Replace, Move, Reset };

template <typename T>
struct ListChange {
    ListChangeType type = ListChangeType::Reset;
    size_t index = 0;
    size_t destination = 0;
};

template <typename T>
class ListState {
public:
    ListState() : storage_(std::make_shared<Storage>()) {}
    explicit ListState(Array<T> values) : storage_(std::make_shared<Storage>(std::move(values))) {}
    ListState(std::initializer_list<T> values) : ListState(Array<T>(values)) {}

    [[nodiscard]] Array<T> values() const
    {
        std::lock_guard<std::mutex> lock(storage_->mutex);
        return storage_->values;
    }

    void append(T value) const { insert(values().size(), std::move(value)); }
    void insert(size_t index, T value) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            storage_->values.values().insert(storage_->values.values().begin() + index, std::move(value));
        }
        publish({ListChangeType::Insert, index, index});
    }

    void removeAt(size_t index) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            storage_->values.values().erase(storage_->values.values().begin() + index);
        }
        publish({ListChangeType::Remove, index, index});
    }

    void replace(size_t index, T value) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            storage_->values[index] = std::move(value);
        }
        publish({ListChangeType::Replace, index, index});
    }

    void move(size_t from, size_t to) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            auto& values = storage_->values.values();
            T value = std::move(values[from]);
            values.erase(values.begin() + from);
            values.insert(values.begin() + to, std::move(value));
        }
        publish({ListChangeType::Move, from, to});
    }

    void clear() const { replaceAll(Array<T>()); }
    void replaceAll(Array<T> values) const
    {
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            storage_->values = std::move(values);
        }
        publish({ListChangeType::Reset, 0, 0});
    }

    [[nodiscard]] Reactive::Subscription subscribe(std::function<void(const ListChange<T>&)> callback) const
    {
        return storage_->publisher.subscribe(std::move(callback));
    }

private:
    void publish(ListChange<T> change) const
    {
        const Publisher<ListChange<T>> publisher = storage_->publisher;
        Scheduler::notify([publisher, change] { publisher.publish(change); });
    }

    struct Storage {
        Storage() = default;
        explicit Storage(Array<T> initialValues) : values(std::move(initialValues)) {}
        mutable std::mutex mutex;
        Array<T> values;
        Publisher<ListChange<T>> publisher;
    };
    std::shared_ptr<Storage> storage_;
};

} // namespace DeclarativeUI::Reactive
