# DeclarativeUI

DeclarativeUI is a lightweight declarative UI framework built on top of
**LVGL 8.4** for ESP-IDF.

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

The framework is independent of the display hardware, but all rendering must
be synchronized with the application's LVGL port. Configure it after the LVGL
port is initialized and before the first call to `UI::run()`.

The host application must provide the LVGL synchronization callbacks.

```cpp
DeclarativeUI::Platform::configure(
    lvgl_port_lock,
    lvgl_port_unlock
);
```

## Requirements

- ESP-IDF
- LVGL 8.4
- C++17 or later

## License

MIT
