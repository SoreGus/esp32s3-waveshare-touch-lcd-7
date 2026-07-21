#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include "lvgl.h"

#include "color.hpp"
#include "string.hpp"
#include "reactive/binding.hpp"

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

class View;
enum class SliderThumb;

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
    View& foregroundColor(const Reactive::Binding<Color>& color);
    View& background(Color color);
    View& background(const Reactive::Binding<Color>& color);

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
    View& opacity(const Reactive::Binding<uint8_t>& value);

    View& offset(
        int x,
        int y
    );

    View& textAlignment(TextAlignment alignment);

    View& font(const lv_font_t* font);

    View& hidden(bool value = true);
    View& hidden(const Reactive::Binding<bool>& value);
    View& scrollable(bool value = true);

    View& modifier(const ViewModifier& modifier);

    View& fill(Color color);
    View& fill(const Reactive::Binding<Color>& color);

    [[nodiscard]] bool valid() const;

    [[nodiscard]] lv_obj_t* mount(lv_obj_t* parent) const;

    View& onTap(std::function<void()> action);

    View& sliderFilledColor(Color color);
    View& sliderEmptyColor(Color color);
    View& sliderThumbColor(Color color);
    View& sliderBarHeight(int height);
    View& sliderThumb(SliderThumb style);
    View& pickerColors(Color background, Color text, Color selected, Color border = Color::hex(0x334155));
    View& toggleColors(Color on, Color off, Color thumb = Color::White());
    View& toggleAnimation(int milliseconds);
    View& segmentedColors(Color background, Color selected, Color text = Color::LightGray(), Color selectedText = Color::White());
    View& segmentedAnimation(int milliseconds);

protected:
    std::shared_ptr<ViewNode> node_;
};

} // namespace DeclarativeUI
