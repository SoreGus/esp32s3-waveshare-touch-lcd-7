#include "view.hpp"

#include <utility>

#include "platform/platform.hpp"
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

template <typename T>
struct ControlBindingContext {
    Reactive::Subscription subscription;
};

template <typename T>
static void controlBindingDeleteHandler(lv_event_t* event)
{
    delete static_cast<ControlBindingContext<T>*>(lv_event_get_user_data(event));
}

static int clampControlValue(int value, int minimum, int maximum)
{
    return value < minimum ? minimum : (value > maximum ? maximum : value);
}

static void stylePickerList(lv_obj_t* picker, const ViewNode::PickerData& data)
{
    lv_obj_t* list = lv_dropdown_get_list(picker);
    if (list == nullptr) return;
    lv_obj_set_style_bg_color(list, toLVColor(data.backgroundColor), LV_PART_MAIN);
    lv_obj_set_style_text_color(list, toLVColor(data.textColor), LV_PART_MAIN);
    lv_obj_set_style_bg_color(list, toLVColor(data.selectedColor), LV_PART_SELECTED);
    lv_obj_set_style_text_color(list, toLVColor(Color::White()), LV_PART_SELECTED);
    lv_obj_set_style_radius(list, 12, LV_PART_MAIN);
}

struct SegmentedContext {
    lv_obj_t* root = nullptr;
    lv_obj_t* indicator = nullptr;
    std::vector<lv_obj_t*> labels;
    int selected = 0;
    int count = 0;
    int animationTime = 0;
    Color textColor = Color::LightGray();
    Color selectedTextColor = Color::White();
    Reactive::Binding<int> binding;
    Reactive::Subscription subscription;
};

static void layoutSegmented(SegmentedContext* context, bool animated)
{
    if (context == nullptr || context->root == nullptr || context->indicator == nullptr || context->count == 0) return;

    const int width = lv_obj_get_content_width(context->root) / context->count;
    const int height = lv_obj_get_content_height(context->root);
    if (width <= 0 || height <= 0) return;

    const int selected = context->selected < 0 ? 0 : (context->selected >= context->count ? context->count - 1 : context->selected);
    const int targetX = width * selected;
    lv_obj_set_size(context->indicator, width, height);
    for (int index = 0; index < context->count; ++index) {
        lv_obj_set_style_text_color(
            context->labels[index],
            toLVColor(index == selected ? context->selectedTextColor : context->textColor),
            LV_PART_MAIN
        );
    }

    if (!animated || context->animationTime == 0) {
        lv_obj_set_x(context->indicator, targetX);
        return;
    }

    lv_anim_del(context->indicator, nullptr);
    lv_anim_t animation;
    lv_anim_init(&animation);
    lv_anim_set_var(&animation, context->indicator);
    lv_anim_set_values(&animation, lv_obj_get_x(context->indicator), targetX);
    lv_anim_set_time(&animation, context->animationTime);
    lv_anim_set_path_cb(&animation, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&animation, [](void* target, int32_t value) {
        lv_obj_set_x(static_cast<lv_obj_t*>(target), value);
    });
    lv_anim_start(&animation);
}

struct ReactiveBindingContext {
    Reactive::Subscription subscription;
};

static void reactiveBindingDeleteHandler(lv_event_t* event)
{
    delete static_cast<ReactiveBindingContext*>(lv_event_get_user_data(event));
}

