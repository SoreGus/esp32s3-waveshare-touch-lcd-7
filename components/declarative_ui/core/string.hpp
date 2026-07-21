#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>
#include <utility>

namespace DeclarativeUI {

class String {
public:
    String() = default;
    String(const char* value) : value_(value != nullptr ? value : "") {}
    String(std::string value) : value_(std::move(value)) {}

    [[nodiscard]] const char* c_str() const { return value_.c_str(); }
    [[nodiscard]] const std::string& std() const { return value_; }
    [[nodiscard]] bool empty() const { return value_.empty(); }
    [[nodiscard]] size_t size() const { return value_.size(); }

    static String format(const char* format, ...)
    {
        va_list arguments;
        va_start(arguments, format);
        va_list copy;
        va_copy(copy, arguments);
        const int length = std::vsnprintf(nullptr, 0, format, copy);
        va_end(copy);

        std::string value(length > 0 ? static_cast<size_t>(length) : 0, '\0');
        if (length > 0) {
            std::vsnprintf(value.data(), value.size() + 1, format, arguments);
        }
        va_end(arguments);
        return String(std::move(value));
    }

    friend bool operator==(const String& left, const String& right)
    {
        return left.value_ == right.value_;
    }

private:
    std::string value_;
};

} // namespace DeclarativeUI
