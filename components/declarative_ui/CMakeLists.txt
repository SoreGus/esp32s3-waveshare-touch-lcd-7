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

        "examples/state/examples.cpp"
        "examples/binding/examples.cpp"
        "examples/state_object/dashboard_view.cpp"
        "examples/state_object/dashboard_view_model.cpp"
        "examples/state_object/device_view.cpp"
        "examples/state_object/device_view_model.cpp"
        "examples/list/examples.cpp"
        "examples/for_each/examples.cpp"
        "examples/compose/compose_view_model.cpp"
        "examples/compose/compose_components.cpp"
        "examples/compose/compose_view.cpp"

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
