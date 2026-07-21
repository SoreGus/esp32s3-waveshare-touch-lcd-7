idf_component_register(
    SRCS
        "core/ui.cpp"
        "core/view.cpp"

        "platform/platform.cpp"

        "layout/stack.cpp"
        "layout/scroll_view.cpp"

        "views/text.cpp"
        "views/shape.cpp"
        "views/button.cpp"

    INCLUDE_DIRS
        "."
        "core"
        "platform"
        "layout"
        "views"

    REQUIRES
        lvgl
)

target_compile_features(
    ${COMPONENT_LIB}
    PUBLIC
        cxx_std_17
)