#include "scroll_view.hpp"

#include <memory>
#include <utility>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

View ScrollView(
    std::initializer_list<View> children
)
{
    return ScrollView(
        ScrollAxis::Vertical,
        children
    );
}

View ScrollView(
    ScrollAxis axis,
    std::initializer_list<View> children
)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::ScrollView;
    node->scrollAxis = axis;

    node->children.assign(
        children.begin(),
        children.end()
    );

    node->style.size.fillWidth = true;
    node->style.size.height = LV_SIZE_CONTENT;

    /*
     * This prevents applyViewStyle() from disabling
     * LV_OBJ_FLAG_SCROLLABLE after mounting.
     */
    node->style.scrollable = true;

    node->alignment = Alignment::Start;
    node->justify = Justify::Start;

    return View(std::move(node));
}

} // namespace DeclarativeUI