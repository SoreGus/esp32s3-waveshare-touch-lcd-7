#include "declarative_ui.hpp"
#include "platform/platform.hpp"

namespace DeclarativeUI {

void UI::initialize()
{
}

void UI::render(const View& root)
{
    if (!root.valid()) {
        return;
    }

    if (!Platform::lock(-1)) {
        return;
    }

    lv_obj_t* screen = lv_scr_act();

    lv_obj_clean(screen);

    lv_obj_clear_flag(
        screen,
        LV_OBJ_FLAG_SCROLLABLE
    );

    lv_obj_set_style_pad_all(
        screen,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_border_width(
        screen,
        0,
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_color(
        screen,
        lv_color_hex(0x000000),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        screen,
        LV_OPA_COVER,
        LV_PART_MAIN
    );

    lv_obj_t* mountedRoot = root.mount(screen);

    if (mountedRoot != nullptr) {
        lv_obj_align(
            mountedRoot,
            LV_ALIGN_TOP_LEFT,
            0,
            0
        );
    }

    Platform::unlock();
}

void UI::run(const View& root)
{
    initialize();
    render(root);
}

void UI::clear()
{
    if (!Platform::lock(-1)) {
        return;
    }

    lv_obj_clean(
        lv_scr_act()
    );

    Platform::unlock();
}

} // namespace DeclarativeUI