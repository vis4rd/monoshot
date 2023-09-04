include_guard()

add_subdirectory(opengl)

add_library(mono_module_renderer INTERFACE)
add_library(mono::renderer ALIAS mono_module_renderer)

target_link_libraries(mono_module_renderer
    INTERFACE mono::renderer_opengl
)
