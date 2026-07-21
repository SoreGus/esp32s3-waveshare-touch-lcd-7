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
- ZStack
- ScrollView
- Spacer

## Input Controls

`Slider`, `Picker`, `Toggle`, and `SegmentedControl` work directly with a
`Binding`, so user interaction and external state changes stay synchronized.
They expose focused styling for their respective domains, including colors,
pill/circle thumbs, and animation timing.

```cpp
State<int> brightness(70);
State<bool> notifications(true);
State<int> mode(0);

VStack({
    Slider(Bind(brightness), 0, 100)
        .sliderFilledColor(Color::Cyan())
        .sliderThumb(SliderThumb::Pill),
    Picker({"Ocean", "Forest", "Sunset"}, Bind(mode)),
    Toggle(Bind(notifications)).toggleColors(Color::Teal(), Color::DarkGray()),
    SegmentedControl({"Eco", "Normal", "Boost"}, Bind(mode))
        .segmentedColors(Color::hex(0x223246), Color::Purple())
        .segmentedAnimation(220)
});
```

`ZStack` overlays its children at the same origin. Children declared later are
drawn above previous children. Use `frame()` or `fill()` to define its bounds,
and `offset()` to position an overlaid child.

## Colors

Use one of the predefined colors or create one from RGB or hexadecimal values.

```cpp
Text("Connected")
    .foregroundColor(Color::Teal());

Button("Save")
    .background(Color::hex(0x6750A4));

Rectangle()
    .fill(Color::rgb(42, 157, 143).lighten(10));
```

Predefined colors include `Black`, `White`, `Red`, `Pink`, `Purple`, `Indigo`,
`Blue`, `Cyan`, `Teal`, `Green`, `Lime`, `Yellow`, `Orange`, `Brown`, `Gray`,
`LightGray`, and `DarkGray`. Use `lighten(percentage)` and
`darken(percentage)` to derive related colors.

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

Buttons automatically provide pressed touch feedback: their background becomes
18% darker while the touch is held and returns to its normal color when it is
released or cancelled. This works with both the default button color and a
custom `background()` color.

## Reactive state

`State<T>` holds a reactive value. Use `Bind(...)` to update a view whenever
the value changes.

```cpp
State<int> temperature(24);

Text(Bind(temperature, [](int value) {
    return String::format("%d C", value);
}));
```

For models with multiple properties, inherit from `ObservableObject`. The
`published(...)` helper associates a property with the model without requiring
an explicit `*this` argument.

```cpp
class DashboardModel final : public ObservableObject {
public:
    Published<String> title = published("Dashboard");
    Published<int> temperature = published(24);
};

StateObject<DashboardModel> dashboard;
```

`ListState<T>` and `PublishedList<T>` support `append`, `insert`, `removeAt`,
`replace`, `move`, `clear`, and `replaceAll`. Use `ForEach` with a stable ID to
render an observable collection. The examples in `examples/` demonstrate each
reactive API and can be selected from the application `main.cpp`.

## Examples

Examples are grouped by feature under `examples/state`, `examples/binding`,
`examples/state_object`, `examples/list`, and `examples/for_each`. The project
`main.cpp` lists every example and keeps exactly one active for device testing.

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
