include_guard()

add_library(vis_module_section)
add_library(vis::section ALIAS vis_module_section)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_section
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_section
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_section
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/SectionManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Section.cpp"
)

target_include_directories(vis_module_section
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_section
    PUBLIC spdlog::spdlog
           vis::traits
           vis::audio
           vis::resource
)
