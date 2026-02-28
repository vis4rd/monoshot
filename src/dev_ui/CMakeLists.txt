include_guard()

add_library(mono_module_dev_ui)
add_library(mono::dev_ui ALIAS mono_module_dev_ui)

target_sources(mono_module_dev_ui
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/DevUI.cpp"
)

target_include_directories(mono_module_dev_ui
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
)

target_link_libraries(mono_module_dev_ui
    PUBLIC
    mono::common
    mono::external::imgui
    mono::external::spdlog
    mono::config
)
