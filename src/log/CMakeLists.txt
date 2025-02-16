include_guard()

add_library(mono_module_log)
add_library(mono::log ALIAS mono_module_log)

if(DEBUG_BUILD)
    target_compile_definitions(mono_module_log
        PRIVATE "DEBUG_BUILD"
    )
endif()

target_compile_features(mono_module_log
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_log
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Logging.cpp"
)

target_include_directories(mono_module_log
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
)

target_link_libraries(mono_module_log
    PUBLIC spdlog::spdlog
           glad
           mono::config
           mono::util::compiler
)
