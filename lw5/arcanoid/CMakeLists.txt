add_executable(
        arcanoid
        main.cpp
        controller/BaseWindow.cpp
        controller/GameController.cpp
        model/GameModel.cpp
        view/GameView.cpp
        view/TextureFactory.cpp
)

target_link_libraries(arcanoid PRIVATE
        glfw
        GLEW::GLEW
        GLUT::GLUT
        glm::glm
)
