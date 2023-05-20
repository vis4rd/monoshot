include_guard()

add_subdirectory(opengl)

add_library(vis_module_renderer INTERFACE)
add_library(vis::renderer ALIAS vis_module_renderer)

target_link_libraries(vis_module_renderer
    INTERFACE vis::renderer_opengl
)
