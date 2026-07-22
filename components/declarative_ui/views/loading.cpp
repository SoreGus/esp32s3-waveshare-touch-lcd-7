#include "loading.hpp"

#include <memory>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

View Loading()
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Loading;
    node->style.size.width = 32;
    node->style.size.height = 32;
    return View(std::move(node));
}

} // namespace DeclarativeUI
