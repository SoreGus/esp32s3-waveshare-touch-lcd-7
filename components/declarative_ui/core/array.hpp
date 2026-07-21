#pragma once

#include <initializer_list>
#include <utility>
#include <vector>

namespace DeclarativeUI {

template <typename T>
class Array {
public:
    Array() = default;
    Array(std::initializer_list<T> values) : values_(values) {}
    explicit Array(std::vector<T> values) : values_(std::move(values)) {}

    [[nodiscard]] size_t size() const { return values_.size(); }
    [[nodiscard]] bool empty() const { return values_.empty(); }
    [[nodiscard]] const T& operator[](size_t index) const { return values_[index]; }
    [[nodiscard]] T& operator[](size_t index) { return values_[index]; }
    [[nodiscard]] auto begin() const { return values_.begin(); }
    [[nodiscard]] auto end() const { return values_.end(); }
    [[nodiscard]] const std::vector<T>& values() const { return values_; }
    [[nodiscard]] std::vector<T>& values() { return values_; }

private:
    std::vector<T> values_;
};

} // namespace DeclarativeUI
