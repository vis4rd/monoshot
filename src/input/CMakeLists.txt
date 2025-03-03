include_guard()

add_library(mono_module_input)
add_library(mono::input ALIAS mono_module_input)

if(DEBUG_BUILD)
    target_compile_definitions(mono_module_input
        PRIVATE "DEBUG_BUILD"
    )
endif()

target_compile_features(mono_module_input
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_input
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FakeInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Input.cpp"
)

target_include_directories(mono_module_input
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_input
    PUBLIC
    mono::external::glfw
    mono::external::spdlog
    mono::external::imgui
    mono::config
)
