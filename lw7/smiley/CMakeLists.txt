add_executable(
        smiley
        main.cpp
        Window.cpp
        ../common/BaseWindow.cpp
)

target_include_directories(smiley PRIVATE
        ../common
)

target_link_libraries(smiley PRIVATE
        glfw
        GLEW::GLEW
)
