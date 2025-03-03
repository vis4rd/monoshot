cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOSHOT_EXAMPLE_PLAYGROUND LANGUAGES C CXX)

add_executable(example_playground)

file(GLOB_RECURSE PLAYGROUND_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
target_sources(example_playground
    PRIVATE "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_compile_features(example_playground
    PRIVATE "cxx_std_20"
)

target_link_libraries(example_playground
    PRIVATE
    mono::input
    mono::renderer
    mono::camera
    mono::traits
    mono::log
    mono::time
    mono::config
    mono::log
    mono::external::glm
    mono::external::tinyfd
    mono::external::spdlog
)
