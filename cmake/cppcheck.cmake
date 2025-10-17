include_guard(GLOBAL)

# Helper script for running cppcheck tool.
#
# Note: It is assumed that this script is run from the root of the workspace.
# Note: Before running, compile_commands.json must be generated and placed in build/ directory.
function(launch_cppcheck)
    message(STATUS "Hint: Make sure this scipt is run from the root of the workspace.")

    find_program(CPPCHECK_CMD NAMES "cppcheck" REQUIRED)
    find_package(Python3 REQUIRED)
    find_file(COMPILE_COMMANDS_JSON_FILE NAMES "compile_commands.json" PATHS "build/" REQUIRED)

    message(STATUS "Launching cppcheck...")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
        COMMAND ${CPPCHECK_CMD} --cppcheck-build-dir=${CMAKE_CURRENT_SOURCE_DIR}/.cache/cppcheck --quiet --std=c++20 --project=${COMPILE_COMMANDS_JSON_FILE} -i ${CMAKE_CURRENT_SOURCE_DIR}/lib/ -i ${CMAKE_CURRENT_SOURCE_DIR}/build/ --enable=style --error-exitcode=2 --check-level=exhaustive
    )
endfunction()

launch_cppcheck()
