cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(INIFILE-CPP
    HOMEPAGE_URL https://github.com/Rookfighter/inifile-cpp/tree/master
    LANGUAGES CXX
)

add_library(inifile INTERFACE)
add_library(inifile::inifile ALIAS inifile)

target_include_directories(inifile
    INTERFACE "${PROJECT_SOURCE_DIR}/inifile-cpp/include"
)
