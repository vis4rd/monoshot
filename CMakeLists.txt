cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

# Project setup
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

project(MONOSHOT)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are forbidden.")
endif()

set(EXECUTABLE_NAME "Monoshot")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "Debug" DEBUG_MODE)

include(FetchContent)

# GLFW
set(GLFW_CLIENT_LIBRARY "opengl" CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glfw")

# OpenGL
find_package(OpenGL 4.5 REQUIRED)

# GLAD
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glad/cmake")
glad_add_library(glad STATIC REPRODUCIBLE
    LANGUAGE C
    API gl:core=4.5
)

# GLM
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glm")

# ImGUI
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/imgui")

# spdlog
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/spdlog")

# stb_image
set(STB_IMAGE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/lib/stb_image")
set(STB_IMAGE_SOURCE_PATH "${STB_IMAGE_SOURCE_DIR}/stb_image.h")
file(MAKE_DIRECTORY ${STB_IMAGE_SOURCE_DIR})

if(NOT EXISTS ${STB_IMAGE_SOURCE_PATH})
    file(DOWNLOAD
        "https://raw.githubusercontent.com/nothings/stb/dev/stb_image.h"
        ${STB_IMAGE_SOURCE_PATH}
        INACTIVITY_TIMEOUT 5
        TIMEOUT 10
        STATUS STB_IMAGE_DOWNLOAD_STATUS
    )
    list(GET STB_IMAGE_DOWNLOAD_STATUS 0 STB_IMAGE_NOT_FOUND)
    list(GET STB_IMAGE_DOWNLOAD_STATUS 1 STB_IMAGE_DOWNLOAD_ERROR_MESSAGE)
    if(STB_IMAGE_NOT_FOUND)
        message(SEND_ERROR ${STB_IMAGE_DOWNLOAD_ERROR_MESSAGE})
    else()
        message(STATUS "Library stb_image fetched from https://raw.githubusercontent.com/nothings/stb/dev/stb_image.h")
    endif()
else()
    message(STATUS "Library stb_image found at ${STB_IMAGE_SOURCE_PATH}")
    set(STB_IMAGE_NOT_FOUND FALSE)
endif()

set(STB_IMAGE_FOUND (NOT STB_IMAGE_NOT_FOUND))


set(STB_IMAGE_FILE_SOURCE "${STB_IMAGE_SOURCE_DIR}/stb_image.c")
if(NOT EXISTS ${STB_IMAGE_FILE_SOURCE})
    file(WRITE ${STB_IMAGE_FILE_SOURCE}
"#define STB_IMAGE_IMPLEMENTATION
#include \"stb_image.h\"
    ")
endif()

add_library(stbi OBJECT STATIC)
target_include_directories(stbi INTERFACE ${STB_IMAGE_SOURCE_DIR})
target_sources(stbi PRIVATE ${STB_IMAGE_FILE_SOURCE})

# EnTT
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/entt")

# tinyfiledialogs
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/tinyfiledialogs")

# Other sources
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

# App
add_executable(${EXECUTABLE_NAME})

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build"
    VS_STARTUP_PROJECT ${EXECUTABLE_NAME}
    FOLDER "App"
)

if(DEBUG_MODE)
    target_compile_definitions(${EXECUTABLE_NAME}
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(${EXECUTABLE_NAME}
    PRIVATE "cxx_std_20"
)

target_sources(${EXECUTABLE_NAME}
    PRIVATE ${SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC glfw
           glad
           glm::glm
           imgui::imgui
           imgui::backend_glfw
           spdlog::spdlog
           stbi
           EnTT::EnTT
           tinyfd::tinyfd
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

# IDE Folder Management
function(target_group_source_files target)
    get_target_property(sources ${target} SOURCES)
    foreach(file ${sources})
        file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})

        if("${relative_file}" MATCHES ".*\\.cpp")
            source_group("Source Files" FILES ${relative_file})
        elseif("${relative_file}" MATCHES ".*\\.hpp")
            source_group("Header Files" FILES ${relative_file})
        else()
            source_group("Other Files" FILES ${relative_file})
        endif()

    endforeach()
endfunction()

target_group_source_files(${EXECUTABLE_NAME})
