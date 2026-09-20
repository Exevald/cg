add_executable(
        paraboloid
        main.cpp
        Window.cpp
)

target_link_libraries(paraboloid PRIVATE
        lw8_common
)
