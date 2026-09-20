add_library(lw8_common
        common/BaseWindow.cpp
        common/FrameBuffer.cpp
        common/RayTracer.cpp
        common/RayTracingWindow.cpp
)

target_include_directories(lw8_common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_link_libraries(lw8_common PUBLIC
        glfw
        GLEW::GLEW
        glm::glm
)

add_subdirectory(lighting)
add_subdirectory(shadows)
add_subdirectory(paraboloid)
