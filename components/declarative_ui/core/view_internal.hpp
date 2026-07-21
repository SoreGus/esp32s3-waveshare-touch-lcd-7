#pragma once

#include <string>
#include <vector>

#include "view.hpp"

namespace DeclarativeUI {

enum class ViewType {
    Stack,
    Text,
    Spacer,
    Shape,
    Button,
    ScrollView,
    ZStack
};

enum class StackAxis {
    Horizontal,
    Vertical
};

enum class ShapeType {
    Rectangle,
    RoundedRectangle,
    Circle
};

struct EdgeInsets {
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;
};

struct ViewSize {
    int width = LV_SIZE_CONTENT;
    int height = LV_SIZE_CONTENT;

    bool fillWidth = false;
    bool fillHeight = false;
};

struct ViewStyle {
    bool hasForegroundColor = false;
    Color foregroundColor = Color::White();

    bool hasBackgroundColor = false;
    Color backgroundColor = Color::Black();

    EdgeInsets padding;
    ViewSize size;

    int cornerRadius = 0;

    bool hasBorder = false;
    Color borderColor = Color::White();
    int borderWidth = 0;

    uint8_t opacity = LV_OPA_COVER;

    int offsetX = 0;
    int offsetY = 0;

    uint8_t grow = 0;

    bool hidden = false;
    bool scrollable = false;

    TextAlignment textAlignment = TextAlignment::Left;

    const lv_font_t* font = nullptr;
};

struct ViewNode {
    ViewType type = ViewType::Shape;

    StackAxis stackAxis = StackAxis::Vertical;

    ScrollAxis scrollAxis = ScrollAxis::Vertical;

    ShapeType shapeType = ShapeType::Rectangle;

    Alignment alignment = Alignment::Start;
    Justify justify = Justify::Start;

    int spacing = 0;

    std::string text;

    std::vector<View> children;

    std::function<void()> action;

    ViewStyle style;
};

lv_color_t toLVColor(Color color);

lv_flex_align_t toLVAlignment(Alignment alignment);

lv_flex_align_t toLVJustify(Justify justify);

lv_text_align_t toLVTextAlignment(TextAlignment alignment);

void removeDefaultStyle(lv_obj_t* object);

void applyViewStyle(
    lv_obj_t* object,
    const ViewStyle& style
);

void registerButtonAction(
    lv_obj_t* object,
    const std::function<void()>& action
);

} // namespace DeclarativeUI
