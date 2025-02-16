include_guard(GLOBAL)

# Find *.cpp, *.hpp and *.inl files in a directory.
#
# DIR_PATH: The directory to search in.
# OUT_VAR: The variable to store the list of files in.
function(find_source_files DIR_PATH OUT_VAR)
    file(REAL_PATH "${DIR_PATH}" DIR_PATH)
    message(DEBUG "Searching for source files in '${DIR_PATH}'")
    file(GLOB_RECURSE SOURCES
        "${DIR_PATH}/*.[ch]pp"
        "${DIR_PATH}/*.inl"
    )

    set(${OUT_VAR} ${SOURCES} PARENT_SCOPE)
endfunction()

if(CMAKE_SCRIPT_MODE_FILE AND NOT CMAKE_PARENT_LIST_FILE)
    if(NOT DEFINED CMAKE_ARGV3)
        message(FATAL_ERROR "DIR_PATH not provided")
    else()
        find_source_files("${CMAKE_ARGV3}" SOURCE_FILES)
        string(REPLACE ";" " " SOURCE_FILES "${SOURCE_FILES}")
        message(${SOURCE_FILES})
    endif()
endif()
