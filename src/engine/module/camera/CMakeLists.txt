include_guard()

add_library(vis_module_camera)
add_library(vis::camera ALIAS vis_module_camera)

if(DEBUG_MODE)
    target_compile_definitions(vis_module_camera

        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(vis_module_camera
    PRIVATE "cxx_std_20"
)

target_sources(vis_module_camera
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/PerspectiveCamera.cpp"
)

target_include_directories(vis_module_camera
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(vis_module_camera
    PUBLIC glm
)
