add_executable(
        image-watcher
        model/ImageModel.cpp
        view/ImageView.cpp
        dialogs/MacFileDialog.cpp
        controller/ImageController.cpp
        main.cpp
)

target_link_libraries(image-watcher PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)