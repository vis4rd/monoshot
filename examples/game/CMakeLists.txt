cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOSHOT_EXAMPLE_GAME LANGUAGES C CXX)

add_executable(example_game)

target_compile_definitions(example_game
    PRIVATE
    $<$<BOOL:${DEBUG_BUILD}>:DEBUG_BUILD>
    $<IF:$<BOOL:${MONOSHOT_VERSION}>,
    MONOSHOT_VERSION="${MONOSHOT_VERSION}",
    MONOSHOT_VERSION="unknown"
    >
)

target_compile_features(example_game
    PRIVATE "cxx_std_20"
)

target_sources(example_game
    PRIVATE "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(example_game
    PUBLIC
    mono::input
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
