# Copyright (c) 2018 Weiming Hu

cmake_minimum_required (VERSION 3.1)
project (linearSolvers)
set (CMAKE_CXX_STANDARD 11)

# Set the output folder
set (COMMON_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/output")
if (EXISTS ${COMMON_OUTPUT_DIR})
    file(REMOVE_RECURSE ${COMMON_OUTPUT_DIR})
endif(EXISTS ${COMMON_OUTPUT_DIR})
file(MAKE_DIRECTORY ${COMMON_OUTPUT_DIR})

# Add the library linearSolvers
set (linearSolvers_SOURCES "Matrix.cpp;Solvers.cpp")
include_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_library (linearSolvers SHARED ${linearSolvers_SOURCES})
set_target_properties(linearSolvers
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/lib")

# Set the names of the files for building executables
set (PROGRAM_NAMES "testMatrix;testSolvers")
foreach (PROGRAM_NAME IN LISTS PROGRAM_NAMES)
    message(STATUS "building program ${PROGRAM_NAME}")
    add_executable (${PROGRAM_NAME} "${PROGRAM_NAME}.cpp")
    target_link_libraries (${PROGRAM_NAME} linearSolvers)
    set_target_properties(${PROGRAM_NAME}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${COMMON_OUTPUT_DIR}/bin")
    add_dependencies(${PROGRAM_NAME} linearSolvers)
endforeach(PROGRAM_NAME IN PROGRAM_NAMES)
