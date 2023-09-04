include_guard()

add_library(mono_module_config INTERFACE)
add_library(mono::config ALIAS mono_module_config)

target_compile_features(mono_module_config
    INTERFACE "cxx_std_20"
)

target_include_directories(mono_module_config
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
