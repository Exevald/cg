add_executable(
        lighting
        main.cpp
        Window.cpp
)

target_link_libraries(lighting PRIVATE
        lw8_common
)
