include_guard()

add_subdirectory(opengl)

add_library(mono_module_renderer)
add_library(mono::renderer ALIAS mono_module_renderer)

target_sources(mono_module_renderer
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderTexture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderWindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pass/ImmediateDrawRenderPass.cpp"
)

target_include_directories(mono_module_renderer
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_renderer
    PUBLIC
    mono::common
    mono::external::glm
    mono::renderer_opengl
    mono::dev_ui
)
