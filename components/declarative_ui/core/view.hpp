#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include "lvgl.h"

namespace DeclarativeUI {

struct ViewNode;

enum class Alignment {
    Start,
    Center,
    End
};

enum class Justify {
    Start,
    Center,
    End,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
};

enum class TextAlignment {
    Left,
    Center,
    Right
};

enum class ScrollAxis {
    Vertical,
    Horizontal,
    Both
};

class Color {
public:
    constexpr explicit Color(uint32_t value)
        : value_(value)
    {
    }

    [[nodiscard]] constexpr uint32_t value() const
    {
        return value_;
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

    static constexpr Color Black()
    {
        return Color(0x000000);
    }

    static constexpr Color White()
    {
        return Color(0xFFFFFF);
    }

    static constexpr Color Red()
    {
        return Color(0xF44336);
    }

    static constexpr Color Green()
    {
        return Color(0x4CAF50);
    }

    static constexpr Color Blue()
    {
        return Color(0x2196F3);
    }

    static constexpr Color Orange()
    {
        return Color(0xFF9800);
    }

    static constexpr Color Yellow()
    {
        return Color(0xFFEB3B);
    }

    static constexpr Color Gray()
    {
        return Color(0x9E9E9E);
    }

    static constexpr Color LightGray()
    {
        return Color(0xD0D0D0);
    }

    static constexpr Color DarkGray()
    {
        return Color(0x303030);
    }

private:
    uint32_t value_;
};

class View;

class ViewModifier {
public:
    virtual ~ViewModifier() = default;

    virtual void apply(View& view) const = 0;
};

class View {
public:
    View();

    explicit View(std::shared_ptr<ViewNode> node);

    View(const View&) = default;
    View(View&&) noexcept = default;

    View& operator=(const View&) = default;
    View& operator=(View&&) noexcept = default;

    virtual ~View() = default;

    View& foregroundColor(Color color);
    View& background(Color color);

    View& padding(int value);

    View& padding(
        int horizontal,
        int vertical
    );

    View& paddingHorizontal(int value);
    View& paddingVertical(int value);

    View& paddingLeft(int value);
    View& paddingRight(int value);
    View& paddingTop(int value);
    View& paddingBottom(int value);

    View& frame(
        int width,
        int height
    );

    View& width(int value);
    View& height(int value);

    View& fill();
    View& fillWidth();
    View& fillHeight();

    View& grow(uint8_t value = 1);

    View& spacing(int value);

    View& alignment(Alignment alignment);
    View& justify(Justify justify);

    View& cornerRadius(int radius);

    View& border(
        Color color,
        int width = 1
    );

    View& opacity(uint8_t value);

    View& offset(
        int x,
        int y
    );

    View& textAlignment(TextAlignment alignment);

    View& font(const lv_font_t* font);

    View& hidden(bool value = true);
    View& scrollable(bool value = true);

    View& modifier(const ViewModifier& modifier);

    View& fill(Color color);

    [[nodiscard]] bool valid() const;

    [[nodiscard]] lv_obj_t* mount(lv_obj_t* parent) const;

    View& onTap(std::function<void()> action);

protected:
    std::shared_ptr<ViewNode> node_;
};

} // namespace DeclarativeUI