include_guard()

add_library(mono_module_renderer_opengl_backend)
add_library(mono::renderer_opengl ALIAS mono_module_renderer_opengl_backend)

message(STATUS "Renderer: using OpenGL backend")

if(DEBUG_MODE)
    target_compile_definitions(mono_module_renderer_opengl_backend
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_renderer_opengl_backend
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_renderer_opengl_backend
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShaderAttribute.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ElementBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/FrameBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShaderAttributeLayout.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexArray.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexBuffer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/RenderPass.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/RenderPipeline.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/Renderer.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/Shader.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/ShaderManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shader/ShaderProgram.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/target/RenderTarget.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/target/RenderWindow.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/texture/Animation.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/texture/Texture.cpp"
)

target_include_directories(mono_module_renderer_opengl_backend
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_renderer_opengl_backend
    PUBLIC glad
           glfw
           glm::glm
           imgui::backend_glfw
           imgui::imgui
           spdlog::spdlog
           stb::image
           mono::traits
           mono::config
           mono::section
           mono::resource
           mono::time
           mono::log
           mono::input
           mono::cstring
)

# if(CMAKE_HOST_WIN32)
#     target_link_libraries(mono_module_renderer_opengl_backend
#         PUBLIC opengl32
#                comdlg32
#                ole32
#     )
#     message(STATUS "WIN32 system detected.")
# elseif(CMAKE_HOST_UNIX)
#     target_link_libraries(mono_module_renderer_opengl_backend
#         PUBLIC GL
#                X11
#                pthread
#                Xrandr
#                Xi
#                dl
#     )
#     message(STATUS "UNIX system detected.")
# endif()
