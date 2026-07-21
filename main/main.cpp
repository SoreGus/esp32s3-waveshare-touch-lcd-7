#include "waveshare_rgb_lcd_port.h"
#include "platform.hpp"
#include "declarative_ui.hpp"
#include "examples/examples.hpp"

extern "C" void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();

    DeclarativeUI::Platform::configure(
        lvgl_port_lock,
        lvgl_port_unlock
    );

    // State examples
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::State::counter());
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::State::visibility());

    // Binding examples
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::Binding::status());
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::Binding::colors());

    // StateObject examples
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::StateObject::dashboard());
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::StateObject::device());

    // ListState examples
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::List::operations());
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::List::replaceAndMove());

    // ForEach examples
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::ForEach::sensors());
    // DeclarativeUI::UI::run(DeclarativeUI::Examples::ForEach::cards());

    // Compose example
    DeclarativeUI::UI::run(DeclarativeUI::Examples::Compose::dashboard());
}
