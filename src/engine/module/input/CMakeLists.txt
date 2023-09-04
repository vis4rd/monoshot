include_guard()

add_library(mono_module_input)
add_library(mono::input ALIAS mono_module_input)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_input
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_input
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_input
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/InputManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Keybind.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/KeyGroup.cpp"
)

target_include_directories(mono_module_input
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_input
    PUBLIC glfw
           spdlog::spdlog
)
