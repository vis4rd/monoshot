cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(IMGUI)

# Main Library
add_library(imgui)

target_compile_features(imgui
    PRIVATE "cxx_std_20"
)

target_sources(imgui
    PRIVATE "${PROJECT_SOURCE_DIR}/imgui/imgui.cpp"
            "${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp"
            "${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp"
            "${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp"
            "${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)

target_include_directories(imgui
    INTERFACE "${PROJECT_SOURCE_DIR}"
)

# Support Backend
add_library(imgui_backend STATIC)

target_compile_features(imgui_backend
    PRIVATE "cxx_std_20"
)

target_sources(imgui_backend
    PRIVATE "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
            "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui_backend
    PUBLIC "${PROJECT_SOURCE_DIR}/imgui"
    INTERFACE "${PROJECT_SOURCE_DIR}/imgui/backends"
)

target_link_libraries(imgui_backend
    PRIVATE glfw
)

# add_library(IMGUI_SDL STATIC)
# target_compile_features(IMGUI_SDL
#     PRIVATE
#         "cxx_std_20")
# target_sources(IMGUI_SDL
#     PRIVATE
#         "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl.cpp"
#         "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer.cpp"
# )
# target_include_directories(IMGUI_SDL
#     PRIVATE
#         "${PROJECT_SOURCE_DIR}/imgui"
#     PUBLIC
#         "${PROJECT_SOURCE_DIR}/imgui/backends"
# )
# target_link_libraries(IMGUI_SDL
#     PRIVATE
#         m
#         SDL2::Core
# )
# add_library(IMGUI::SDL2 ALIAS IMGUI_SDL)
