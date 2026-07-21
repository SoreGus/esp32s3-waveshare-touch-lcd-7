#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "subscription.hpp"

namespace DeclarativeUI::Reactive {

template <typename T>
class Publisher {
public:
    using Callback = std::function<void(const T&)>;

    Subscription subscribe(Callback callback) const
    {
        size_t id;
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            id = storage_->nextId++;
            storage_->callbacks.emplace(id, std::move(callback));
        }
        std::weak_ptr<Storage> storage = storage_;
        return Subscription([storage, id] {
            if (const auto shared = storage.lock()) {
                std::lock_guard<std::mutex> lock(shared->mutex);
                shared->callbacks.erase(id);
            }
        });
    }

    void publish(const T& value) const
    {
        std::vector<Callback> callbacks;
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            for (const auto& [_, callback] : storage_->callbacks) {
                callbacks.push_back(callback);
            }
        }
        for (const Callback& callback : callbacks) {
            callback(value);
        }
    }

private:
    struct Storage {
        std::mutex mutex;
        size_t nextId = 0;
        std::map<size_t, Callback> callbacks;
    };
    std::shared_ptr<Storage> storage_ = std::make_shared<Storage>();
};

class Signal {
public:
    using Callback = std::function<void()>;

    Subscription subscribe(Callback callback) const
    {
        size_t id;
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            id = storage_->nextId++;
            storage_->callbacks.emplace(id, std::move(callback));
        }
        std::weak_ptr<Storage> storage = storage_;
        return Subscription([storage, id] {
            if (const auto shared = storage.lock()) {
                std::lock_guard<std::mutex> lock(shared->mutex);
                shared->callbacks.erase(id);
            }
        });
    }

    void publish() const
    {
        std::vector<Callback> callbacks;
        {
            std::lock_guard<std::mutex> lock(storage_->mutex);
            for (const auto& [_, callback] : storage_->callbacks) callbacks.push_back(callback);
        }
        for (const Callback& callback : callbacks) callback();
    }

private:
    struct Storage {
        std::mutex mutex;
        size_t nextId = 0;
        std::map<size_t, Callback> callbacks;
    };
    std::shared_ptr<Storage> storage_ = std::make_shared<Storage>();
};

} // namespace DeclarativeUI::Reactive
