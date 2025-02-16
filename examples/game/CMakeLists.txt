cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOSHOT_GAME
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "A simple 2D top-down shooter game."
    HOMEPAGE_URL "https://github.com/vis4rd/monoshot"
    LANGUAGES C CXX
)

# Engine
add_subdirectory("${PROJECT_SOURCE_DIR}/../../src/engine" "${CMAKE_BINARY_DIR}/src/engine")

# Game
add_executable(monoshot)

set_target_properties(monoshot PROPERTIES
    VS_STARTUP_PROJECT monoshot
    FOLDER "Monoshot"
)

target_compile_definitions(monoshot
    PRIVATE
        $<$<BOOL:${DEBUG_BUILD}>:DEBUG_BUILD>
        $<IF:$<BOOL:${MONOSHOT_VERSION}>,
            MONOSHOT_VERSION="${MONOSHOT_VERSION}",
            MONOSHOT_VERSION="unknown"
        >
)

target_compile_features(monoshot
    PRIVATE "cxx_std_20"
)

target_sources(monoshot
    PRIVATE "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(monoshot
    PUBLIC mono::input
           mono::renderer
           mono::camera
           mono::traits
           mono::log
           mono::time
           mono::config
           mono::dev_ui
           EnTT::EnTT
           glm::glm
           tinyfd::tinyfd
)
