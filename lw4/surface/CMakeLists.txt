add_executable(
        surface
        main.cpp
        Window.cpp
        MoebiusStrip.cpp
)

target_link_libraries(surface PRIVATE
        glfw
        GLEW::GLEW
        GLUT::GLUT
        glm::glm
)