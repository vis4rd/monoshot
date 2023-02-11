include_guard()

add_library(vis_module_cstring INTERFACE)
add_library(vis::cstring ALIAS vis_module_cstring)

target_compile_features(vis_module_cstring
    INTERFACE "cxx_std_20"
)

target_include_directories(vis_module_cstring
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
