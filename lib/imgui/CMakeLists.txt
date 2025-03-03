cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(IMGUI)

# Main Library
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_compile_features(imgui
    PRIVATE "cxx_std_20"
)

target_sources(imgui
    PRIVATE
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    PRIVATE
    "${PROJECT_SOURCE_DIR}/imgui/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/imgui_demo.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)

target_include_directories(imgui
    PUBLIC "${PROJECT_SOURCE_DIR}/imgui"
    INTERFACE "${PROJECT_SOURCE_DIR}"
)
target_link_libraries(imgui
    PUBLIC mono::external::glfw
)
