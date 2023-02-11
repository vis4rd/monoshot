cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOSHOT_GAME LANGUAGES CXX C)

set(GAME_NAME "Monoshot")
set(GLOBAL_BINARY_DIR "${PROJECT_SOURCE_DIR}/../../build/engine/lib")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()
string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "Debug" DEBUG_MODE)

option(MOLD_LINKER_PATH "Path to the mold linker executable" "")

# Use mold linker if present in the system (to speed up linking time)
find_program(MOLD_LINKER
    NAMES "mold"
    HINTS "/usr/bin"
          "/usr/libexec/mold"
          "/usr/local/libexec/mold"
          "${MOLD_LINKER_PATH}"
    DOC "Tells if mold linker has been found in the system"
)

if(MOLD_LINKER)
    cmake_path(GET MOLD_LINKER ROOT_PATH MOLD_LINKER_PATH)
endif()

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
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/stb_image")

# EnTT
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/entt")

# tinyfiledialogs
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/tinyfiledialogs")

# SFML
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(SFML_ENABLE_COVERAGE OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL_PKGCONFIG_FILES OFF CACHE BOOL "" FORCE)
set(SFML_ENABLE_PCH ON CACHE BOOL "" FORCE)
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_FRAMEWORKS OFF CACHE BOOL "" FORCE)
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/sfml")

# Modules
add_subdirectory("${PROJECT_SOURCE_DIR}/module/input")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/cstring")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/audio")

# Other sources
file(GLOB_RECURSE GAME_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

# Game
add_executable(${GAME_NAME})

set_target_properties(${GAME_NAME} PROPERTIES
    VS_STARTUP_PROJECT ${GAME_NAME}
    FOLDER "App"
)

if(DEBUG_MODE)
    target_compile_definitions(${GAME_NAME}
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(${GAME_NAME}
    PRIVATE "cxx_std_20"
)

target_sources(${GAME_NAME}
    PRIVATE ${GAME_SOURCES}
            "${PROJECT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(${GAME_NAME}
    PUBLIC glfw
           glad
           glm::glm
           imgui::imgui
           imgui::backend_glfw
           spdlog::spdlog
           stb::image
           EnTT::EnTT
           tinyfd::tinyfd
           vis::input
           vis::audio
)

target_precompile_headers(${GAME_NAME}
    PRIVATE "${PROJECT_SOURCE_DIR}/include/Root.hpp"
)

if(MOLD_LINKER AND CMAKE_HOST_UNIX)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        message(STATUS "Found mold linker at '${MOLD_LINKER}', replacing default linker for clang...")
        target_link_options(${EXECUTABLE_NAME}
            PRIVATE "-fuse-ld=${MOLD_LINKER}"
        )
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        message(STATUS "Found mold linker at '${MOLD_LINKER}', replacing default linker for GNU...")
        target_link_options(${EXECUTABLE_NAME}
            PRIVATE "-fuse-ld=mold"
        )
    endif()
endif()

if(CMAKE_HOST_WIN32)
    target_link_libraries(${GAME_NAME}
    PUBLIC opengl32
           comdlg32
           ole32)
    message(STATUS "WIN32 system detected.")
elseif(CMAKE_HOST_UNIX)
    target_link_libraries(${GAME_NAME}
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

# Copy dynamic libraries (they are SFML's dependencies and it's dynamic because of LGPL2 license)
if(MINGW AND CMAKE_HOST_WIN32) # I hate SFML's CMake files
    set(COMPILER_DIR "$<PATH:REMOVE_FILENAME,${CMAKE_CXX_COMPILER}>")

    add_custom_command(TARGET ${GAME_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/lib/sfml/extlibs/bin/x64/openal32.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libgcc_s_seh-1.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libstdc++-6.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMAND ${CMAKE_COMMAND} -E copy "${COMPILER_DIR}/libwinpthread-1.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
    )
endif()

# Versioning
add_custom_command(
    TARGET ${GAME_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_SOURCE_DIR}/../../meta/ProjectVersioning.cmake"
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

target_group_source_files(${GAME_NAME})
