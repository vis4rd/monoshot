cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

# Project setup
project(OPENGL_TEMPLATE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(EXECUTABLE_NAME "main")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glfw")

# OpenGL
find_package(OpenGL 4 REQUIRED)

# GLAD
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glad")

# GLM
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glm")

# ImGUI
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/imgui")

# spdlog
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/spdlog")

# Other sources
file(GLOB_RECURSE SOURCES "src/*.cpp")


# App
add_executable(${EXECUTABLE_NAME})

target_compile_features(${EXECUTABLE_NAME}
    PRIVATE "cxx_std_20")

target_sources(${EXECUTABLE_NAME}
    PRIVATE ${SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp")

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC glfw
           glad
           glm::glm
           imgui
           imgui_backend
           spdlog::spdlog
)

target_precompile_headers(${EXECUTABLE_NAME}
    PRIVATE "${PROJECT_SOURCE_DIR}/include/Root.hpp")

if(CMAKE_HOST_WIN32)
    target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC opengl32
           comdlg32
           ole32)
    message(STATUS "WIN32 system detected.")
elseif(CMAKE_HOST_UNIX)
    target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE GL
            X11
            pthread
            Xrandr
            Xi
            dl)
    message(STATUS "UNIX system detected.")
else()
    message(STATUS "Unknown system detected.")
endif()

# Versioning
add_custom_command(
    TARGET ${EXECUTABLE_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_SOURCE_DIR}/meta/ProjectVersioning.cmake"
)
