#pragma once

#include <functional>
#include <utility>

#include "declarative_ui.hpp"

namespace DeclarativeUI::Examples::Shared {

inline View card(View content)
{
    content.fillWidth()
        .spacing(14);

    return VStack({content})
        .fillWidth()
        .padding(22)
        .background(Color::hex(0x1B2634))
        .cornerRadius(20)
        .border(Color::hex(0x334155), 1);
}

inline View row(View content, Color accent = Color::Cyan())
{
    return VStack({content})
        .fillWidth()
        .padding(14)
        .background(Color::hex(0x223246))
        .cornerRadius(14)
        .border(accent.darken(20), 1);
}

inline View fullCard(View content)
{
    content.fill()
        .justify(Justify::SpaceBetween);

    return VStack({content})
        .fill()
        .grow()
        .padding(22)
        .background(Color::hex(0x1B2634))
        .cornerRadius(20)
        .border(Color::hex(0x334155), 1);
}

inline View sectionLabel(const char* value)
{
    return Text(value)
        .fillWidth()
        .foregroundColor(Color::Gray().lighten(28))
        .textAlignment(TextAlignment::Center);
}

inline View metric(Reactive::Binding<String> value)
{
    return Text(value)
        .fillWidth()
        .foregroundColor(Color::White())
        .padding(14)
        .background(Color::hex(0x243447))
        .cornerRadius(12)
        .textAlignment(TextAlignment::Center);
}

inline View actionButton(const char* title, std::function<void()> action)
{
    return Button(title)
        .fillWidth()
        .onTap(std::move(action));
}

inline View bottomActionButton(const char* title, std::function<void()> action)
{
    return Button(title)
        .grow()
        .onTap(std::move(action));
}

inline View header(const char* title)
{
    return VStack({
        Text(title).foregroundColor(Color::White()),
        Text("DeclarativeUI reactive example")
            .foregroundColor(Color::Gray().lighten(20))
    })
        .fillWidth()
        .padding(18)
        .background(Color::hex(0x172433))
        .cornerRadius(18)
        .spacing(5);
}

inline View screen(const char* title, View content)
{
    return VStack({
        header(title),
        content
    })
        .fill()
        .padding(20)
        .spacing(16)
        .background(Color::hex(0x101820));
}

inline View scrollingScreen(const char* title, View content)
{
    return VStack({
        header(title),
        ScrollView({content})
            .grow()
            .fillWidth()
            .paddingRight(8)
    })
        .fill()
        .padding(20)
        .spacing(16)
        .background(Color::hex(0x101820));
}

inline View collectionScreen(View content, View actions)
{
    return VStack({
        ScrollView({content})
            .grow()
            .fillWidth()
            .paddingRight(8),
        actions
    })
        .fill()
        .padding(20)
        .spacing(16)
        .background(Color::hex(0x101820));
}

} // namespace DeclarativeUI::Examples::Shared
