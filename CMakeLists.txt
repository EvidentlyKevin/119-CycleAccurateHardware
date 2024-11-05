cmake_minimum_required(VERSION 3.10)
project(CacheModelProject)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX- -wd4100 -wd4267")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX-")


# Include directories
include_directories(include)

# Gather all source files automatically from the lib and src directories
file(GLOB_RECURSE SRC_FILES src/*.cpp lib/*.cpp) #add scripts folder to source in order to access when calling calling Debug in terminal 
                                                 #fifo remains, ../scripts/cpp file name 
                                                

# Create executable from source files
add_executable(cache_model ${SRC_FILES})

# Set compiler warning options based on the compiler
if (MSVC)
    # Use the appropriate warning flags for MSVC
    target_compile_options(cache_model PRIVATE /W4 /WX)
else()
    # Use the appropriate flags for GCC/Clang
    target_compile_options(cache_model PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
