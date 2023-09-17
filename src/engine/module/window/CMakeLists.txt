include_guard()

add_library(mono_module_window)
add_library(mono::window ALIAS mono_module_window)

if(DEBUG_MODE)
    target_compile_definitions(mono_module_window
        PRIVATE "DEBUG_MODE"
    )
endif()

target_compile_features(mono_module_window
    PRIVATE "cxx_std_20"
)

target_sources(mono_module_window
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/NativeWindow.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp"
)

target_include_directories(mono_module_window
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(mono_module_window
    PUBLIC glad
           glfw
           glm::glm
           imgui::backend_glfw
           imgui::imgui
           spdlog::spdlog
           mono::traits
           mono::config
           mono::resource
           mono::section
           mono::input
           mono::log
)

if(CMAKE_HOST_WIN32)
    target_link_libraries(mono_module_window
        PUBLIC opengl32
               comdlg32
               ole32
    )
    message(STATUS "WIN32 system detected.")
elseif(CMAKE_HOST_UNIX)
    target_link_libraries(mono_module_window
        PUBLIC GL
               X11
               pthread
               Xrandr
               Xi
               dl
    )
    message(STATUS "UNIX system detected.")
endif()
