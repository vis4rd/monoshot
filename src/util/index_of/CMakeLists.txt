include_guard()

add_library(mono_util_indexof INTERFACE)
add_library(mono::util::index_of ALIAS mono_util_indexof)
target_include_directories(mono_util_indexof INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(mono_util_indexof INTERFACE cxx_std_20)
