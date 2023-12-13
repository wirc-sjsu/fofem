cmake_minimum_required(VERSION 2.8)

project(FOFEM)

set(CMAKE_CXX_STANDARD 11)

# Find all CPP files in the src directory
file(GLOB CPP_FILES FOF_UNIX/*.cpp)

# Add the CPP files to the executable
add_executable(fofem ${CPP_FILES})
