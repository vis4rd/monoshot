include_guard()

find_program(CLANG_TIDY_CACHE_CMD NAMES "clang-tidy-cache")

if(CLANG_TIDY_CACHE_CMD)
    message(STATUS "clang-tidy-cache found")
else()
    set(CLANG_TIDY_CACHE_CMD ". ./tools/clang-tidy-cache")
    message(STATUS "clang-tidy-cache not found, using shipped version.")
endif()

set(ENV{CTCACHE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/.cache/clang-tidy/")
set(CLANG_TIDY_CMD "${CLANG_TIDY_CACHE_CMD} clang-tidy")

file(GLOB_RECURSE CT_ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/engine/module/*.[ch]pp")
file(GLOB_RECURSE CT_GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.[ch]pp")
set(CT_SOURCES "${CT_ENGINE_SOURCES};${CT_GAME_SOURCES}")

add_custom_target(clang_tidy
    COMMAND clang-tidy --config-file=.clang-tidy --extra-arg="--std=c++20" --use-color --header-filter="^src/\(engine/module/|game/include/\)" -p=build/ ${CT_ENGINE_SOURCES}
    COMMAND clang-tidy --config-file=.clang-tidy --extra-arg="--std=c++20" --use-color --header-filter="^src/\(engine/module/|game/include/\)" -p=build/ ${CT_GAME_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    COMMAND_EXPAND_LISTS
)
