cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(STB_IMAGE
    HOMEPAGE_URL https://github.com/nothings/stb
    VERSION 2.27
    LANGUAGES CXX
)

set(STB_IMAGE_SOURCE_PATH "${PROJECT_SOURCE_DIR}/include/stbi")
set(STB_IMAGE_SOURCES "${STB_IMAGE_SOURCE_PATH}/stb_image.h")
set(STB_IMAGE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

file(MAKE_DIRECTORY "${STB_IMAGE_SOURCE_PATH}")

# Download sources
set(STB_IMAGE_SOURCE_FETCH_URL "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h")
if(NOT EXISTS ${STB_IMAGE_SOURCES})
    file(DOWNLOAD
        ${STB_IMAGE_SOURCE_FETCH_URL}
        ${STB_IMAGE_SOURCES}
        INACTIVITY_TIMEOUT 5
        TIMEOUT 10
        STATUS STB_IMAGE_DOWNLOAD_STATUS
    )
    list(GET STB_IMAGE_DOWNLOAD_STATUS 0 STB_IMAGE_NOT_FOUND)
    list(GET STB_IMAGE_DOWNLOAD_STATUS 1 STB_IMAGE_DOWNLOAD_ERROR_MESSAGE)
    if(STB_IMAGE_NOT_FOUND)
        message(SEND_ERROR ${STB_IMAGE_DOWNLOAD_ERROR_MESSAGE})
    else()
        message(STATUS "Library stb_image fetched from ${STB_IMAGE_SOURCE_FETCH_URL}")
    endif()
else()
    message(STATUS "Library stb_image found at ${STB_IMAGE_SOURCES}")
    set(STB_IMAGE_NOT_FOUND FALSE)
endif()
set(STB_IMAGE_FOUND (NOT STB_IMAGE_NOT_FOUND))

# Download license file
if(NOT EXISTS ${STB_IMAGE_LICENSE})
    file(DOWNLOAD
        "https://raw.githubusercontent.com/nothings/stb/dev/LICENSE"
        ${STB_IMAGE_LICENSE}
        INACTIVITY_TIMEOUT 5
        TIMEOUT 10
        STATUS STB_IMAGE_LICENSE_DOWNLOAD_STATUS
    )
    # TODO: Extract fetching of these files to a separate function
endif()

# Generate dummy source to compile into library
set(STB_IMAGE_FILE_SOURCE "${STB_IMAGE_SOURCE_PATH}/stb_image.c")
if(NOT EXISTS ${STB_IMAGE_FILE_SOURCE})
    file(WRITE ${STB_IMAGE_FILE_SOURCE}
"#define STB_IMAGE_IMPLEMENTATION
#include \"stb_image.h\"
    ")
endif()

# Create library object
add_library(stb_image OBJECT STATIC)
add_library(stb::image ALIAS stb_image)

target_include_directories(stb_image
    INTERFACE "${PROJECT_SOURCE_DIR}/include"
)

target_sources(stb_image
    PRIVATE ${STB_IMAGE_FILE_SOURCE}
)
