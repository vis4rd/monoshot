cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(EXECUTABLE_NAME "main")
project(OPENGL_TEMPLATE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glfw")

# OpenGL
find_package(OpenGL 4 REQUIRED)

# GLAD
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glad")
# set(GLAD_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/lib/glad/include")
# set(GLAD_SOURCE "${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c")

add_executable(${EXECUTABLE_NAME})

target_sources(${EXECUTABLE_NAME}
    PRIVATE main.cpp)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE glfw
            glad)

if(CMAKE_HOST_WIN32)
    target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE opengl32
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