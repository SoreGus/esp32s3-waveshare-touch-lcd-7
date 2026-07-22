#include "controls.hpp"

#include <algorithm>
#include <memory>
#include <utility>

#include "core/view_internal.hpp"
#include "platform/platform.hpp"

namespace DeclarativeUI {

namespace {

int clamp(int value, int minimum, int maximum)
{
    return std::max(minimum, std::min(value, maximum));
}

std::vector<std::string> copyOptions(std::initializer_list<const char*> values)
{
    std::vector<std::string> result;
    result.reserve(values.size());
    for (const char* value : values) result.emplace_back(value != nullptr ? value : "");
    return result;
}

std::vector<std::string> copyOptions(const Array<String>& values)
{
    std::vector<std::string> result;
    result.reserve(values.size());
    for (const String& value : values) result.emplace_back(value.c_str());
    return result;
}

} // namespace

View Slider(const Reactive::Binding<int>& value, int minimum, int maximum)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Slider;
    node->slider.minimum = std::min(minimum, maximum);
    node->slider.maximum = std::max(minimum, maximum);
    node->slider.value = clamp(value.get(), node->slider.minimum, node->slider.maximum);
    node->slider.binding = value;
    node->style.size.width = lv_pct(100);
    node->style.size.height = 28;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->slider.emptyColor;
    node->style.cornerRadius = LV_RADIUS_CIRCLE;
    return View(std::move(node));
}

View Picker(std::initializer_list<const char*> options, const Reactive::Binding<int>& selected)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Picker;
    node->picker.options = copyOptions(options);
    node->picker.selected = selected.get();
    node->picker.binding = selected;
    node->style.size.width = lv_pct(100);
    node->style.size.height = 46;
    node->style.cornerRadius = 12;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->picker.backgroundColor;
    return View(std::move(node));
}

View Picker(const Array<String>& options, const Reactive::Binding<int>& selected)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Picker;
    node->picker.options = copyOptions(options);
    node->picker.selected = selected.get();
    node->picker.binding = selected;
    node->style.size.width = lv_pct(100);
    node->style.size.height = 46;
    node->style.cornerRadius = 12;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->picker.backgroundColor;
    return View(std::move(node));
}

View Toggle(const Reactive::Binding<bool>& value)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Toggle;
    node->toggle.value = value.get();
    node->toggle.binding = value;
    node->style.size.width = 58;
    node->style.size.height = 32;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->toggle.offColor;
    node->style.cornerRadius = LV_RADIUS_CIRCLE;
    return View(std::move(node));
}

View SegmentedControl(std::initializer_list<const char*> options, const Reactive::Binding<int>& selected)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::SegmentedControl;
    node->segmented.options = copyOptions(options);
    node->segmented.selected = selected.get();
    node->segmented.binding = selected;
    node->style.size.width = lv_pct(100);
    node->style.size.height = 48;
    node->style.cornerRadius = 14;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->segmented.backgroundColor;
    return View(std::move(node));
}

View SegmentedControl(const Array<String>& options, const Reactive::Binding<int>& selected)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::SegmentedControl;
    node->segmented.options = copyOptions(options);
    node->segmented.selected = selected.get();
    node->segmented.binding = selected;
    node->style.size.width = lv_pct(100);
    node->style.size.height = 48;
    node->style.cornerRadius = 14;
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = node->segmented.backgroundColor;
    return View(std::move(node));
}

View& View::sliderFilledColor(Color color) { if (node_) node_->slider.filledColor = color; return *this; }
View& View::sliderEmptyColor(Color color) { if (node_) { node_->slider.emptyColor = color; node_->style.hasBackgroundColor = true; node_->style.backgroundColor = color; } return *this; }
View& View::sliderThumbColor(Color color) { if (node_) node_->slider.thumbColor = color; return *this; }
View& View::sliderBarHeight(int height)
{
    if (node_) {
        node_->slider.barHeight = std::max(2, height);
        node_->style.size.height = std::max(24, node_->slider.barHeight + 20);
    }
    return *this;
}
View& View::sliderThumb(SliderThumb style) { if (node_) node_->slider.pillThumb = style == SliderThumb::Pill; return *this; }

View& View::pickerColors(Color background, Color text, Color selected, Color border)
{
    if (node_) { node_->picker = {node_->picker.options, node_->picker.selected, background, text, selected, border, node_->picker.binding}; node_->style.hasBackgroundColor = true; node_->style.backgroundColor = background; }
    return *this;
}

View& View::toggleColors(Color on, Color off, Color thumb)
{
    if (node_) { node_->toggle.onColor = on; node_->toggle.offColor = off; node_->toggle.thumbColor = thumb; node_->style.hasBackgroundColor = true; node_->style.backgroundColor = off; }
    return *this;
}

View& View::toggleAnimation(int milliseconds) { if (node_) node_->toggle.animationTime = std::max(0, milliseconds); return *this; }

View& View::segmentedColors(Color background, Color selected, Color text, Color selectedText)
{
    if (node_) { node_->segmented.backgroundColor = background; node_->segmented.selectedColor = selected; node_->segmented.textColor = text; node_->segmented.selectedTextColor = selectedText; node_->style.hasBackgroundColor = true; node_->style.backgroundColor = background; }
    return *this;
}

View& View::segmentedAnimation(int milliseconds) { if (node_) node_->segmented.animationTime = std::max(0, milliseconds); return *this; }

View& View::imageStorageSize(int maxWidth, int maxHeight)
{
    if (node_ && node_->type == ViewType::Image) {
        node_->image.storageWidth = std::max(0, maxWidth);
        node_->image.storageHeight = std::max(0, maxHeight);
    }
    return *this;
}

} // namespace DeclarativeUI
