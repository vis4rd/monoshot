include_guard()

add_library(vis_module_resource)
add_library(vis::resource ALIAS vis_module_resource)

target_compile_features(vis_module_resource
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_resource
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceManager.cpp"
)

target_include_directories(vis_module_resource
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_resource
    PUBLIC glm::glm
           vis::renderer
           vis::time
           vis::ui
)