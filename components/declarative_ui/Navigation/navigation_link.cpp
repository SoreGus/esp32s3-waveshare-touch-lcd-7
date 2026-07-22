#include "navigation_link.hpp"

#include <memory>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

View NavigationLink(const char* title, View destination)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::NavigationLink;
    node->text = title != nullptr ? title : "";
    node->navigationLink.destination = std::move(destination);
    node->style.hasForegroundColor = true;
    node->style.foregroundColor = Color::White();
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = Color::Blue();
    node->style.padding = {20, 20, 12, 12};
    node->style.cornerRadius = 12;
    return View(std::move(node));
}

View NavigationBackLink(const char* title)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::NavigationLink;
    node->text = title != nullptr ? title : "Back";
    node->navigationLink.destination = View();
    node->style.hasForegroundColor = true;
    node->style.foregroundColor = Color::White();
    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = Color::hex(0x334155);
    node->style.padding = {14, 14, 10, 10};
    node->style.cornerRadius = 10;
    return View(std::move(node));
}

} // namespace DeclarativeUI
