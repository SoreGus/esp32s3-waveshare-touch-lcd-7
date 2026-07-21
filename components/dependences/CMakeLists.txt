idf_component_register(
    SRCS
        "lvgl_port.c"
        "waveshare_rgb_lcd_port.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        driver
        esp_lcd
        esp_timer
        espressif__esp_lcd_touch
        espressif__esp_lcd_touch_gt911
        lvgl
)
