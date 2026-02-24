add_executable(
        circles
        CircleRenderer.cpp
        main.cpp
)

target_link_libraries(circles PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)