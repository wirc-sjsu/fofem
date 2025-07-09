cmake_minimum_required(VERSION 3.5)

project(FOFEM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)


# Find all CPP files in the src directory
file(GLOB CPP_FILES FOF_UNIX/*.cpp)

# Add the CPP files to the executable
add_executable(fofem ${CPP_FILES})

#copy of FOF_UNIX with all functions marked for export
file(GLOB CPP_LIB_FILES FOF_UNIX/*.cpp)

add_library(fofem_debug_c SHARED ${CPP_LIB_FILES})