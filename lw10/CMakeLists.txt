add_library(lw10_common
        common/BaseWindow.cpp
        common/TextureLoader.cpp
)

target_include_directories(lw10_common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_link_libraries(lw10_common PUBLIC
        glfw
        GLEW::GLEW
        SFML::Graphics
        SFML::System
)

add_subdirectory(sharpen)