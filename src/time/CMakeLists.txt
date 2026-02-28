include_guard()

add_library(mono_module_time)
add_library(mono::time ALIAS mono_module_time)

target_sources(mono_module_time
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Timer.cpp"
)

target_include_directories(mono_module_time
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_time
    PUBLIC
    mono::external::glfw
    mono::external::spdlog
    mono::common
)
