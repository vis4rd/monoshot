include_guard()

add_library(vis_module_input)
add_library(vis::input ALIAS vis_module_input)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_input
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_input
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_input
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/InputManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Keybind.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/KeyGroup.cpp"
)

target_include_directories(vis_module_input
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_input
    PUBLIC glfw
           spdlog::spdlog
)
