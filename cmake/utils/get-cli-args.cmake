include_guard(GLOBAL)

# Get whole CLI command into as a list.
#
# OUT_VAR: The variable to store the list of arguments in.
function(get_cli_args OUT_VAR)
    set(CMD_ARGS "")
    foreach(ARG_ITER RANGE ${CMAKE_ARGC})
        list(APPEND CMD_ARGS "${CMAKE_ARGV${ARG_ITER}}")
        math(EXPR ARG_ITER "${ARG_ITER} + 1")
    endforeach()
    set(${OUT_VAR} ${CMD_ARGS} PARENT_SCOPE)
endfunction()

# Extract extra arguments from a list containing CLI command.
#
# CLI_ARGS: The list containing the CLI command. Obtained from `get_cli_args`.
# OUT_VAR: The variable to store the list of extra arguments in.
#
# Example:
#   get_cli_args(CLI_ARGS)
#   extract_cli_extra_args("${CLI_ARGS}" EXTRA_ARGS)
#   message(STATUS "Extra arguments: ${EXTRA_ARGS}")
function(extract_cli_extra_args CLI_ARGS OUT_VAR)
    list(FIND CLI_ARGS "-P" INDEX)

    if(INDEX EQUAL -1)
        set(${OUT_VAR} "" PARENT_SCOPE)
        return()
    endif()

    list(LENGTH CLI_ARGS CLI_ARGS_LEN)
    math(EXPR INDEX "${INDEX} + 2")
    if(INDEX GREATER_EQUAL ${CLI_ARGS_LEN})
        set(${OUT_VAR} "" PARENT_SCOPE)
        return()
    endif()

    list(SUBLIST CLI_ARGS ${INDEX} -1 EXTRA_ARGS)
    set(${OUT_VAR} ${EXTRA_ARGS} PARENT_SCOPE)
endfunction()
