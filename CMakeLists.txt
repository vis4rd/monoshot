cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Project setup
set(MONOSHOT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MONOSHOT_BINARY_DIR "${MONOSHOT_ROOT_DIR}/build")
set(MONOSHOT_OUTPUT_DIR "${MONOSHOT_ROOT_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MONOSHOT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MONOSHOT_OUTPUT_DIR}")

project(
    MONOSHOT
    VERSION 0.0.4
    DESCRIPTION "A simple engine/framework for experimenting with graphics."
    HOMEPAGE_URL "https://github.com/vis4rd/monoshot"
    LANGUAGES C CXX
)

unset(MONOSHOT_VERSION)
unset(MONOSHOT_VERSION_MAJOR)
unset(MONOSHOT_VERSION_MINOR)
unset(MONOSHOT_VERSION_PATCH)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are forbidden.")
endif()

if(CMAKE_BUILD_TYPE EQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

include(${PROJECT_SOURCE_DIR}/cmake/version.cmake)

# External libraries
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

# Sources
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

# Examples
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples" EXCLUDE_FROM_ALL)
