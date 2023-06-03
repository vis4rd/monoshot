include_guard()

add_library(vis_module_renderer_opengl_backend)
add_library(vis::renderer_opengl ALIAS vis_module_renderer_opengl_backend)

message(STATUS "Renderer: using OpenGL backend")

if(DEBUG_MODE)
    target_compile_definitions(vis_module_renderer_opengl_backend
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_renderer_opengl_backend
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_renderer_opengl_backend
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ElementBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/FrameBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexArray.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/Renderer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/Shader.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/ShaderManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/ShaderProgram.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/texture/Animation.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/texture/Texture.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/window/NativeWindow.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/window/Window.cpp"
)

target_include_directories(vis_module_renderer_opengl_backend
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_renderer_opengl_backend
    PUBLIC glad
           glfw
           glm::glm
           imgui::backend_glfw
           imgui::imgui
           spdlog::spdlog
           stb::image
           vis::traits
           vis::config
           vis::section
           vis::resource
           vis::time
           vis::log
           vis::input
)