template <typename T, typename Apply>
static void registerReactiveBinding(
    lv_obj_t* object,
    const Reactive::Binding<T>& binding,
    Apply apply
)
{
    apply(object, binding.get());

    auto* context = new ReactiveBindingContext();
    context->subscription = binding.subscribe([object, binding, apply] {
        if (!Platform::lock(-1)) {
            return;
        }
        apply(object, binding.get());
        Platform::unlock();
    });

    lv_obj_add_event_cb(
        object,
        reactiveBindingDeleteHandler,
        LV_EVENT_DELETE,
        context
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

View& View::foregroundColor(const Reactive::Binding<Color>& color)
{
    if (node_) {
        node_->reactiveBindings.emplace_back([color](lv_obj_t* object) {
            registerReactiveBinding(object, color, [](lv_obj_t* target, Color value) {
                lv_obj_set_style_text_color(target, toLVColor(value), LV_PART_MAIN);
            });
        });
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

View& View::background(const Reactive::Binding<Color>& color)
{
    if (node_) {
        node_->reactiveBindings.emplace_back([color](lv_obj_t* object) {
            registerReactiveBinding(object, color, [](lv_obj_t* target, Color value) {
                lv_obj_set_style_bg_color(target, toLVColor(value), LV_PART_MAIN);
                lv_obj_set_style_bg_opa(target, LV_OPA_COVER, LV_PART_MAIN);
            });
        });
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

View& View::opacity(const Reactive::Binding<uint8_t>& value)
{
    if (node_) {
        node_->reactiveBindings.emplace_back([value](lv_obj_t* object) {
            registerReactiveBinding(object, value, [](lv_obj_t* target, uint8_t opacity) {
                lv_obj_set_style_opa(target, opacity, LV_PART_MAIN);
            });
        });
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

View& View::hidden(const Reactive::Binding<bool>& value)
{
    if (node_) {
        node_->reactiveBindings.emplace_back([value](lv_obj_t* object) {
            registerReactiveBinding(object, value, [](lv_obj_t* target, bool hidden) {
                if (hidden) lv_obj_add_flag(target, LV_OBJ_FLAG_HIDDEN);
                else lv_obj_clear_flag(target, LV_OBJ_FLAG_HIDDEN);
            });
        });
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

View& View::fill(const Reactive::Binding<Color>& color)
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

        case ViewType::TextField: {
            object = lv_textarea_create(parent);
            const auto& data = node_->textField;
            lv_textarea_set_text(object, data.binding.get().c_str());
            lv_textarea_set_placeholder_text(object, data.placeholder.c_str());
            lv_textarea_set_one_line(object, false);
            lv_textarea_set_cursor_click_pos(object, true);

            struct TextFieldContext {
                Reactive::Binding<String> binding;
                Reactive::Subscription subscription;
                bool applying = false;
            };

            auto* context = new TextFieldContext { .binding = data.binding };
            context->subscription = data.binding.subscribe([object, context] {
                if (!Platform::lock(-1)) return;
                const String value = context->binding.get();
                if (strcmp(lv_textarea_get_text(object), value.c_str()) != 0) {
                    context->applying = true;
                    lv_textarea_set_text(object, value.c_str());
                    context->applying = false;
                }
                Platform::unlock();
            });

            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* context = static_cast<TextFieldContext*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED && !context->applying) {
                    context->binding.set(String(lv_textarea_get_text(lv_event_get_target(event))));
                }
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete context;
            }, LV_EVENT_ALL, context);
            break;
        }

        case ViewType::Slider: {
            object = lv_slider_create(parent);
            removeDefaultStyle(object);
            const auto& data = node_->slider;
            lv_slider_set_range(object, data.minimum, data.maximum);
            lv_slider_set_value(object, clampControlValue(data.value, data.minimum, data.maximum), LV_ANIM_OFF);
            lv_obj_set_style_bg_color(object, toLVColor(data.emptyColor), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_set_style_radius(object, LV_RADIUS_CIRCLE, LV_PART_MAIN);
            lv_obj_set_style_bg_color(object, toLVColor(data.filledColor), LV_PART_INDICATOR);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_INDICATOR);
            lv_obj_set_style_radius(object, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(object, toLVColor(data.thumbColor), LV_PART_KNOB);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_KNOB);
            lv_obj_set_style_radius(object, data.pillThumb ? 8 : LV_RADIUS_CIRCLE, LV_PART_KNOB);
            lv_obj_set_style_width(object, data.pillThumb ? 20 : data.barHeight + 10, LV_PART_KNOB);
            lv_obj_set_style_height(object, data.barHeight + 10, LV_PART_KNOB);

            auto* context = new ControlBindingContext<int>();
            context->subscription = data.binding.subscribe([object, binding = data.binding, data] {
                if (!Platform::lock(-1)) return;
                lv_slider_set_value(object, clampControlValue(binding.get(), data.minimum, data.maximum), LV_ANIM_ON);
                Platform::unlock();
            });
            lv_obj_add_event_cb(object, controlBindingDeleteHandler<int>, LV_EVENT_DELETE, context);
            auto* binding = new Reactive::Binding<int>(data.binding);
            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* input = static_cast<Reactive::Binding<int>*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) input->set(lv_slider_get_value(lv_event_get_target(event)));
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete input;
            }, LV_EVENT_ALL, binding);
            break;
        }

        case ViewType::Picker: {
            object = lv_dropdown_create(parent);
            const auto& data = node_->picker;
            std::string options;
            for (size_t index = 0; index < data.options.size(); ++index) {
                if (index > 0) options += '\n';
                options += data.options[index];
            }
            lv_dropdown_set_options(object, options.c_str());
            const int maximum = data.options.empty() ? 0 : static_cast<int>(data.options.size() - 1);
            lv_dropdown_set_selected(object, clampControlValue(data.selected, 0, maximum));
            lv_dropdown_set_selected_highlight(object, true);
            lv_obj_set_style_bg_color(object, toLVColor(data.backgroundColor), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_set_style_text_color(object, toLVColor(data.textColor), LV_PART_MAIN);
            lv_obj_set_style_border_color(object, toLVColor(data.borderColor), LV_PART_MAIN);
            lv_obj_set_style_border_width(object, 1, LV_PART_MAIN);
            lv_obj_set_style_pad_left(object, 14, LV_PART_MAIN);
            lv_obj_set_style_pad_right(object, 14, LV_PART_MAIN);
            lv_obj_set_style_radius(object, 12, LV_PART_MAIN);
            stylePickerList(object, data);
            auto* context = new ControlBindingContext<int>();
            context->subscription = data.binding.subscribe([object, binding = data.binding, maximum] {
                if (!Platform::lock(-1)) return;
                lv_dropdown_set_selected(object, clampControlValue(binding.get(), 0, maximum));
                Platform::unlock();
            });
            lv_obj_add_event_cb(object, controlBindingDeleteHandler<int>, LV_EVENT_DELETE, context);
            auto* binding = new Reactive::Binding<int>(data.binding);
            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* input = static_cast<Reactive::Binding<int>*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) input->set(lv_dropdown_get_selected(lv_event_get_target(event)));
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete input;
            }, LV_EVENT_ALL, binding);
            auto* pickerData = new ViewNode::PickerData(data);
            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* style = static_cast<ViewNode::PickerData*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_CLICKED) stylePickerList(lv_event_get_target(event), *style);
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete style;
            }, LV_EVENT_ALL, pickerData);
            break;
        }

        case ViewType::Toggle: {
            object = lv_switch_create(parent);
            const auto& data = node_->toggle;
            if (data.value) lv_obj_add_state(object, LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(object, toLVColor(data.offColor), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_set_style_radius(object, LV_RADIUS_CIRCLE, LV_PART_MAIN);
            lv_obj_set_style_bg_color(object, toLVColor(data.onColor), LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_radius(object, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(object, toLVColor(data.thumbColor), LV_PART_KNOB);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_KNOB);
            lv_obj_set_style_radius(object, LV_RADIUS_CIRCLE, LV_PART_KNOB);
            lv_obj_set_style_anim_time(object, data.animationTime, LV_PART_MAIN);
            auto* context = new ControlBindingContext<bool>();
            context->subscription = data.binding.subscribe([object, binding = data.binding] {
                if (!Platform::lock(-1)) return;
                if (binding.get()) lv_obj_add_state(object, LV_STATE_CHECKED); else lv_obj_clear_state(object, LV_STATE_CHECKED);
                Platform::unlock();
            });
            lv_obj_add_event_cb(object, controlBindingDeleteHandler<bool>, LV_EVENT_DELETE, context);
            auto* binding = new Reactive::Binding<bool>(data.binding);
            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* input = static_cast<Reactive::Binding<bool>*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_VALUE_CHANGED) input->set(lv_obj_has_state(lv_event_get_target(event), LV_STATE_CHECKED));
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete input;
            }, LV_EVENT_ALL, binding);
            break;
        }

        case ViewType::SegmentedControl: {
            object = lv_obj_create(parent);
            removeDefaultStyle(object);
            const auto& data = node_->segmented;
            const int count = static_cast<int>(data.options.size());
            auto* context = new SegmentedContext();
            context->root = object;
            context->count = count;
            context->selected = clampControlValue(data.selected, 0, count > 0 ? count - 1 : 0);
            context->animationTime = data.animationTime;
            context->binding = data.binding;
            context->textColor = data.textColor;
            context->selectedTextColor = data.selectedTextColor;

            lv_obj_set_flex_flow(object, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(object, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_set_style_bg_color(object, toLVColor(data.backgroundColor), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_set_style_radius(object, 14, LV_PART_MAIN);
            lv_obj_set_style_pad_all(object, 4, LV_PART_MAIN);

            context->indicator = lv_obj_create(object);
            removeDefaultStyle(context->indicator);
            lv_obj_add_flag(context->indicator, LV_OBJ_FLAG_IGNORE_LAYOUT);
            lv_obj_set_style_bg_color(context->indicator, toLVColor(data.selectedColor), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(context->indicator, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_set_style_radius(context->indicator, 10, LV_PART_MAIN);

            for (int index = 0; index < count; ++index) {
                lv_obj_t* item = lv_obj_create(object);
                removeDefaultStyle(item);
                lv_obj_set_flex_grow(item, 1);
                lv_obj_set_height(item, lv_pct(100));
                lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_bg_opa(item, LV_OPA_TRANSP, LV_PART_MAIN);
                lv_obj_set_style_radius(item, 10, LV_PART_MAIN);

                lv_obj_t* label = lv_label_create(item);
                lv_label_set_text(label, data.options[index].c_str());
                lv_obj_set_style_text_color(label, toLVColor(data.textColor), LV_PART_MAIN);
                lv_obj_center(label);
                context->labels.push_back(label);

                struct TapContext { SegmentedContext* segmented; int index; };
                auto* tap = new TapContext { context, index };
                lv_obj_add_event_cb(item, [](lv_event_t* event) {
                    auto* target = static_cast<TapContext*>(lv_event_get_user_data(event));
                    if (lv_event_get_code(event) == LV_EVENT_CLICKED) target->segmented->binding.set(target->index);
                    if (lv_event_get_code(event) == LV_EVENT_DELETE) delete target;
                }, LV_EVENT_ALL, tap);
            }

            context->subscription = data.binding.subscribe([context, binding = data.binding] {
                if (!Platform::lock(-1)) return;
                context->selected = binding.get();
                layoutSegmented(context, true);
                Platform::unlock();
            });
            lv_obj_add_event_cb(object, [](lv_event_t* event) {
                auto* context = static_cast<SegmentedContext*>(lv_event_get_user_data(event));
                if (lv_event_get_code(event) == LV_EVENT_SIZE_CHANGED) layoutSegmented(context, false);
                if (lv_event_get_code(event) == LV_EVENT_DELETE) delete context;
            }, LV_EVENT_ALL, context);
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

        case ViewType::ForEach: {
            object = lv_obj_create(parent);
            removeDefaultStyle(object);
            lv_obj_set_flex_flow(object, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_flex_align(
                object,
                toLVJustify(node_->justify),
                toLVAlignment(node_->alignment),
                LV_FLEX_ALIGN_START
            );
            lv_obj_set_style_pad_row(object, node_->spacing, LV_PART_MAIN);
            lv_obj_set_width(object, lv_pct(100));
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

    // The generic view style owns the outer frame. Reapply the control parts
    // afterwards so specialized colors and dimensions remain authoritative.
    if (node_->type == ViewType::Slider) {
        const auto& data = node_->slider;
        lv_obj_set_style_bg_color(object, toLVColor(data.emptyColor), LV_PART_MAIN);
        lv_obj_set_style_bg_color(object, toLVColor(data.filledColor), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(object, toLVColor(data.thumbColor), LV_PART_KNOB);
        lv_obj_set_style_width(object, data.pillThumb ? 20 : data.barHeight + 10, LV_PART_KNOB);
        lv_obj_set_style_height(object, data.barHeight + 10, LV_PART_KNOB);
        lv_obj_set_style_radius(object, data.pillThumb ? 8 : LV_RADIUS_CIRCLE, LV_PART_KNOB);
    } else if (node_->type == ViewType::Picker) {
        const auto& data = node_->picker;
        lv_obj_set_style_bg_color(object, toLVColor(data.backgroundColor), LV_PART_MAIN);
        lv_obj_set_style_text_color(object, toLVColor(data.textColor), LV_PART_MAIN);
        lv_obj_set_style_border_color(object, toLVColor(data.borderColor), LV_PART_MAIN);
        stylePickerList(object, data);
    } else if (node_->type == ViewType::Toggle) {
        const auto& data = node_->toggle;
        lv_obj_set_style_bg_color(object, toLVColor(data.offColor), LV_PART_MAIN);
        lv_obj_set_style_bg_color(object, toLVColor(data.onColor), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_set_style_bg_color(object, toLVColor(data.thumbColor), LV_PART_KNOB);
        lv_obj_set_style_anim_time(object, data.animationTime, LV_PART_MAIN);
    } else if (node_->type == ViewType::SegmentedControl) {
        const auto& data = node_->segmented;
        lv_obj_set_style_bg_color(object, toLVColor(data.backgroundColor), LV_PART_MAIN);
        lv_obj_set_style_radius(object, 14, LV_PART_MAIN);
        lv_obj_set_style_pad_all(object, 4, LV_PART_MAIN);
    }

    if (node_->type == ViewType::Button) {
        applyButtonPressedStyle(
            object,
            node_->style
        );
    }

    for (const auto& bind : node_->reactiveBindings) {
        bind(object);
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

    const std::vector<View> dynamicChildren = node_->dynamicChildren
        ? node_->dynamicChildren()
        : std::vector<View>();
    const std::vector<View>& children = node_->dynamicChildren
        ? dynamicChildren
        : node_->children;

    for (const View& child : children) {
        [[maybe_unused]] lv_obj_t* childObject = child.mount(object);
    }

    if (node_->dynamicSubscription) {
        struct DynamicContext {
            Reactive::Subscription subscription;
        };

        const std::shared_ptr<ViewNode> node = node_;
        auto* context = new DynamicContext();
        context->subscription = node_->dynamicSubscription([object, node] {
            if (!Platform::lock(-1)) return;
            lv_obj_clean(object);
            for (const View& child : node->dynamicChildren()) {
                [[maybe_unused]] lv_obj_t* childObject = child.mount(object);
            }
            lv_obj_update_layout(object);
            for (lv_obj_t* ancestor = lv_obj_get_parent(object);
                 ancestor != nullptr;
                 ancestor = lv_obj_get_parent(ancestor)) {
                lv_obj_update_layout(ancestor);
            }
            Platform::unlock();
        });
        lv_obj_add_event_cb(object, [](lv_event_t* event) {
            delete static_cast<DynamicContext*>(lv_event_get_user_data(event));
        }, LV_EVENT_DELETE, context);
    }

    return object;
}

} // namespace DeclarativeUI
