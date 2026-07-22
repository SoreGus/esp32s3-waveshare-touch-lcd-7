#include "title.hpp"

#include "views/text.hpp"

namespace DeclarativeUI {

View Title(const char* value)
{
    return Text(value)
        .foregroundColor(Color::White())
        .font(&lv_font_montserrat_20);
}

View Title(const String& value)
{
    return Title(value.c_str());
}

} // namespace DeclarativeUI
