add_executable(
        figure
        main.cpp
        Window.cpp
        Dodecahedron.cpp
)

target_link_libraries(figure PRIVATE
        glfw
        GLEW::GLEW
        GLUT::GLUT
        glm::glm
)