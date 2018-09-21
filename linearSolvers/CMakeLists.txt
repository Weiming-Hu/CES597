# Copyright (c) 2018 Weiming Hu

cmake_minimum_required (VERSION 3.1)
project (Matrix)
set (CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Add definitions for debugging")
    add_definitions(-D_DEBUG)
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Set the output folder
set (COMMON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/output")
if (EXISTS ${COMMON_OUTPUT_DIR})
    file(REMOVE_RECURSE ${COMMON_OUTPUT_DIR})
endif(EXISTS ${COMMON_OUTPUT_DIR})
file(MAKE_DIRECTORY ${COMMON_OUTPUT_DIR})

# Add the library Matrix
set (Matrix_SOURCES "Matrix.cpp")
include_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_library (Matrix SHARED ${Matrix_SOURCES})
set_target_properties(Matrix
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")

# Set the names of the files for building executables
set (PROGRAM_NAMES "testMatrix;directSolver;iterativeSolver")
foreach (PROGRAM_NAME IN LISTS PROGRAM_NAMES)
    message(STATUS "building program ${PROGRAM_NAME}")
    add_executable (${PROGRAM_NAME} "${PROGRAM_NAME}.cpp")
    target_link_libraries (${PROGRAM_NAME} Matrix)
    set_target_properties(${PROGRAM_NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")
    add_dependencies(${PROGRAM_NAME} Matrix)
endforeach(PROGRAM_NAME IN PROGRAM_NAMES)
