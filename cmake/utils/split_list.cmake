include_guard(GLOBAL)

# Split a list into chunks.
#
# The function returns a list of chunk names which can be used to access actual chunks' contents.
#
# Example usage:
# split_list(SOME_LIST 4 CHUNK_NAMES)
# foreach(CHUNK ${CHUNK_NAMES})
#     message(STATUS "Chunk variable name: ${CHUNK}")
#     message(STATUS "Chunk contents: ${${CHUNK}}")
# endforeach()
function(split_list list num_chunks output_chunk_names_list)
    set(result)
    list(LENGTH ${list} list_length)
    math(EXPR chunk_size "(${list_length}/${num_chunks}) + 1")

    foreach(i RANGE ${num_chunks})
        math(EXPR start_index "${i} * ${chunk_size}")
        if(start_index GREATER_EQUAL ${list_length})
            break()
        endif()

        list(SUBLIST ${list} ${start_index} ${chunk_size} chunk)
        list(APPEND result "CHUNK_${i}")
        set(CHUNK_${i} ${chunk} PARENT_SCOPE)
    endforeach()

    set(${output_chunk_names_list} ${result} PARENT_SCOPE)
endfunction()
