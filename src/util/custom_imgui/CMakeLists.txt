include_guard()

add_library(mono_util_custom_imgui)
add_library(mono::util::custom_imgui ALIAS mono_util_custom_imgui)

target_sources(mono_util_custom_imgui
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/BeginCombo.cpp"
)

target_include_directories(mono_util_custom_imgui
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
)

target_link_libraries(mono_util_custom_imgui
    PUBLIC
    mono::external::imgui
    mono::common
)
