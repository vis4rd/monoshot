include_guard()

add_library(vis_module_log)
add_library(vis::log ALIAS vis_module_log)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_log
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_log
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_log
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Logging.cpp"
)

target_include_directories(vis_module_log
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_log
    PUBLIC spdlog::spdlog
           glad
)
