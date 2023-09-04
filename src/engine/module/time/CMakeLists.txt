include_guard()

add_library(mono_module_time)
add_library(mono::time ALIAS mono_module_time)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_time
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_time
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_time
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/FramerateLimiter.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Timer.cpp"
)

target_include_directories(mono_module_time
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_time
    PUBLIC glfw
           spdlog::spdlog
           mono::resource
)
