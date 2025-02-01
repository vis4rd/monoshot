include_guard()

add_library(mono_util_custom_imgui INTERFACE)
add_library(mono::util::custom_imgui ALIAS mono_util_custom_imgui)
target_include_directories(mono_util_custom_imgui INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(mono_util_custom_imgui INTERFACE cxx_std_20)
