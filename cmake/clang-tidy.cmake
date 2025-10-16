include_guard(GLOBAL)

# Helper script for running clang-tidy tool.
#
# Actually, run-clang-tidy tool is used to launch multiple clang-tidy processes in parallel.
# The number of parallel processes is determined with the following formula:
# P = N%2 + N/2 + N/8
# where N is the number of logical processors and P is the number of parallel processes.
#
# The above formula is presented in table below for increasing numbers of CPU cores:
# N | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15| 16| 17| 18|
# --+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
# P | 1 | 1 | 2 | 2 | 3 | 3 | 4 | 5 | 6 | 6 | 7 | 7 | 8 | 8 | 9 | 10| 11| 11|
#
# Additionally, errors/warnings are displayed only from the following paths:
# - src/*
# - examples/*
#
# Note: It is assumed that this script is run from the root of the workspace.
# Note: Before running, compile_commands.json must be generated and placed in build/ directory.
#
# Sources:
# - https://clang.llvm.org/extra/doxygen/run-clang-tidy_8py_source.html
function(launch_clang_tidy)
    message(STATUS "Hint: Make sure this scipt is run from the root of the workspace.")

    find_program(CLANG_TIDY_CMD NAMES "clang-tidy" REQUIRED)
    find_program(RUN_CLANG_TIDY_CMD NAMES "run-clang-tidy" REQUIRED)
    find_package(Python3 REQUIRED)
    find_file(COMPILE_COMMANDS_JSON_FILE NAMES "compile_commands.json" PATHS "build/" REQUIRED)
    find_file(CLANG_TIDY_CONFIG_FILE NAMES ".clang-tidy" PATHS "${CMAKE_CURRENT_SOURCE_DIR}" REQUIRED)

    message(STATUS "Found clang-tidy: ${CLANG_TIDY_CMD}")
    message(STATUS "Found run-clang-tidy: ${RUN_CLANG_TIDY_CMD}")
    message(STATUS "Found compile_commands.json: ${COMPILE_COMMANDS_JSON_FILE}")
    message(STATUS "Found .clang-tidy config file: ${CLANG_TIDY_CONFIG_FILE}")

    # Query number of CPU cores
    include(ProcessorCount)
    ProcessorCount(NPROC)

    if(NPROC EQUAL 0)
        set(NPROC 1)
    endif()

    # Calculate number of parallel processes
    math(EXPR NPROC "${NPROC}%2 + ${NPROC}/2 + ${NPROC}/8")

    set(FILTER_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PATH_SEP "/")

    if(WIN32)
        set(PATH_SEP "\\\\")
        string(REPLACE "/" "\\\\" FILTER_BASE_PATH "${FILTER_BASE_PATH}")
        message(STATUS "Windows detected. Converted base filter path to: ${FILTER_BASE_PATH}")
    endif()

    set(FILTER "(${FILTER_BASE_PATH}${PATH_SEP}examples|${FILTER_BASE_PATH}${PATH_SEP}src)")

    execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ECHO STDOUT
        COMMAND ${Python3_EXECUTABLE} ${RUN_CLANG_TIDY_CMD} -config-file ${CLANG_TIDY_CONFIG_FILE} -p build/ -j ${NPROC} ${FILTER}
    )
endfunction()

launch_clang_tidy(" ${EXTRA_ARGS} ")
