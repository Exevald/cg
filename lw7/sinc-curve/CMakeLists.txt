add_executable(
        sinc-curve
        main.cpp
        Window.cpp
        ../common/BaseWindow.cpp
)

target_include_directories(sinc-curve PRIVATE
        ../common
)

target_link_libraries(sinc-curve PRIVATE
        glfw
        GLEW::GLEW
        glm::glm
)
