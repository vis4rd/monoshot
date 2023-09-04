cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MONOENGINE LANGUAGES CXX C)

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()
string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "Debug" DEBUG_MODE)

option(MOLD_LINKER_PATH "Path to the mold linker executable (Unix only)" "")

# GLFW
set(GLFW_CLIENT_LIBRARY "opengl" CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glfw")
target_compile_definitions(glfw  # prevent GLFW from including gl headers
    INTERFACE "GLFW_INCLUDE_NONE"
)

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
add_subdirectory("${PROJECT_SOURCE_DIR}/module/renderer")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/camera")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/section")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/traits")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/log")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/ui")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/resource")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/time")
add_subdirectory("${PROJECT_SOURCE_DIR}/module/config")


# Use mold linker if present in the system (to speed up linking time)
# if(MOLD_LINKER)
#     find_program(MOLD_LINKER
#         NAMES "mold"
#         HINTS "/usr/bin"
#             "/usr/libexec/mold"
#             "/usr/local/libexec/mold"
#             "${MOLD_LINKER_PATH}"
#         DOC "Tells if mold linker has been found in the system"
#     )

#     cmake_path(GET MOLD_LINKER ROOT_PATH MOLD_LINKER_PATH)

#     if(CMAKE_HOST_UNIX)
#         # TODO: check whether GCC can finally specify path to the linker in -fuse-ld
#         if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
#             message(STATUS "Found mold linker at '${MOLD_LINKER}', replacing default linker for clang...")
#             target_link_options(${EXECUTABLE_NAME}
#                 PRIVATE "-fuse-ld=${MOLD_LINKER}"
#             )
#         elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
#             message(STATUS "Found mold linker at '${MOLD_LINKER}', replacing default linker for GNU...")
#             target_link_options(${EXECUTABLE_NAME}
#                 PRIVATE "-fuse-ld=mold"
#             )
#         endif()
#     endif()
# endif()

# TODO: figure out folder management for engine tree
# # IDE Folder Management
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# function(target_group_source_files target)
#     get_target_property(sources ${target} SOURCES)
#     foreach(file ${sources})
#         file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})

#         if("${relative_file}" MATCHES ".*\\.cpp")
#             source_group("Source Files" FILES ${relative_file})
#         elseif("${relative_file}" MATCHES ".*\\.hpp")
#             source_group("Header Files" FILES ${relative_file})
#         else()
#             source_group("Other Files" FILES ${relative_file})
#         endif()

#     endforeach()
# endfunction()

# target_group_source_files(${GAME_NAME})
