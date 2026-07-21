idf_component_register(
    SRCS
        "main.cpp"
        "lvgl_port.c"
        "waveshare_rgb_lcd_port.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        declarative_ui
        esp_driver_gpio
        esp_lcd
        esp_timer
        espressif__esp_lcd_touch
        espressif__esp_lcd_touch_gt911
)

target_compile_features(
    ${COMPONENT_LIB}
    PUBLIC
        cxx_std_17
)