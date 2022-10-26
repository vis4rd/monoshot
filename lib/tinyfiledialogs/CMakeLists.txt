cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(TINYFILEDIALOGS
    HOMEPAGE_URL https://sourceforge.net/projects/tinyfiledialogs/
    LANGUAGES CXX
)

add_library(tinyfd STATIC)

target_compile_features(tinyfd
    PRIVATE "cxx_std_20"
)

target_sources(tinyfd
    PRIVATE "${PROJECT_SOURCE_DIR}/tinyfiledialogs/tinyfiledialogs.c"
)

target_include_directories(tinyfd
    INTERFACE "${PROJECT_SOURCE_DIR}"
)
