#include "esp_log.h"
#include "waveshare_rgb_lcd_port.h"
#include "platform.hpp"
#include "declarative_ui.hpp"

using namespace DeclarativeUI;

namespace {

static const char* TAG = "app";

constexpr Color screenBackground()
{
    return Color::hex(0x0C111A);
}

constexpr Color cardBackground()
{
    return Color::hex(0x1B222F);
}

constexpr Color cardBorder()
{
    return Color::hex(0x374255);
}

constexpr Color secondaryText()
{
    return Color::hex(0x9EA9BB);
}

constexpr Color primaryColor()
{
    return Color::hex(0x488BF5);
}

constexpr Color successColor()
{
    return Color::Green().lighten(15);
}

class CardModifier final : public ViewModifier {
public:
    void apply(View& view) const override
    {
        view
            .fillWidth()
            .padding(18)
            .background(cardBackground())
            .cornerRadius(16)
            .border(
                cardBorder(),
                1
            );
    }
};

View makeMetricCard(
    const char* title,
    const char* value,
    Color accent
)
{
    static const CardModifier cardModifier;

    return VStack({
        HStack({
            Text(title)
                .foregroundColor(secondaryText()),

            Spacer(),

            Circle()
                .frame(12, 12)
                .fill(accent)
        })
            .fillWidth()
            .alignment(Alignment::Center),

        Text(value)
            .foregroundColor(Color::White()),

        HStack({
            RoundedRectangle(5)
                .frame(110, 8)
                .fill(accent),

            Rectangle()
                .grow()
                .height(8)
                .fill(cardBorder())
        })
            .fillWidth()
            .spacing(5)
            .alignment(Alignment::Center)
    })
        .spacing(12)
        .modifier(cardModifier);
}

View makeStatusCard()
{
    static const CardModifier cardModifier;

    return HStack({
        ZStack({
            Circle()
                .frame(18, 18)
                .fill(successColor()),

            Circle()
                .frame(6, 6)
                .fill(Color::White())
                .offset(9, 9)
        })
            .frame(18, 18),

        VStack({
            Text("Display connected")
                .foregroundColor(Color::White()),

            Text("RGB panel and touch are active")
                .foregroundColor(secondaryText())
        })
            .spacing(5),

        Spacer(),

        Text("ONLINE")
            .foregroundColor(successColor())
    })
        .spacing(12)
        .alignment(Alignment::Center)
        .modifier(cardModifier);
}

View makeSystemActivity()
{
    static const CardModifier cardModifier;

    return VStack({
        HStack({
            Text("System activity")
                .foregroundColor(Color::White()),

            Spacer(),

            Text("LIVE")
                .foregroundColor(successColor())
        })
            .fillWidth()
            .alignment(Alignment::Center),

        Text("The content is now inside a vertical ScrollView.")
            .foregroundColor(secondaryText()),

        HStack({
            RoundedRectangle(5)
                .frame(28, 34)
                .fill(Color::hex(0x305B99)),

            RoundedRectangle(5)
                .frame(28, 54)
                .fill(Color::hex(0x3A70BB)),

            RoundedRectangle(5)
                .frame(28, 42)
                .fill(Color::hex(0x4381D6)),

            RoundedRectangle(5)
                .frame(28, 72)
                .fill(primaryColor()),

            RoundedRectangle(5)
                .frame(28, 58)
                .fill(primaryColor().lighten(12))
        })
            .fillWidth()
            .height(80)
            .spacing(12)
            .alignment(Alignment::End)
            .justify(Justify::Center)
    })
        .spacing(14)
        .modifier(cardModifier);
}

View makeActions()
{
    return HStack({
        Button("Refresh")
            .foregroundColor(Color::White())
            .background(primaryColor())
            .cornerRadius(14)
            .padding(24, 13)
            .onTap([] {
                ESP_LOGI(
                    TAG,
                    "Refresh pressed"
                );
            }),

        Button("Touch test")
            .foregroundColor(Color::White())
            .background(Color::Purple().lighten(12))
            .cornerRadius(14)
            .padding(24, 13)
            .onTap([] {
                ESP_LOGI(
                    TAG,
                    "Touch test pressed"
                );
            }),

        Spacer(),

        Button("Power")
            .foregroundColor(Color::White())
            .background(Color::Red().darken(8))
            .cornerRadius(14)
            .padding(24, 13)
            .onTap([] {
                ESP_LOGI(
                    TAG,
                    "Power pressed"
                );
            })
    })
        .fillWidth()
        .spacing(12)
        .alignment(Alignment::Center);
}

View makeDashboard()
{
    return VStack({
        HStack({
            VStack({
                Text("ESP32 Dashboard")
                    .foregroundColor(Color::White()),

                Text("Declarative UI demonstration")
                    .foregroundColor(secondaryText())
            })
                .spacing(5),

            Spacer(),

            HStack({
                Circle()
                    .frame(10, 10)
                    .fill(successColor()),

                Text("READY")
                    .foregroundColor(successColor())
            })
                .spacing(7)
                .alignment(Alignment::Center)
        })
            .fillWidth()
            .alignment(Alignment::Center),

        /*
         * This is the viewport.
         *
         * grow() makes it occupy all space between the header
         * and the fixed action bar.
         */
        ScrollView({
            makeMetricCard(
                "TEMPERATURE",
                "24 C",
                Color::Orange()
            ),

            makeMetricCard(
                "HUMIDITY",
                "68%",
                Color::Blue()
            ),

            makeStatusCard(),

            makeSystemActivity(),

            makeMetricCard(
                "TOUCH INPUT",
                "ACTIVE",
                successColor()
            ),

            makeMetricCard(
                "RENDERER",
                "LVGL",
                primaryColor()
            )
        })
            .grow()
            .fillWidth()
            .spacing(14)
            .paddingRight(8),

        /*
         * This remains fixed at the bottom.
         * It is outside the ScrollView.
         */
        makeActions()
    })
        .fill()
        .padding(22)
        .spacing(16)
        .background(screenBackground());
}

} // namespace

extern "C" void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();

    DeclarativeUI::Platform::configure(
        lvgl_port_lock,
        lvgl_port_unlock
    );

    DeclarativeUI::UI::run(
        makeDashboard()
    );
}
