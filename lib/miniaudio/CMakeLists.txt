cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(MINIAUDIO
    HOMEPAGE_URL https://miniaud.io/
    LANGUAGES C
)

option(MA_NO_RUNTIME_LINKING "Disables runtime linking" OFF)
option(MA_DEBUG_OUTPUT "Enable `printf()` output of debug logs (`MA_LOG_LEVEL_DEBUG`)" OFF)

# Generate dummy source to compile into library
set(MINIAUDIO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(MINIAUDIO_SOURCE_FILE "${MINIAUDIO_SOURCE_DIR}/miniaudio_implementation.c")
if(NOT EXISTS ${MINIAUDIO_SOURCE_FILE})
    file(WRITE ${MINIAUDIO_SOURCE_FILE}
"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
"
    )
endif()

add_library(miniaudio STATIC)
add_library(miniaudio::miniaudio ALIAS miniaudio)

target_compile_features(miniaudio
    PRIVATE "c_std_17"
)

target_sources(miniaudio
    PRIVATE "${MINIAUDIO_SOURCE_FILE}"
)

target_include_directories(miniaudio
    PRIVATE "${PROJECT_SOURCE_DIR}/miniaudio"
    INTERFACE "${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(miniaudio
    PRIVATE $<$<BOOL:${MA_NO_RUNTIME_LINKING}>:"MA_NO_RUNTIME_LINKING">
            $<$<BOOL:${MA_DEBUG_OUTPUT}>:"MA_DEBUG_OUTPUT">
)

if(CMAKE_HOST_UNIX)
    target_link_libraries(miniaudio
        PRIVATE pthread
                dl
                m
    )
endif()

message(STATUS "Miniaudio: Version <unknown>")
