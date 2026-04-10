add_executable(
        wow-alchemy
        model/AlchemyModel.cpp
        view/SFMLView.cpp
        controller/AlchemyController.cpp
        main.cpp
)

target_link_libraries(wow-alchemy PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)