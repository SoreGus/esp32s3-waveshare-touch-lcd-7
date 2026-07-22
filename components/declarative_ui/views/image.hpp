#pragma once

#include "core/view.hpp"

namespace DeclarativeUI {

enum class ImageSource {
    Local,
    URL
};

// Local images use a source understood by LVGL, such as a registered file path.
View Image(const char* source, ImageSource type = ImageSource::Local);
View Image(const lv_img_dsc_t* source);

} // namespace DeclarativeUI
