add_executable(
        sharpen
        main.cpp
        Window.cpp
)

target_compile_definitions(sharpen PRIVATE
        LW10_SAMPLE_TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/image.jpg"
)

target_link_libraries(sharpen PRIVATE
        lw10_common
        glm::glm
)
