# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(compression)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Add the executable by specifying the source files
add_executable(compression
    main/normalize.cpp
    main/execute.cpp
)