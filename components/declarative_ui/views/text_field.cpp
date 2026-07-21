#include "text_field.hpp"

#include <cstring>
#include <memory>

#include "core/view_internal.hpp"
#include "platform/platform.hpp"

namespace DeclarativeUI {

View TextField(const Reactive::Binding<String>& value, const char* placeholder)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::TextField;
    node->textField.binding = value;
    node->textField.placeholder = placeholder != nullptr ? placeholder : "";
    node->style.size.fillWidth = true;
    node->style.size.height = 120;
    node->style.hasForegroundColor = true;
    node->style.foregroundColor = Color::White();
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = Color::hex(0x223246);
    node->style.padding.left = 14;
    node->style.padding.right = 14;
    node->style.padding.top = 12;
    node->style.padding.bottom = 12;
    node->style.cornerRadius = 12;
    node->style.hasBorder = true;
    node->style.borderColor = Color::hex(0x475569);
    node->style.borderWidth = 1;
    return View(std::move(node));
}

} // namespace DeclarativeUI
