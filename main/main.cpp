#include "esp_log.h"
#include "waveshare_rgb_lcd_port.h"
#include "platform.hpp"
#include "declarative_ui.hpp"

using namespace DeclarativeUI;

namespace {

static const char* TAG = "app";

constexpr Color screenBackground()
{
    return Color::rgb(
        12,
        17,
        26
    );
}

constexpr Color cardBackground()
{
    return Color::rgb(
        27,
        34,
        47
    );
}

constexpr Color cardBorder()
{
    return Color::rgb(
        55,
        66,
        85
    );
}

constexpr Color secondaryText()
{
    return Color::rgb(
        158,
        169,
        187
    );
}

constexpr Color primaryColor()
{
    return Color::rgb(
        72,
        139,
        245
    );
}

constexpr Color successColor()
{
    return Color::rgb(
        62,
        201,
        125
    );
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
        Circle()
            .frame(18, 18)
            .fill(successColor()),

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
                .fill(Color::rgb(48, 91, 153)),

            RoundedRectangle(5)
                .frame(28, 54)
                .fill(Color::rgb(58, 112, 187)),

            RoundedRectangle(5)
                .frame(28, 42)
                .fill(Color::rgb(67, 129, 214)),

            RoundedRectangle(5)
                .frame(28, 72)
                .fill(primaryColor()),

            RoundedRectangle(5)
                .frame(28, 58)
                .fill(Color::rgb(94, 157, 247))
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
            .background(Color::rgb(125, 88, 222))
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
            .background(Color::rgb(194, 67, 81))
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