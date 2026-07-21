#include "stack.hpp"

#include <memory>

#include "view_internal.hpp"

namespace DeclarativeUI {

View HStack(std::initializer_list<View> children)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Stack;
    node->stackAxis = StackAxis::Horizontal;

    node->children.assign(
        children.begin(),
        children.end()
    );

    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    return View(node);
}

View VStack(std::initializer_list<View> children)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Stack;
    node->stackAxis = StackAxis::Vertical;

    node->children.assign(
        children.begin(),
        children.end()
    );

    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    return View(node);
}

View Spacer()
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Spacer;
    node->style.grow = 1;

    return View(node);
}

} // namespace DeclarativeUI