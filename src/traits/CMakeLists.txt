include_guard()

add_library(mono_module_traits INTERFACE)
add_library(mono::traits ALIAS mono_module_traits)

target_include_directories(mono_module_traits
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
