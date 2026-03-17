add_executable(
        tetris
        model/Model.cpp
        view/View.cpp
        main.cpp
        controller/Controller.cpp
)

target_link_libraries(tetris PRIVATE
        glfw
        GLEW::GLEW
        GLUT::GLUT
        glm::glm
)