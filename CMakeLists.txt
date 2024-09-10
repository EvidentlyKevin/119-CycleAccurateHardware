# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CYCLE_ACC VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories - adding the include folder
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files for the project
# src and lib directories contain the .cpp files
set(SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/
)

# Add the executable target
add_executable(CYCLE_ACC ${SRC_FILES})