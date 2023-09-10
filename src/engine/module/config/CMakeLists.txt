include_guard()

add_library(mono_module_config)
add_library(mono::config ALIAS mono_module_config)

target_compile_features(mono_module_config
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_config
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigLoader.cpp"
)

target_include_directories(mono_module_config
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_config
    PUBLIC inifile::inifile
           spdlog::spdlog
)
