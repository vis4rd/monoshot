include_guard()

add_library(mono_module_config)
add_library(mono::config ALIAS mono_module_config)

target_sources(mono_module_config
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigItem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigStorage.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CallbackGuard.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/priv/DevUiExtension.cpp"
)

target_include_directories(mono_module_config
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mono"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_config
    PUBLIC
    mono::external::inifile
    mono::external::spdlog
    mono::external::glm
    mono::external::imgui
    mono::common
    mono::traits
    mono::log
    mono::dev_ui
    mono::util
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Monoshot: DEBUG build")
    target_compile_definitions(mono_module_config
        PUBLIC "DEBUG_BUILD"
    )
endif()
