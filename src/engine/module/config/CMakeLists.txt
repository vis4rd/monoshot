include_guard()

add_library(mono_module_config)
add_library(mono::config ALIAS mono_module_config)

target_compile_features(mono_module_config
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_config
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigItem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigStorage.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/CallbackGuard.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dev_ui/DevUIConfigWindow.cpp"
)

target_include_directories(mono_module_config
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_config
    PUBLIC inifile::inifile
           spdlog::spdlog
           glm::glm
           imgui::imgui
           mono::traits
           mono::log
           mono::dev_ui
)
