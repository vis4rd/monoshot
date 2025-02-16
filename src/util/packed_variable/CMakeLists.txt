include_guard()

add_library(mono_util_pv INTERFACE)
add_library(mono::util::pv ALIAS mono_util_pv)

if(DEBUG_BUILD)
    target_compile_definitions(mono_util_pv
        INTERFACE "DEBUG_BUILD"
    )
endif()

target_include_directories(mono_util_pv INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(mono_util_pv INTERFACE cxx_std_20)
