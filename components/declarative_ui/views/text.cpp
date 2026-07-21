#include "text.hpp"

#include <memory>

#include "core/view_internal.hpp"
#include "platform/platform.hpp"

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

View Text(const String& value)
{
    return Text(value.c_str());
}

View Text(const Reactive::Binding<String>& value)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::Text;
    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;
    node->reactiveBindings.emplace_back([value](lv_obj_t* object) {
        struct Context {
            Reactive::Subscription subscription;
        };
        lv_label_set_text(object, value.get().c_str());
        auto* context = new Context();
        context->subscription = value.subscribe([object, value] {
            if (!Platform::lock(-1)) return;
            lv_label_set_text(object, value.get().c_str());
            Platform::unlock();
        });
        lv_obj_add_event_cb(object, [](lv_event_t* event) {
            delete static_cast<Context*>(lv_event_get_user_data(event));
        }, LV_EVENT_DELETE, context);
    });
    return View(node);
}

} // namespace DeclarativeUI
