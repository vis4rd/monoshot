include_guard()

add_library(mono_module_ui)
add_library(mono::ui ALIAS mono_module_ui)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_ui
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_ui
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_ui
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Font.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/external/BeginCombo.cpp"
)

target_include_directories(mono_module_ui
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_ui
    PUBLIC imgui::imgui
           imgui::backend_glfw
           spdlog::spdlog
           mono::config
           mono::renderer
)
