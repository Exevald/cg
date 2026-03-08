add_executable(
        bezier-curve
        src/ControlPoint.cpp
        src/BezierCurve.cpp
        src/Input.cpp
        src/Renderer.cpp
        src/App.cpp
        main.cpp
)

target_link_libraries(bezier-curve PRIVATE
        glfw
        GLEW::GLEW
        glm::glm
)