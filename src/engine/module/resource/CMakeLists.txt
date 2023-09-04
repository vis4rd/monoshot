include_guard()

add_library(mono_module_resource)
add_library(mono::resource ALIAS mono_module_resource)

target_compile_features(mono_module_resource
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_resource
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/ResourceManager.cpp"
)

target_include_directories(mono_module_resource
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_resource
    PUBLIC glm::glm
           mono::renderer
           mono::time
           mono::ui
)