include_guard()

add_library(mono_util_compiler INTERFACE)
add_library(mono::util::compiler ALIAS mono_util_compiler)
target_include_directories(mono_util_compiler INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(mono_util_compiler INTERFACE cxx_std_20)
