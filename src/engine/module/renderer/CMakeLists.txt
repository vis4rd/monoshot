include_guard()

add_subdirectory(opengl)

add_library(mono_module_renderer)
add_library(mono::renderer ALIAS mono_module_renderer)

target_sources(mono_module_renderer
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp")

target_include_directories(mono_module_renderer
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(mono_module_renderer
    PUBLIC glm::glm
           mono::renderer_opengl
)
