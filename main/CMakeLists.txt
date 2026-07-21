idf_component_register(
    SRCS
        "main.cpp"

    INCLUDE_DIRS
        "."

    REQUIRES
        declarative_ui
        dependences
)

target_compile_features(
    ${COMPONENT_LIB}
    PUBLIC
        cxx_std_17
)
