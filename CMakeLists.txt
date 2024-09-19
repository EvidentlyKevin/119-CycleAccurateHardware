cmake_minimum_required(VERSION 3.10)
project(CacheModelProject)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Gather all source files automatically from the lib and src directories
file(GLOB_RECURSE SRC_FILES src/*.cpp lib/*.cpp)

# Create executable from source files
add_executable(cache_model ${SRC_FILES})

# Optional: Enable warnings for better code quality
target_compile_options(cache_model PRIVATE -Wall -Wextra -Werror)
