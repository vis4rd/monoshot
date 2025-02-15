cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(PLAYGROUND LANGUAGES C CXX)

# Engine
add_subdirectory("${PROJECT_SOURCE_DIR}/../engine" "${CMAKE_BINARY_DIR}/src/playground/engine")

# Playground
add_executable(playground)

set_target_properties(playground PROPERTIES
    VS_STARTUP_PROJECT playground
    FOLDER "Monoshot"
)

# Playground sources
file(GLOB_RECURSE PLAYGROUND_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
target_sources(playground
    PRIVATE ${PLAYGROUND_SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_compile_features(playground
    PRIVATE "cxx_std_20"
)

target_link_libraries(playground
    PRIVATE mono::input
           mono::renderer
           mono::camera
           mono::traits
           mono::log
           mono::time
           mono::config
           mono::log
           glm::glm
           tinyfd::tinyfd
           spdlog::spdlog
)
