include_guard(GLOBAL)

# Run clang-tidy on a directory.
#
# DIR_PATH: The directory to run clang-tidy on. Every *.cpp, *.hpp and *.inl file will be checked.
function(run_clang_tidy DIR_PATH)
    find_program(CLANG_TIDY_CMD NAMES "clang-tidy")

    if(NOT CLANG_TIDY_CMD)
        message(SEND_ERROR "clang-tidy not found, aborting...")
        return()
    endif()

    include("${CMAKE_CURRENT_LIST_DIR}/utils/find-source-files.cmake")

    find_source_files("${DIR_PATH}" CT_SOURCES)
    list(LENGTH CT_SOURCES CT_SOURCES_COUNT)
    message(STATUS "There are ${CT_SOURCES_COUNT} files to check.")

    set(CT_FULL_COMMAND ${CLANG_TIDY_CMD} --quiet --config-file=.clang-tidy -p=build/ ${CT_SOURCES})
    message(STATUS "Launching 1 process of clang-tidy.")
    message(DEBUG "Working directory: ${CMAKE_CURRENT_SOURCE_DIR}")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CT_FULL_COMMAND}
    )
endfunction()

if(CMAKE_SCRIPT_MODE_FILE AND NOT CMAKE_PARENT_LIST_FILE)
    if(NOT DEFINED CMAKE_ARGV3)
        message(FATAL_ERROR "DIR_PATH not provided")
    endif()

    include("${CMAKE_CURRENT_LIST_DIR}/utils/get-cli-args.cmake")
    get_cli_args(CLI_ARGS)
    extract_cli_extra_args("${CLI_ARGS}" EXTRA_ARGS)

    run_clang_tidy("${EXTRA_ARGS}")
endif()
