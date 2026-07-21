#include "text.hpp"

#include <memory>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

View Text(const char* value)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Text;

    node->text = value != nullptr
        ? value
        : "";

    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    return View(node);
}

} // namespace DeclarativeUI