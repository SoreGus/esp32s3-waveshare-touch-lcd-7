#include "tool_bar.hpp"

#include "layout/stack.hpp"
#include "views/button.hpp"

namespace DeclarativeUI {

View ToolBar(std::initializer_list<View> items)
{
    return HStack(items)
        .fillWidth()
        .padding(10)
        .spacing(8)
        .alignment(Alignment::Center)
        .background(Color::hex(0x172433))
        .cornerRadius(14);
}

View ToolBarItem(const char* title, std::function<void()> action)
{
    return Button(title)
        .padding(12, 8)
        .background(Color::hex(0x334155))
        .cornerRadius(9)
        .onTap(std::move(action));
}

} // namespace DeclarativeUI
