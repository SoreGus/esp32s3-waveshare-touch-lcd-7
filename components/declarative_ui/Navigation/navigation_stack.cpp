#include "navigation_stack.hpp"

#include <memory>
#include <vector>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

namespace {

struct NavigationContext {
    lv_obj_t* root = nullptr;
    std::vector<lv_obj_t*> pages;
};

int navigationWidth(lv_obj_t* root)
{
    const int width = lv_obj_get_content_width(root);
    return width > 0 ? width : lv_disp_get_hor_res(lv_obj_get_disp(root));
}

lv_obj_t* mountPage(NavigationContext* context, const View& content)
{
    lv_obj_t* page = lv_obj_create(context->root);
    removeDefaultStyle(page);
    lv_obj_add_flag(page, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(page, lv_pct(100), lv_pct(100));
    lv_obj_set_pos(page, 0, 0);
    [[maybe_unused]] lv_obj_t* mountedContent = content.mount(page);
    return page;
}

void animateX(lv_obj_t* object, int from, int to, int duration, lv_anim_ready_cb_t ready = nullptr)
{
    lv_obj_set_x(object, from);
    lv_anim_t animation;
    lv_anim_init(&animation);
    lv_anim_set_var(&animation, object);
    lv_anim_set_values(&animation, from, to);
    lv_anim_set_time(&animation, duration);
    lv_anim_set_path_cb(&animation, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&animation, [](void* target, int32_t value) {
        lv_obj_set_x(static_cast<lv_obj_t*>(target), value);
    });
    if (ready != nullptr) lv_anim_set_ready_cb(&animation, ready);
    lv_anim_start(&animation);
}

void push(NavigationContext* context, const View& destination, int duration)
{
    if (context == nullptr || context->root == nullptr || !destination.valid()) return;

    lv_obj_t* previous = context->pages.empty() ? nullptr : context->pages.back();
    lv_obj_t* next = mountPage(context, destination);
    const int width = navigationWidth(context->root);

    if (previous != nullptr) {
        animateX(previous, 0, -width / 3, duration);
    }
    animateX(next, width, 0, duration);
    context->pages.push_back(next);
}

void pop(NavigationContext* context, int duration)
{
    if (context == nullptr || context->pages.size() < 2) return;

    lv_obj_t* current = context->pages.back();
    lv_obj_t* previous = context->pages[context->pages.size() - 2];
    context->pages.pop_back();
    const int width = navigationWidth(context->root);

    animateX(previous, -width / 3, 0, duration);
    animateX(current, 0, width, duration, [](lv_anim_t* animation) {
        lv_obj_del(static_cast<lv_obj_t*>(animation->var));
    });
}

NavigationContext* findContext(lv_obj_t* object)
{
    for (lv_obj_t* ancestor = object; ancestor != nullptr; ancestor = lv_obj_get_parent(ancestor)) {
        auto* context = static_cast<NavigationContext*>(lv_obj_get_user_data(ancestor));
        if (context != nullptr && context->root == ancestor) return context;
    }
    return nullptr;
}

void bubbleTapToNavigationLink(lv_obj_t* object)
{
    lv_obj_add_flag(object, LV_OBJ_FLAG_EVENT_BUBBLE);
    const uint32_t childCount = lv_obj_get_child_cnt(object);
    for (uint32_t index = 0; index < childCount; ++index) {
        bubbleTapToNavigationLink(lv_obj_get_child(object, index));
    }
}

} // namespace

View NavigationStack(std::initializer_list<View> children)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::NavigationStack;
    node->children.assign(children.begin(), children.end());
    node->style.size.fillWidth = true;
    node->style.size.fillHeight = true;
    return View(std::move(node));
}

lv_obj_t* mountNavigationStack(lv_obj_t* parent, const ViewNode& node)
{
    lv_obj_t* root = lv_obj_create(parent);
    removeDefaultStyle(root);
    lv_obj_set_style_clip_corner(root, true, LV_PART_MAIN);

    auto* context = new NavigationContext();
    context->root = root;
    lv_obj_set_user_data(root, context);
    lv_obj_add_event_cb(root, [](lv_event_t* event) {
        delete static_cast<NavigationContext*>(lv_event_get_user_data(event));
    }, LV_EVENT_DELETE, context);

    if (!node.children.empty()) {
        context->pages.push_back(mountPage(context, node.children.front()));
    }
    return root;
}

lv_obj_t* mountNavigationLink(lv_obj_t* parent, const ViewNode& node)
{
    lv_obj_t* button = lv_btn_create(parent);
    removeDefaultStyle(button);
    if (!node.children.empty()) {
        lv_obj_t* label = node.children.front().mount(button);
        bubbleTapToNavigationLink(label);
    } else {
        lv_obj_t* label = lv_label_create(button);
        lv_label_set_text(label, node.text.c_str());
        lv_obj_center(label);
    }

    struct LinkContext {
        View destination;
        int duration = 260;
    };
    auto* context = new LinkContext { node.navigationLink.destination, node.navigationLink.animationTime };
    lv_obj_add_event_cb(button, [](lv_event_t* event) {
        auto* link = static_cast<LinkContext*>(lv_event_get_user_data(event));
        if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
            NavigationContext* navigation = findContext(lv_event_get_target(event));
            if (link->destination.valid()) push(navigation, link->destination, link->duration);
            else pop(navigation, link->duration);
        }
        if (lv_event_get_code(event) == LV_EVENT_DELETE) delete link;
    }, LV_EVENT_ALL, context);
    return button;
}

} // namespace DeclarativeUI
