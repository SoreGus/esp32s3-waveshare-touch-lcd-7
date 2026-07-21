#pragma once

#include <functional>
#include <utility>

namespace DeclarativeUI::Reactive {

class Subscription {
public:
    Subscription() = default;
    explicit Subscription(std::function<void()> cancel) : cancel_(std::move(cancel)) {}
    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;
    Subscription(Subscription&& other) noexcept : cancel_(std::move(other.cancel_)) {}
    Subscription& operator=(Subscription&& other) noexcept
    {
        reset();
        cancel_ = std::move(other.cancel_);
        return *this;
    }
    ~Subscription() { reset(); }

    void reset()
    {
        if (cancel_) {
            cancel_();
            cancel_ = nullptr;
        }
    }

private:
    std::function<void()> cancel_;
};

} // namespace DeclarativeUI::Reactive
