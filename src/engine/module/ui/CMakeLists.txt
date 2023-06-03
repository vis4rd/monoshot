include_guard()

add_library(vis_module_ui)
add_library(vis::ui ALIAS vis_module_ui)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_ui
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_ui
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_ui
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Font.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/external/BeginCombo.cpp"
)

target_include_directories(vis_module_ui
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_ui
    PUBLIC imgui::imgui
           imgui::backend_glfw
           spdlog::spdlog
           vis::config
           vis::renderer
)
