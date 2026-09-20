add_executable(
        shadows
        main.cpp
        Window.cpp
)

target_link_libraries(shadows PRIVATE
        lw8_common
)
