include_guard()

add_library(vis_module_time)
add_library(vis::time ALIAS vis_module_time)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_time
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_time
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_time
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/FramerateLimiter.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Timer.cpp"
)

target_include_directories(vis_module_time
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_time
    PUBLIC glfw
           spdlog::spdlog
           vis::resource
)
