find_program(CLANG_FORMAT_CMD NAMES "clang-format")

if(NOT CLANG_FORMAT_CMD)
    message(FATAL_ERROR "clang-format not found, aborting...")
endif()

file(GLOB_RECURSE CF_ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/engine/module/*.[ch]pp")
file(GLOB_RECURSE CF_GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.[ch]pp")
set(CF_SOURCES "${CF_ENGINE_SOURCES};${CF_GAME_SOURCES}")

message(STATUS "Launching clang-format...")
execute_process(
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CLANG_FORMAT_CMD} -i ${CF_SOURCES}
)
