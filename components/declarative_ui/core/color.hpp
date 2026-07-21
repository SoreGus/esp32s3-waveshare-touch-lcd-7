#pragma once

#include <cstdint>

namespace DeclarativeUI {

class Color {
public:
    constexpr explicit Color(uint32_t value)
        : value_(value & 0xFFFFFFU)
    {
    }

    [[nodiscard]] constexpr uint32_t value() const
    {
        return value_;
    }

    [[nodiscard]] constexpr uint8_t red() const
    {
        return static_cast<uint8_t>(value_ >> 16U);
    }

    [[nodiscard]] constexpr uint8_t green() const
    {
        return static_cast<uint8_t>(value_ >> 8U);
    }

    [[nodiscard]] constexpr uint8_t blue() const
    {
        return static_cast<uint8_t>(value_);
    }

    static constexpr Color hex(uint32_t value)
    {
        return Color(value);
    }

    static constexpr Color rgb(
        uint8_t red,
        uint8_t green,
        uint8_t blue
    )
    {
        return Color(
            (static_cast<uint32_t>(red) << 16U) |
            (static_cast<uint32_t>(green) << 8U) |
            static_cast<uint32_t>(blue)
        );
    }

    [[nodiscard]] constexpr Color lighten(uint8_t percentage) const
    {
        return Color::rgb(
            lightenChannel(red(), percentage),
            lightenChannel(green(), percentage),
            lightenChannel(blue(), percentage)
        );
    }

    [[nodiscard]] constexpr Color darken(uint8_t percentage) const
    {
        return Color::rgb(
            darkenChannel(red(), percentage),
            darkenChannel(green(), percentage),
            darkenChannel(blue(), percentage)
        );
    }

    static constexpr Color Black() { return hex(0x000000); }
    static constexpr Color White() { return hex(0xFFFFFF); }
    static constexpr Color Red() { return hex(0xF44336); }
    static constexpr Color Pink() { return hex(0xE91E63); }
    static constexpr Color Purple() { return hex(0x9C27B0); }
    static constexpr Color Indigo() { return hex(0x3F51B5); }
    static constexpr Color Blue() { return hex(0x2196F3); }
    static constexpr Color Cyan() { return hex(0x00BCD4); }
    static constexpr Color Teal() { return hex(0x009688); }
    static constexpr Color Green() { return hex(0x4CAF50); }
    static constexpr Color Lime() { return hex(0xCDDC39); }
    static constexpr Color Yellow() { return hex(0xFFEB3B); }
    static constexpr Color Orange() { return hex(0xFF9800); }
    static constexpr Color Brown() { return hex(0x795548); }
    static constexpr Color Gray() { return hex(0x9E9E9E); }
    static constexpr Color LightGray() { return hex(0xD0D0D0); }
    static constexpr Color DarkGray() { return hex(0x303030); }

private:
    static constexpr uint8_t lightenChannel(
        uint8_t channel,
        uint8_t percentage
    )
    {
        const uint8_t clampedPercentage = percentage > 100 ? 100 : percentage;
        const uint16_t remaining = 255U - channel;

        return static_cast<uint8_t>(
            channel + ((remaining * clampedPercentage) / 100U)
        );
    }

    static constexpr uint8_t darkenChannel(
        uint8_t channel,
        uint8_t percentage
    )
    {
        const uint8_t clampedPercentage = percentage > 100 ? 100 : percentage;

        return static_cast<uint8_t>(
            (static_cast<uint16_t>(channel) * (100U - clampedPercentage)) / 100U
        );
    }

    uint32_t value_;
};

} // namespace DeclarativeUI
