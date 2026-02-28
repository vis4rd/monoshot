include_guard()

add_library(mono_module_camera)
add_library(mono::camera ALIAS mono_module_camera)

target_sources(mono_module_camera
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PerspectiveCamera.cpp"
)

target_include_directories(mono_module_camera
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_camera
    PUBLIC
    mono::common
    mono::external::glm
)
