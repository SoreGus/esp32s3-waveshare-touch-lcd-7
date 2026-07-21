#include "button.hpp"

#include <functional>
#include <memory>
#include <utility>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

namespace {

struct ButtonActionContext {
    std::function<void()> action;
};

void buttonEventHandler(lv_event_t* event)
{
    auto* context = static_cast<ButtonActionContext*>(
        lv_event_get_user_data(event)
    );

    if (context == nullptr) {
        return;
    }

    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_CLICKED) {
        if (context->action) {
            context->action();
        }

        return;
    }

    if (code == LV_EVENT_DELETE) {
        delete context;
    }
}

} // namespace

View Button(const char* title)
{
    auto node = std::make_shared<ViewNode>();

    node->type = ViewType::Button;

    node->text = title != nullptr
        ? title
        : "";

    node->style.size.width = LV_SIZE_CONTENT;
    node->style.size.height = LV_SIZE_CONTENT;

    node->style.hasForegroundColor = true;
    node->style.foregroundColor = Color::White();

    node->style.hasBackgroundColor = true;
    node->style.backgroundColor = Color::Blue();

    node->style.padding.left = 20;
    node->style.padding.right = 20;
    node->style.padding.top = 12;
    node->style.padding.bottom = 12;

    node->style.cornerRadius = 12;

    return View(std::move(node));
}

View& View::onTap(std::function<void()> action)
{
    if (node_) {
        node_->action = std::move(action);
    }

    return *this;
}

void registerButtonAction(
    lv_obj_t* object,
    const std::function<void()>& action
)
{
    if (object == nullptr || !action) {
        return;
    }

    auto* context = new ButtonActionContext {
        .action = action
    };

    lv_obj_add_event_cb(
        object,
        buttonEventHandler,
        LV_EVENT_ALL,
        context
    );
}

} // namespace DeclarativeUI