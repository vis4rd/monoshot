cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOSHOT_GAME LANGUAGES CXX C)

# Engine
add_subdirectory("${PROJECT_SOURCE_DIR}/../engine" "${CMAKE_BINARY_DIR}/engine")

# Game
add_executable(monoshot)

set_target_properties(monoshot PROPERTIES
    VS_STARTUP_PROJECT monoshot
    FOLDER "Monoshot"
)

target_compile_definitions(monoshot
    PRIVATE $<$<BOOL:${DEBUG_MODE}>:DEBUG_MODE>
)

# Game sources
file(GLOB_RECURSE GAME_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

target_compile_features(monoshot
    PRIVATE "cxx_std_20"
)

target_sources(monoshot
    PRIVATE ${GAME_SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(monoshot
    PUBLIC mono::input
           mono::audio
           mono::renderer
           mono::camera
           mono::section
           mono::traits
           mono::log
           mono::ui
           mono::resource
           mono::time
           mono::config
)

# Versioning
add_custom_command(
    TARGET monoshot PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_SOURCE_DIR}/../../meta/ProjectVersioning.cmake"
)