include_guard()

add_library(mono_module_renderer_opengl_backend)
add_library(mono::renderer_opengl ALIAS mono_module_renderer_opengl_backend)

message(STATUS "Renderer: using OpenGL backend")

target_compile_features(mono_module_renderer_opengl_backend
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_renderer_opengl_backend
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ElementBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/FrameBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShaderAttribute.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShaderAttributeLayout.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShaderStorageBufferAny.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/VertexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glfw/RenderWindowUserStorage.cpp"
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
    PUBLIC
    mono::external::glad
    mono::external::glfw
    mono::external::glm
    mono::external::imgui
    mono::external::spdlog
    mono::external::stb_image
    mono::traits
    mono::config
    mono::time
    mono::log
    mono::input
    mono::util::pv
    mono::util::index_of
    mono::util::custom_imgui
)
