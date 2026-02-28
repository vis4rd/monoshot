include_guard()

add_library(mono_module_common INTERFACE)
add_library(mono::common ALIAS mono_module_common)

# Below settings will be inherited by all modules that link to mono::common.
target_compile_features(mono_module_common
    INTERFACE "cxx_std_20"
)

set(GLOBALS_HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Globals.hpp")

# Include the Globals.hpp header file in all translation units that link to mono::common.
# This gives access to utilities defined here without explicitly including the header in each file.
target_compile_options(mono_module_common
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/FI"${GLOBALS_HEADER_FILE}">
    $<$<CXX_COMPILER_ID:GNU,Clang>:-include "${GLOBALS_HEADER_FILE}" >
)
