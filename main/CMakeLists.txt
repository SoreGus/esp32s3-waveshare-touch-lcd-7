idf_component_register(
    SRCS
        "main.cpp"
        "api/zelda_view.cpp"
        "api/zelda_view_model.cpp"

    INCLUDE_DIRS
        "."

    REQUIRES
        declarative_ui
        dependences
        esp_http_client
        mbedtls
        esp_wifi
        esp_netif
        esp_event
        nvs_flash
        json
)

target_compile_features(
    ${COMPONENT_LIB}
    PUBLIC
        cxx_std_17
)
