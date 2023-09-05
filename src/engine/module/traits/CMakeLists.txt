include_guard()

add_library(mono_module_traits INTERFACE)
add_library(mono::traits ALIAS mono_module_traits)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_traits
        INTERFACE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_traits
    INTERFACE "cxx_std_20"
)

target_include_directories(mono_module_traits
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
