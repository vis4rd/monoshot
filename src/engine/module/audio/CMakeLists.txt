include_guard()

add_library(vis_module_audio)
add_library(vis::audio ALIAS vis_module_audio)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_audio
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_audio
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_audio
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/audio/AudioManager.hpp"
)

target_include_directories(vis_module_audio
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_audio
    PUBLIC sfml-audio
           spdlog::spdlog
)
