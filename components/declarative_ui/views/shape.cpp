#include "shape.hpp"

#include <memory>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

View Rectangle()
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Shape;
    node->shapeType = ShapeType::Rectangle;

    node->style.size.width = 80;
    node->style.size.height = 40;

    return View(node);
}

View RoundedRectangle(int radius)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Shape;
    node->shapeType = ShapeType::RoundedRectangle;

    node->style.size.width = 80;
    node->style.size.height = 40;
    node->style.cornerRadius = radius;

    return View(node);
}

View Circle()
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Shape;
    node->shapeType = ShapeType::Circle;

    node->style.size.width = 40;
    node->style.size.height = 40;
    node->style.cornerRadius = LV_RADIUS_CIRCLE;

    return View(node);
}

} // namespace DeclarativeUI