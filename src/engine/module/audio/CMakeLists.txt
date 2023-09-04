include_guard()

add_library(mono_module_audio)
add_library(mono::audio ALIAS mono_module_audio)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_audio
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_audio
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_audio
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioManager.cpp"
)

target_include_directories(mono_module_audio
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_audio
    PUBLIC sfml-audio
           spdlog::spdlog
)

# Copy dynamic libraries (they are SFML's dependencies and it's dynamic because of LGPL2 license)
if(MINGW AND CMAKE_HOST_WIN32) # I hate SFML's CMake files
    set(COMPILER_DIR "$<PATH:REMOVE_FILENAME,${CMAKE_CXX_COMPILER}>")

    add_custom_command(TARGET mono_module_audio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x64/openal32.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libgcc_s_seh-1.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libstdc++-6.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libwinpthread-1.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
    )
endif()