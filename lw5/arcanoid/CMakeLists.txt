add_executable(
        arcanoid
        main.cpp
        controller/BaseWindow.cpp
        controller/GameController.cpp
        model/GameModel.cpp
        view/GameView.cpp
        view/TextureBuilder.cpp
)

target_compile_definitions(arcanoid PRIVATE
        ARCANOID_TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/textures"
)

target_link_libraries(arcanoid PRIVATE
        glfw
        GLEW::GLEW
        GLUT::GLUT
        glm::glm
)
