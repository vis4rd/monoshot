cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(RAUDIO
    HOMEPAGE_URL https://www.raylib.com/
    VERSION 1.1
    LANGUAGES CXX
)

option(RAUDIO_SUPPORT_WAV "Define support for .wav files" OFF)
option(RAUDIO_SUPPORT_MP3 "Define support for .mp3 files" ON)
option(RAUDIO_SUPPORT_FLAC "Define support for .flac files" OFF)

add_library(raudio STATIC)
add_library(raylib::audio ALIAS raudio)

target_compile_features(raudio
    PRIVATE "cxx_std_20"
)

target_sources(raudio
    PRIVATE "${PROJECT_SOURCE_DIR}/raudio/src/raudio.c"
)

target_include_directories(raudio
    INTERFACE "${PROJECT_SOURCE_DIR}/raudio/src"
)

target_compile_definitions(raudio
    PRIVATE "RAUDIO_STANDALONE"
            $<$<BOOL:${RAUDIO_SUPPORT_WAV}>:"SUPPORT_FILEFORMAT_WAV">
            $<$<BOOL:${RAUDIO_SUPPORT_MP3}>:"SUPPORT_FILEFORMAT_MP3">
            $<$<BOOL:${RAUDIO_SUPPORT_FLAC}>:"SUPPORT_FILEFORMAT_FLAC">
)

message(STATUS "Raudio: Version ${PROJECT_VERSION}")
