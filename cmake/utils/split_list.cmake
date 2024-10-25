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
    message(DEBUG "Requested number of chunks: ${num_chunks}")

    math(EXPR last_chunk_index "${num_chunks} - 1")
    set(start_index 0)
    foreach(i RANGE ${last_chunk_index})
        if(start_index GREATER_EQUAL ${list_length})
            message(DEBUG "No more items to split.")
            break()
        endif()
        math(EXPR items_left_count "${list_length} - ${start_index}")
        math(EXPR chunks_left_count "${num_chunks} - ${i}")
        math(EXPR chunk_size "${items_left_count}/${chunks_left_count}")

        message(DEBUG "Chunk ${i}:")
        message(DEBUG "  start index: ${start_index}")
        message(DEBUG "  items_left_count: ${items_left_count}")
        message(DEBUG "  chunks_left_count: ${chunks_left_count}")
        message(DEBUG "  size: ${chunk_size}")

        list(SUBLIST ${list} ${start_index} ${chunk_size} chunk)
        list(APPEND result "CHUNK_${i}")
        set(CHUNK_${i} ${chunk} PARENT_SCOPE)

        math(EXPR start_index "${start_index} + ${chunk_size}")
    endforeach()

    set(${output_chunk_names_list} ${result} PARENT_SCOPE)
endfunction()
