# DeclarativeUI

DeclarativeUI is a lightweight declarative UI framework built on top of LVGL for ESP-IDF.

It provides a simple, composable API inspired by modern declarative UI frameworks while remaining fully native to C++ and embedded systems.

## Features

- Declarative view hierarchy
- Composable layouts
- Fluent modifiers
- Reusable view modifiers
- Native LVGL rendering
- ESP-IDF component

## Example

```cpp
using namespace DeclarativeUI;

UI::run(
    VStack({
        Text("Dashboard")
            .foregroundColor(Color::White),

        HStack({
            Text("Temperature"),
            Spacer(),
            Text("24°C")
        })
        .padding()
        .background(Color::Blue)
    })
    .fill()
);
```

## Views

- Text
- Button
- Rectangle
- RoundedRectangle
- Circle
- HStack
- VStack
- ScrollView
- Spacer

## Layout Modifiers

- fill()
- fillWidth()
- fillHeight()
- frame()
- width()
- height()
- padding()
- spacing()
- alignment()
- justify()
- grow()
- offset()

## Appearance Modifiers

- background()
- foregroundColor()
- fill()
- border()
- cornerRadius()
- opacity()
- hidden()

## Interaction

- onTap()
- modifier()

## Custom Modifiers

Create reusable styles by inheriting from `ViewModifier`.

```cpp
class CardModifier : public ViewModifier
{
public:
    void apply(View& view) const override
    {
        view
            .padding(16)
            .background(Color::White)
            .cornerRadius(12);
    }
};

static const CardModifier card;

VStack({
    Text("Hello")
})
.modifier(card);
```

## Project Structure

```
declarative_ui/
├── core/
├── layout/
├── platform/
├── views/
├── declarative_ui.hpp
└── CMakeLists.txt
```

## Platform Integration

The framework is platform-independent.

The host application must provide the LVGL synchronization callbacks.

```cpp
DeclarativeUI::Platform::configure(
    lvgl_port_lock,
    lvgl_port_unlock
);
```

## Requirements

- ESP-IDF
- LVGL 9.x
- C++17 or later

## License

MIT