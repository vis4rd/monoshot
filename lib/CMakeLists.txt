# GLFW
set(GLFW_CLIENT_LIBRARY "opengl" CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_WAYLAND ON CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glfw" SYSTEM)
target_compile_definitions(glfw # prevent GLFW from including gl headers
    INTERFACE "GLFW_INCLUDE_NONE"
)

# OpenGL
find_package(OpenGL 4.5 REQUIRED)

# glbindings
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glbinding" SYSTEM)

# GLM
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/glm" SYSTEM)

# ImGUI
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/imgui" SYSTEM)

# spdlog
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "" FORCE)
set(SPDLOG_SYSTEM_INCLUDES ON CACHE BOOL "" FORCE)
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/spdlog" SYSTEM)
target_compile_definitions(spdlog PUBLIC SPDLOG_SOURCE_LOCATION)

# stb_image
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/stb_image" SYSTEM)

# EnTT
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/entt" SYSTEM)

# tinyfiledialogs
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tinyfiledialogs" SYSTEM)

# inifile-cpp
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/inifile-cpp" SYSTEM)

# ##################################################################################################

# Alias external libraries
add_library(mono::external::glfw ALIAS glfw)
add_library(mono::external::glbinding ALIAS glbinding)
add_library(mono::external::glm ALIAS glm)
add_library(mono::external::imgui ALIAS imgui)
add_library(mono::external::spdlog ALIAS spdlog)
add_library(mono::external::stb_image ALIAS stb_image)
add_library(mono::external::entt ALIAS EnTT)
add_library(mono::external::tinyfd ALIAS tinyfd)
add_library(mono::external::inifile ALIAS inifile)
