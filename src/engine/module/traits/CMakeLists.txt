include_guard()

add_library(vis_module_traits)
add_library(vis::traits ALIAS vis_module_traits)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_traits
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_traits
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_traits
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderable.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Updateable.cpp"
)

target_include_directories(vis_module_traits
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_traits
    PUBLIC spdlog::spdlog
)
