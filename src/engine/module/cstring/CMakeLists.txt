include_guard()

add_library(mono_module_cstring INTERFACE)
add_library(mono::cstring ALIAS mono_module_cstring)

target_compile_features(mono_module_cstring
    INTERFACE "cxx_std_20"
)

target_include_directories(mono_module_cstring
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
