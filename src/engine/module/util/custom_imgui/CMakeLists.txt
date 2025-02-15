include_guard()

add_library(mono_util_custom_imgui)
add_library(mono::util::custom_imgui ALIAS mono_util_custom_imgui)
target_sources(mono_util_custom_imgui
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/BeginCombo.cpp"
)
target_include_directories(mono_util_custom_imgui INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(mono_util_custom_imgui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono/util/custom_imgui")
target_compile_features(mono_util_custom_imgui PUBLIC cxx_std_20)
target_link_libraries(mono_util_custom_imgui PUBLIC imgui::imgui)
