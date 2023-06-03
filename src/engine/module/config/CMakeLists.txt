include_guard()

add_library(vis_module_config INTERFACE)
add_library(vis::config ALIAS vis_module_config)

target_compile_features(vis_module_config
    INTERFACE "cxx_std_20"
)

target_include_directories(vis_module_config
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
