# Monoshot versioning

# Semver 2.0 is respected, see https://semver.org/

set(VERSION_CACHE_FILE "${MONOSHOT_BINARY_DIR}/version.txt")

# ensure file exists
if(NOT EXISTS ${VERSION_CACHE_FILE})
    file(TOUCH ${VERSION_CACHE_FILE})
    file(WRITE ${VERSION_CACHE_FILE} "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
endif()

# read from cache file
file(READ ${VERSION_CACHE_FILE} VERSION_IN_CACHE)

# validation
string(REGEX MATCH "^[0-9]+(\\.[0-9]+(\\.[0-9]+(\\-(rc|pre|alpha|beta)(\\.[0-9]+)?)?(\\+build(\\.[0-9]+)?)?)?)?$" IS_VERSION_VALID ${VERSION_IN_CACHE})
if(NOT IS_VERSION_VALID)
    message(SEND_ERROR "Version found in cache is not valid: ${VERSION_IN_CACHE}")
endif()

# extract version
string(REGEX MATCH "^[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?" VERSION_CACHED ${VERSION_IN_CACHE})
string(REGEX REPLACE "^([0-9]+)(\\.([0-9]+)(\\.([0-9]+))?)?" "\\1" MAJOR_VERSION_CACHED ${VERSION_CACHED})
string(REGEX REPLACE "^([0-9]+)(\\.([0-9]+)(\\.([0-9]+))?)?" "\\3" MINOR_VERSION_CACHED ${VERSION_CACHED})
string(REGEX REPLACE "^([0-9]+)(\\.([0-9]+)(\\.([0-9]+))?)?" "\\5" PATCH_VERSION_CACHED ${VERSION_CACHED})

# extract prerelease version
string(REGEX MATCH "\\-(rc|pre|alpha|beta)(\\.[0-9]+)?" PRERELEASE_VERSION_CACHED_TEMP ${VERSION_IN_CACHE})
if(PRERELEASE_VERSION_CACHED_TEMP)
    string(REGEX REPLACE "\\-(rc|pre|alpha|beta)(\\.([0-9]+))?" "\\3" PRERELEASE_VERSION_CACHED ${PRERELEASE_VERSION_CACHED_TEMP})
    string(REGEX REPLACE "\\-(rc|pre|alpha|beta)(\\.([0-9]+))?" "\\1" PRERELEASE_IDENTIFIER_CACHED ${PRERELEASE_VERSION_CACHED_TEMP})
endif()

# extract build version
string(REGEX MATCH "\\+build(\\.[0-9]+)?" BUILD_VERSION_CACHED ${VERSION_IN_CACHE})
if(BUILD_VERSION_CACHED)
    string(REGEX REPLACE "\\+build(\\.([0-9]+))?" "\\2" BUILD_VERSION_CACHED ${BUILD_VERSION_CACHED})
endif()

# prepare base version
set(MONOSHOT_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(MONOSHOT_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(MONOSHOT_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# prepare the rest of version components
if(PRERELEASE_IDENTIFIER_CACHED)
    set(MONOSHOT_VERSION_PRERELEASE_IDENTIFIER ${PRERELEASE_IDENTIFIER_CACHED})
endif()
if(PRERELEASE_VERSION_CACHED)
    set(MONOSHOT_VERSION_PRERELEASE ${PRERELEASE_VERSION_CACHED})
endif()
if(BUILD_VERSION_CACHED)
    math(EXPR BUILD_VERSION_CACHED "${BUILD_VERSION_CACHED} + 1")
    set(MONOSHOT_VERSION_BUILD ${BUILD_VERSION_CACHED})
endif()

# aggregate base version to one variable
set(MONOSHOT_VERSION "${MONOSHOT_VERSION_MAJOR}.${MONOSHOT_VERSION_MINOR}.${MONOSHOT_VERSION_PATCH}")

# unset cached variables if version in cache is outdated
if("${MONOSHOT_VERSION}" VERSION_GREATER "${VERSION_CACHED}")
    unset(MONOSHOT_VERSION_PRERELEASE_IDENTIFIER)
    unset(MONOSHOT_VERSION_PRERELEASE)
    unset(MONOSHOT_VERSION_BUILD)
endif()

# append prerelease and/or build version to base version
if(MONOSHOT_VERSION_PRERELEASE_IDENTIFIER)
    set(MONOSHOT_VERSION "${MONOSHOT_VERSION}-${MONOSHOT_VERSION_PRERELEASE_IDENTIFIER}")
    if(MONOSHOT_VERSION_PRERELEASE)
        set(MONOSHOT_VERSION "${MONOSHOT_VERSION}.${MONOSHOT_VERSION_PRERELEASE}")
    endif()
endif()
if(MONOSHOT_VERSION_BUILD)
    set(MONOSHOT_VERSION "${MONOSHOT_VERSION}+build.${MONOSHOT_VERSION_BUILD}")
endif()

# prepare to save version to cache file
# append new build version if cached version did not have build identifier
if(NOT BUILD_VERSION_CACHED)
    set(MONOSHOT_VERSION "${MONOSHOT_VERSION}+build.1")
else()
    string(REGEX REPLACE "\\+build(\\.[0-9]+)?" "+build.${BUILD_VERSION_CACHED}" MONOSHOT_VERSION ${MONOSHOT_VERSION})
endif()

# save version to cache file
file(WRITE ${VERSION_CACHE_FILE} ${MONOSHOT_VERSION})
