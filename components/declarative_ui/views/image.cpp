#include "image.hpp"

#include <memory>

#include "Views/loading.hpp"
#include "core/view_internal.hpp"
#include "layout/stack.hpp"

namespace DeclarativeUI {

View Image(const char* source, ImageSource type)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Image;
    node->image.source = source != nullptr ? source : "";
    node->image.remote = type == ImageSource::URL;
    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    if (type == ImageSource::Local) {
        return View(std::move(node));
    }

    return ZStack({
        View(std::move(node)).fill(),
        Loading()
    }).alignment(Alignment::Center);
}

View Image(const lv_img_dsc_t* source)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Image;
    node->image.localSource = source;
    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;
    return View(std::move(node));
}

} // namespace DeclarativeUI
