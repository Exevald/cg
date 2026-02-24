add_executable(
        rick
        model/RickModel.cpp
        view/RickView.cpp
        controller/RickController.cpp
        main.cpp
)

target_link_libraries(rick PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)