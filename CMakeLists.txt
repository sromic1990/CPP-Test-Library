cmake_minimum_required(VERSION 3.0)

project(CPPTestLibrary)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(ADDITIONAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/tests")

# Glob all source files
file(GLOB SOURCES
    "${SOURCE_DIR}/*.cpp"
    "${ADDITIONAL_SOURCE_DIR}/*.cpp"
)

# Include directories if needed
include_directories(
    "${SOURCE_DIR}"
    "${ADDITIONAL_SOURCE_DIR}"
)

add_executable(Output ${SOURCES})

#Platform specific settings
if (WIN32)
    message("Windows")
    if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
    else()
        message("Windows MSVC")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows/bin)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows/lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows/lib)
    endif()
elseif (APPLE)
    message("Mac OS")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac/lib)
else()
    message("Other")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
endif()
