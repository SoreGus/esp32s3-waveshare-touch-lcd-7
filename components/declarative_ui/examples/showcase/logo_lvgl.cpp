#include "logo_lvgl.hpp"

#include <cstdint>

namespace {

extern const uint8_t logo_lvgl_png_start[] asm("_binary_logo_lvgl_png_start");
extern const uint8_t logo_lvgl_png_end[] asm("_binary_logo_lvgl_png_end");

} // namespace

namespace DeclarativeUI::Examples::Showcase::Assets {

const lv_img_dsc_t logoLvgl = {
    {
        LV_IMG_CF_TRUE_COLOR_ALPHA,
        0,
        0,
        300,
        95
    },
    static_cast<uint32_t>(logo_lvgl_png_end - logo_lvgl_png_start),
    logo_lvgl_png_start
};

} // namespace DeclarativeUI::Examples::Showcase::Assets
