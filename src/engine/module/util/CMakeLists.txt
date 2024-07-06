include_guard()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/packed_variable")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/index_of")

add_library(mono_module_util INTERFACE)
add_library(mono::util ALIAS mono_module_util)

target_link_libraries(mono_module_util INTERFACE
    mono::util::pv
    mono::util::index_of
)
