include_guard(GLOBAL)

# Helper script for running clang-format tool.
#
# The following paths are formatted:
# - src/*
# - examples/*
#
# The following file extensions are formatted:
# - .cpp
# - .hpp
# - .inl
#
# Note: It is assumed that this script is run from the root of the workspace.
function(launch_clang_format)
    message(STATUS "Hint: Make sure this scipt is run from the root of the workspace.")

    find_program(CLANG_FORMAT_CMD NAMES "clang-format" REQUIRED)
    find_file(CLANG_FORMAT_CONFIG_FILE NAMES ".clang-format" PATHS "${CMAKE_CURRENT_SOURCE_DIR}" REQUIRED)

    file(GLOB_RECURSE CF_SOURCES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[ch]pp")
    file(GLOB_RECURSE CF_SOURCES_SRC_INL "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl")
    file(GLOB_RECURSE CF_SOURCES_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.[ch]pp")
    set(CF_SOURCES ${CF_SOURCES_SRC} ${CF_SOURCES_SRC_INL} ${CF_SOURCES_EXAMPLES})

    message(STATUS "Launching clang-format...")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
        COMMAND ${CLANG_FORMAT_CMD} -i ${CF_SOURCES}
    )
endfunction()

launch_clang_format()
