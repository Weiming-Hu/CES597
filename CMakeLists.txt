# Copyright (c) 2018 Weiming Hu

cmake_minimum_required (VERSION 3.1)
project (Matrix)
set (CMAKE_CXX_STANDARD 11)

option(PROFILE_TIME "Build programs with profiling functions" OFF)
option(USE_MPI "Build programs with USE_MPI" OFF)

if (${PROFILE_TIME})
    add_definitions(-D_PROFILE_TIME)
    message(STATUS "Build programs with profiling functions")
endif (${PROFILE_TIME})

if (${USE_MPI})
    add_definitions(-D_USE_MPI)
    message(STATUS "Build programs with MPI")
endif (${USE_MPI})

if (CMAKE_BUILD_TYPE)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(STATUS "Add definitions for debugging")
        add_definitions(-D_DEBUG)
    endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endif (CMAKE_BUILD_TYPE)

# Set the output folder
set (COMMON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/output")
if (EXISTS ${COMMON_OUTPUT_DIR})
    file(REMOVE_RECURSE ${COMMON_OUTPUT_DIR})
endif(EXISTS ${COMMON_OUTPUT_DIR})
file(MAKE_DIRECTORY ${COMMON_OUTPUT_DIR})

# Add the library Matrix
set (Matrix_SOURCES "src/Matrix.cpp")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_library (Matrix SHARED ${Matrix_SOURCES})
set_target_properties(Matrix
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")

# Set the names of the files for building executables
set (PROGRAM_NAMES "testMatrix;directSolver;iterativeSolver")
foreach (PROGRAM_NAME IN LISTS PROGRAM_NAMES)
    message(STATUS "building program ${PROGRAM_NAME}")
    add_executable (${PROGRAM_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROGRAM_NAME}.cpp")
    target_link_libraries (${PROGRAM_NAME} Matrix)
    set_target_properties(${PROGRAM_NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")

    if (EXE_SUFFIX)
        set_target_properties(${PROGRAM_NAME}
            PROPERTIES SUFFIX ${EXE_SUFFIX})
    endif (EXE_SUFFIX)

    add_dependencies(${PROGRAM_NAME} Matrix)
endforeach(PROGRAM_NAME IN PROGRAM_NAMES)

message (STATUS "compile flags are ${CMAKE_CXX_FLAGS}")
