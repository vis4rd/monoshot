include_guard()

add_library(mono_module_section)
add_library(mono::section ALIAS mono_module_section)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_section
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_section
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_section
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/SectionManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Section.cpp"
)

target_include_directories(mono_module_section
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_section
    PUBLIC spdlog::spdlog
           mono::traits
           mono::audio
           mono::resource
)
