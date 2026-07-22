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
        "views/controls.cpp"

        "Views/image.cpp"
        "Views/loading.cpp"

        "Navigation/navigation_stack.cpp"
        "Navigation/navigation_link.cpp"
        "Navigation/tool_bar.cpp"
        "Navigation/title.cpp"

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
        "examples/gadgets/gadgets_view.cpp"
        "examples/gadgets/gadgets_view_model.cpp"

        "examples/navigation/navigation_view.cpp"

        "examples/showcase/showcase_view.cpp"
        "examples/showcase/showcase_view_models.cpp"
        "examples/showcase/logo_lvgl.cpp"

    INCLUDE_DIRS
        "."
        "core"
        "platform"
        "layout"
        "views"
        "Views"
        "Navigation"

    REQUIRES
        lvgl

    EMBED_FILES
        "logo_lvgl.png"
)

target_compile_features(
    ${COMPONENT_LIB}
    PUBLIC
        cxx_std_17
)
