cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(MONOENGINE LANGUAGES CXX C)

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "Debug" DEBUG_BUILD)

option(MOLD_LINKER_PATH "Path to the mold linker executable (Unix only)" "")

# GLFW
set(GLFW_CLIENT_LIBRARY "opengl" CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/glfw" "${PROJECT_BINARY_DIR}/../lib/glfw" SYSTEM)
target_compile_definitions(glfw  # prevent GLFW from including gl headers
    INTERFACE "GLFW_INCLUDE_NONE"
)

# OpenGL
find_package(OpenGL 4.5 REQUIRED)

# GLAD
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/glad/cmake" "${PROJECT_BINARY_DIR}/../lib/glad" SYSTEM)
glad_add_library(glad STATIC REPRODUCIBLE
    LANGUAGE C
    API gl:core=4.5
)

# GLM
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/glm" "${PROJECT_BINARY_DIR}/../lib/glm" SYSTEM)

# ImGUI
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/imgui" "${PROJECT_BINARY_DIR}/../lib/imgui" SYSTEM)

# spdlog
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)
set(SPDLOG_SYSTEM_INCLUDES ON CACHE BOOL "" FORCE)
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/spdlog" "${PROJECT_BINARY_DIR}/../lib/spdlog" SYSTEM)
target_compile_definitions(spdlog PUBLIC SPDLOG_SOURCE_LOCATION)

# stb_image
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/stb_image" "${PROJECT_BINARY_DIR}/../lib/stb_image" SYSTEM)

# EnTT
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/entt" "${PROJECT_BINARY_DIR}/../lib/entt" SYSTEM)

# tinyfiledialogs
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/tinyfiledialogs" "${PROJECT_BINARY_DIR}/../lib/tinyfiledialogs" SYSTEM)

# inifile-cpp
add_subdirectory("${PROJECT_SOURCE_DIR}/../lib/inifile-cpp" "${PROJECT_BINARY_DIR}/../lib/inifile-cpp" SYSTEM)

# Modules
add_subdirectory("${PROJECT_SOURCE_DIR}/input")
add_subdirectory("${PROJECT_SOURCE_DIR}/renderer")
add_subdirectory("${PROJECT_SOURCE_DIR}/camera")
add_subdirectory("${PROJECT_SOURCE_DIR}/traits")
add_subdirectory("${PROJECT_SOURCE_DIR}/log")
add_subdirectory("${PROJECT_SOURCE_DIR}/time")
add_subdirectory("${PROJECT_SOURCE_DIR}/config")
add_subdirectory("${PROJECT_SOURCE_DIR}/util")
add_subdirectory("${PROJECT_SOURCE_DIR}/dev_ui")


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
