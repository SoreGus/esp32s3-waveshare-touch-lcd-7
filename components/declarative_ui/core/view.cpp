#include "view.hpp"

#include <utility>

#include "view_internal.hpp"

namespace DeclarativeUI {

lv_color_t toLVColor(Color color)
{
    return lv_color_hex(color.value());
}

lv_flex_align_t toLVAlignment(Alignment alignment)
{
    switch (alignment) {
        case Alignment::Start:
            return LV_FLEX_ALIGN_START;

        case Alignment::Center:
            return LV_FLEX_ALIGN_CENTER;

        case Alignment::End:
            return LV_FLEX_ALIGN_END;
    }

    return LV_FLEX_ALIGN_START;
}

lv_flex_align_t toLVJustify(Justify justify)
{
    switch (justify) {
        case Justify::Start:
            return LV_FLEX_ALIGN_START;

        case Justify::Center:
            return LV_FLEX_ALIGN_CENTER;

        case Justify::End:
            return LV_FLEX_ALIGN_END;

        case Justify::SpaceBetween:
            return LV_FLEX_ALIGN_SPACE_BETWEEN;

        case Justify::SpaceAround:
            return LV_FLEX_ALIGN_SPACE_AROUND;

        case Justify::SpaceEvenly:
            return LV_FLEX_ALIGN_SPACE_EVENLY;
    }

    return LV_FLEX_ALIGN_START;
}

lv_text_align_t toLVTextAlignment(TextAlignment alignment)
{
    switch (alignment) {
        case TextAlignment::Left:
            return LV_TEXT_ALIGN_LEFT;

        case TextAlignment::Center:
            return LV_TEXT_ALIGN_CENTER;

        case TextAlignment::Right:
            return LV_TEXT_ALIGN_RIGHT;
    }

    return LV_TEXT_ALIGN_LEFT;
}

void removeDefaultStyle(lv_obj_t* object)
{
    lv_obj_remove_style_all(object);

    lv_obj_set_style_bg_opa(
        object,
        LV_OPA_TRANSP,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        object,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_outline_width(
        object,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_shadow_width(
        object,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_all(
        object,
        0,
        LV_PART_MAIN
    );
}

static void applySize(
    lv_obj_t* object,
    const ViewSize& size
)
{
    if (size.fillWidth) {
        lv_obj_set_width(
            object,
            lv_pct(100)
        );
    } else {
        lv_obj_set_width(
            object,
            size.width
        );
    }

    if (size.fillHeight) {
        lv_obj_set_height(
            object,
            lv_pct(100)
        );
    } else {
        lv_obj_set_height(
            object,
            size.height
        );
    }
}

static void applyPadding(
    lv_obj_t* object,
    const EdgeInsets& padding
)
{
    lv_obj_set_style_pad_left(
        object,
        padding.left,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_right(
        object,
        padding.right,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_top(
        object,
        padding.top,
        LV_PART_MAIN
    );

    lv_obj_set_style_pad_bottom(
        object,
        padding.bottom,
        LV_PART_MAIN
    );
}

static Color pressedColor(Color color)
{
    return color.darken(18);
}

static void applyButtonPressedStyle(
    lv_obj_t* object,
    const ViewStyle& style
)
{
    const Color normalColor = style.hasBackgroundColor
        ? style.backgroundColor
        : Color::Blue();

    lv_obj_set_style_bg_color(
        object,
        toLVColor(pressedColor(normalColor)),
        static_cast<lv_style_selector_t>(LV_PART_MAIN) |
            static_cast<lv_style_selector_t>(LV_STATE_PRESSED)
    );

    lv_obj_set_style_bg_opa(
        object,
        LV_OPA_COVER,
        static_cast<lv_style_selector_t>(LV_PART_MAIN) |
            static_cast<lv_style_selector_t>(LV_STATE_PRESSED)
    );
}

void applyViewStyle(
    lv_obj_t* object,
    const ViewStyle& style
)
{
    applySize(
        object,
        style.size
    );

    applyPadding(
        object,
        style.padding
    );

    if (style.hasForegroundColor) {
        lv_obj_set_style_text_color(
            object,
            toLVColor(style.foregroundColor),
            LV_PART_MAIN
        );
    }

    if (style.hasBackgroundColor) {
        lv_obj_set_style_bg_color(
            object,
            toLVColor(style.backgroundColor),
            LV_PART_MAIN
        );

        lv_obj_set_style_bg_opa(
            object,
            LV_OPA_COVER,
            LV_PART_MAIN
        );
    } else {
        lv_obj_set_style_bg_opa(
            object,
            LV_OPA_TRANSP,
            LV_PART_MAIN
        );
    }

    lv_obj_set_style_radius(
        object,
        style.cornerRadius,
        LV_PART_MAIN
    );

    if (style.hasBorder && style.borderWidth > 0) {
        lv_obj_set_style_border_color(
            object,
            toLVColor(style.borderColor),
            LV_PART_MAIN
        );

        lv_obj_set_style_border_width(
            object,
            style.borderWidth,
            LV_PART_MAIN
        );

        lv_obj_set_style_border_opa(
            object,
            LV_OPA_COVER,
            LV_PART_MAIN
        );
    } else {
        lv_obj_set_style_border_width(
            object,
            0,
            LV_PART_MAIN
        );
    }

    lv_obj_set_style_opa(
        object,
        style.opacity,
        LV_PART_MAIN
    );

    lv_obj_set_style_translate_x(
        object,
        style.offsetX,
        LV_PART_MAIN
    );

    lv_obj_set_style_translate_y(
        object,
        style.offsetY,
        LV_PART_MAIN
    );

    lv_obj_set_style_text_align(
        object,
        toLVTextAlignment(style.textAlignment),
        LV_PART_MAIN
    );

    if (style.font != nullptr) {
        lv_obj_set_style_text_font(
            object,
            style.font,
            LV_PART_MAIN
        );
    }

    if (style.grow > 0) {
        lv_obj_set_flex_grow(
            object,
            style.grow
        );
    }

    if (style.hidden) {
        lv_obj_add_flag(
            object,
            LV_OBJ_FLAG_HIDDEN
        );
    } else {
        lv_obj_clear_flag(
            object,
            LV_OBJ_FLAG_HIDDEN
        );
    }

    if (style.scrollable) {
        lv_obj_add_flag(
            object,
            LV_OBJ_FLAG_SCROLLABLE
        );
    } else {
        lv_obj_clear_flag(
            object,
            LV_OBJ_FLAG_SCROLLABLE
        );
    }
}

View::View() = default;

View::View(std::shared_ptr<ViewNode> node)
    : node_(std::move(node))
{
}

View& View::foregroundColor(Color color)
{
    if (node_) {
        node_->style.hasForegroundColor = true;
        node_->style.foregroundColor = color;
    }

    return *this;
}

View& View::background(Color color)
{
    if (node_) {
        node_->style.hasBackgroundColor = true;
        node_->style.backgroundColor = color;
    }

    return *this;
}

View& View::padding(int value)
{
    if (node_) {
        node_->style.padding.left = value;
        node_->style.padding.right = value;
        node_->style.padding.top = value;
        node_->style.padding.bottom = value;
    }

    return *this;
}

View& View::padding(
    int horizontal,
    int vertical
)
{
    if (node_) {
        node_->style.padding.left = horizontal;
        node_->style.padding.right = horizontal;
        node_->style.padding.top = vertical;
        node_->style.padding.bottom = vertical;
    }

    return *this;
}

View& View::paddingHorizontal(int value)
{
    if (node_) {
        node_->style.padding.left = value;
        node_->style.padding.right = value;
    }

    return *this;
}

View& View::paddingVertical(int value)
{
    if (node_) {
        node_->style.padding.top = value;
        node_->style.padding.bottom = value;
    }

    return *this;
}

View& View::paddingLeft(int value)
{
    if (node_) {
        node_->style.padding.left = value;
    }

    return *this;
}

View& View::paddingRight(int value)
{
    if (node_) {
        node_->style.padding.right = value;
    }

    return *this;
}

View& View::paddingTop(int value)
{
    if (node_) {
        node_->style.padding.top = value;
    }

    return *this;
}

View& View::paddingBottom(int value)
{
    if (node_) {
        node_->style.padding.bottom = value;
    }

    return *this;
}

View& View::frame(
    int width,
    int height
)
{
    if (node_) {
        node_->style.size.width = width;
        node_->style.size.height = height;
    }

    return *this;
}

View& View::width(int value)
{
    if (node_) {
        node_->style.size.width = value;
    }

    return *this;
}

View& View::height(int value)
{
    if (node_) {
        node_->style.size.height = value;
    }

    return *this;
}

View& View::fill()
{
    if (node_) {
        node_->style.size.fillWidth = true;
        node_->style.size.fillHeight = true;
    }

    return *this;
}

View& View::fillWidth()
{
    if (node_) {
        node_->style.size.fillWidth = true;
    }

    return *this;
}

View& View::fillHeight()
{
    if (node_) {
        node_->style.size.fillHeight = true;
    }

    return *this;
}

View& View::grow(uint8_t value)
{
    if (node_) {
        node_->style.grow = value;
    }

    return *this;
}

View& View::spacing(int value)
{
    if (node_) {
        node_->spacing = value;
    }

    return *this;
}

View& View::alignment(Alignment alignment)
{
    if (node_) {
        node_->alignment = alignment;
    }

    return *this;
}

View& View::justify(Justify justify)
{
    if (node_) {
        node_->justify = justify;
    }

    return *this;
}

View& View::cornerRadius(int radius)
{
    if (node_) {
        node_->style.cornerRadius = radius;
    }

    return *this;
}

View& View::border(
    Color color,
    int width
)
{
    if (node_) {
        node_->style.hasBorder = true;
        node_->style.borderColor = color;
        node_->style.borderWidth = width;
    }

    return *this;
}

View& View::opacity(uint8_t value)
{
    if (node_) {
        node_->style.opacity = value;
    }

    return *this;
}

View& View::offset(
    int x,
    int y
)
{
    if (node_) {
        node_->style.offsetX = x;
        node_->style.offsetY = y;
    }

    return *this;
}

View& View::textAlignment(TextAlignment alignment)
{
    if (node_) {
        node_->style.textAlignment = alignment;
    }

    return *this;
}

View& View::font(const lv_font_t* font)
{
    if (node_) {
        node_->style.font = font;
    }

    return *this;
}

View& View::hidden(bool value)
{
    if (node_) {
        node_->style.hidden = value;
    }

    return *this;
}

View& View::scrollable(bool value)
{
    if (node_) {
        node_->style.scrollable = value;
    }

    return *this;
}

View& View::modifier(const ViewModifier& modifier)
{
    modifier.apply(*this);
    return *this;
}

View& View::fill(Color color)
{
    return background(color);
}

bool View::valid() const
{
    return node_ != nullptr;
}

lv_obj_t* View::mount(lv_obj_t* parent) const
{
    if (!node_ || parent == nullptr) {
        return nullptr;
    }

    lv_obj_t* object = nullptr;

    switch (node_->type) {
        case ViewType::Text: {
            object = lv_label_create(parent);

            lv_label_set_text(
                object,
                node_->text.c_str()
            );

            lv_label_set_long_mode(
                object,
                LV_LABEL_LONG_WRAP
            );

            break;
        }

        case ViewType::Stack: {
            object = lv_obj_create(parent);

            removeDefaultStyle(object);

            lv_obj_set_flex_flow(
                object,
                node_->stackAxis == StackAxis::Horizontal
                    ? LV_FLEX_FLOW_ROW
                    : LV_FLEX_FLOW_COLUMN
            );

            lv_obj_set_flex_align(
                object,
                toLVJustify(node_->justify),
                toLVAlignment(node_->alignment),
                LV_FLEX_ALIGN_START
            );

            if (node_->stackAxis == StackAxis::Horizontal) {
                lv_obj_set_style_pad_column(
                    object,
                    node_->spacing,
                    LV_PART_MAIN
                );
            } else {
                lv_obj_set_style_pad_row(
                    object,
                    node_->spacing,
                    LV_PART_MAIN
                );
            }

            break;
        }

        case ViewType::Spacer: {
            object = lv_obj_create(parent);

            removeDefaultStyle(object);

            lv_obj_set_size(
                object,
                1,
                1
            );

            lv_obj_set_flex_grow(
                object,
                1
            );

            break;
        }

        case ViewType::Shape: {
            object = lv_obj_create(parent);

            removeDefaultStyle(object);

            break;
        }

        case ViewType::Button: {
            object = lv_btn_create(parent);

            removeDefaultStyle(object);

            lv_obj_t* label = lv_label_create(object);

            lv_label_set_text(
                label,
                node_->text.c_str()
            );

            lv_obj_center(label);

            registerButtonAction(
                object,
                node_->action
            );

            break;
        }
        
        case ViewType::ScrollView: {
            object = lv_obj_create(parent);

            removeDefaultStyle(object);

            lv_obj_add_flag(
                object,
                LV_OBJ_FLAG_SCROLLABLE
            );

            lv_obj_add_flag(
                object,
                LV_OBJ_FLAG_SCROLL_MOMENTUM
            );

            lv_obj_add_flag(
                object,
                LV_OBJ_FLAG_SCROLL_ELASTIC
            );

            lv_obj_set_scrollbar_mode(
                object,
                LV_SCROLLBAR_MODE_AUTO
            );

            switch (node_->scrollAxis) {
                case ScrollAxis::Vertical: {
                    lv_obj_set_scroll_dir(
                        object,
                        LV_DIR_VER
                    );

                    lv_obj_set_flex_flow(
                        object,
                        LV_FLEX_FLOW_COLUMN
                    );

                    lv_obj_add_flag(
                        object,
                        LV_OBJ_FLAG_SCROLL_CHAIN_VER
                    );

                    lv_obj_set_style_pad_row(
                        object,
                        node_->spacing,
                        LV_PART_MAIN
                    );

                    break;
                }

                case ScrollAxis::Horizontal: {
                    lv_obj_set_scroll_dir(
                        object,
                        LV_DIR_HOR
                    );

                    lv_obj_set_flex_flow(
                        object,
                        LV_FLEX_FLOW_ROW
                    );

                    lv_obj_add_flag(
                        object,
                        LV_OBJ_FLAG_SCROLL_CHAIN_HOR
                    );

                    lv_obj_set_style_pad_column(
                        object,
                        node_->spacing,
                        LV_PART_MAIN
                    );

                    break;
                }

                case ScrollAxis::Both: {
                    lv_obj_set_scroll_dir(
                        object,
                        LV_DIR_ALL
                    );

                    lv_obj_set_flex_flow(
                        object,
                        LV_FLEX_FLOW_ROW_WRAP
                    );

                    lv_obj_add_flag(
                        object,
                        LV_OBJ_FLAG_SCROLL_CHAIN_VER
                    );

                    lv_obj_add_flag(
                        object,
                        LV_OBJ_FLAG_SCROLL_CHAIN_HOR
                    );

                    lv_obj_set_style_pad_row(
                        object,
                        node_->spacing,
                        LV_PART_MAIN
                    );

                    lv_obj_set_style_pad_column(
                        object,
                        node_->spacing,
                        LV_PART_MAIN
                    );

                    break;
                }
            }

            lv_obj_set_flex_align(
                object,
                toLVJustify(node_->justify),
                toLVAlignment(node_->alignment),
                LV_FLEX_ALIGN_START
            );

            break;
        }

        case ViewType::ZStack: {
            object = lv_obj_create(parent);

            removeDefaultStyle(object);

            break;
        }
    }

    if (object == nullptr) {
        return nullptr;
    }

    applyViewStyle(
        object,
        node_->style
    );

    if (node_->type == ViewType::Button) {
        applyButtonPressedStyle(
            object,
            node_->style
        );
    }

    if (
        node_->type == ViewType::Shape &&
        node_->shapeType == ShapeType::Circle
    ) {
        lv_obj_set_style_radius(
            object,
            LV_RADIUS_CIRCLE,
            LV_PART_MAIN
        );
    }

    for (const View& child : node_->children) {
        [[maybe_unused]] lv_obj_t* childObject = child.mount(object);
    }

    return object;
}

} // namespace DeclarativeUI
