add_executable(
        tunnel-flight
        main.cpp
        Window.cpp
        TextureLoader.cpp
        ../common/BaseWindow.cpp
)

target_include_directories(tunnel-flight PRIVATE
        ../common
)

target_compile_definitions(tunnel-flight PRIVATE
        TUNNEL_TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/textures"
)

target_link_libraries(tunnel-flight PRIVATE
        glfw
        GLEW::GLEW
        glm::glm
)
