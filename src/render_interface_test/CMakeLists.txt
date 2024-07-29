cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(RENDER_INTERFACE_TEST LANGUAGES C CXX)

# Engine
add_subdirectory("${PROJECT_SOURCE_DIR}/../engine" "${CMAKE_BINARY_DIR}/src/render_interface_test/engine")

# Render Interface Test
add_executable(render_interface_test)

set_target_properties(render_interface_test PROPERTIES
    VS_STARTUP_PROJECT render_interface_test
    FOLDER "Monoshot"
)

# Render Interface Test sources
file(GLOB_RECURSE RENDER_INTERFACE_TEST_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
target_sources(render_interface_test
    PRIVATE ${RENDER_INTERFACE_TEST_SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_compile_features(render_interface_test
    PRIVATE "cxx_std_20"
)

target_link_libraries(render_interface_test
    PRIVATE mono::input
           mono::renderer
           mono::camera
           mono::section
           mono::traits
           mono::log
           mono::ui
           mono::resource
           mono::time
           mono::config
           mono::cstring
           mono::log
           glm::glm
           tinyfd::tinyfd
           spdlog::spdlog
)
