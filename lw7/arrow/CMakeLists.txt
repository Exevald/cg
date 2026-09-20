add_executable(
        arrow
        main.cpp
        Window.cpp
        ../common/BaseWindow.cpp
)

target_include_directories(arrow PRIVATE
        ../common
)

target_link_libraries(arrow PRIVATE
        glfw
        GLEW::GLEW
)
