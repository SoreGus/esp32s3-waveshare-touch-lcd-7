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
    Slider,
    Picker,
    Toggle,
    SegmentedControl,
    ScrollView,
    ZStack,
    ForEach,
    Image,
    Loading,
    NavigationStack,
    NavigationLink
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

    struct SliderData {
        int minimum = 0;
        int maximum = 100;
        int value = 0;
        Color filledColor = Color::Blue();
        Color emptyColor = Color::DarkGray();
        Color thumbColor = Color::White();
        int barHeight = 8;
        bool pillThumb = false;
        Reactive::Binding<int> binding;
    } slider;

    struct PickerData {
        std::vector<std::string> options;
        int selected = 0;
        Color backgroundColor = Color::hex(0x223246);
        Color textColor = Color::White();
        Color selectedColor = Color::Blue();
        Color borderColor = Color::hex(0x334155);
        Reactive::Binding<int> binding;
    } picker;

    struct ToggleData {
        bool value = false;
        Color onColor = Color::Green();
        Color offColor = Color::DarkGray();
        Color thumbColor = Color::White();
        int animationTime = 180;
        Reactive::Binding<bool> binding;
    } toggle;

    struct SegmentedData {
        std::vector<std::string> options;
        int selected = 0;
        Color backgroundColor = Color::hex(0x223246);
        Color selectedColor = Color::Blue();
        Color textColor = Color::LightGray();
        Color selectedTextColor = Color::White();
        int animationTime = 180;
        Reactive::Binding<int> binding;
    } segmented;

    struct ImageData {
        std::string source;
        const void* localSource = nullptr;
        bool remote = false;
        int storageWidth = 0;
        int storageHeight = 0;
    } image;

    struct NavigationLinkData {
        View destination;
        int animationTime = 260;
    } navigationLink;

    std::vector<std::function<void(lv_obj_t*)>> reactiveBindings;
    std::function<std::vector<View>()> dynamicChildren;
    std::function<Reactive::Subscription(std::function<void()>)> dynamicSubscription;

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

lv_obj_t* mountNavigationStack(lv_obj_t* parent, const ViewNode& node);
lv_obj_t* mountNavigationLink(lv_obj_t* parent, const ViewNode& node);
lv_obj_t* mountImage(lv_obj_t* parent, const ViewNode& node);

} // namespace DeclarativeUI
