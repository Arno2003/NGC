# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(compression)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Add optimization flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

# Add the executable by specifying the source files
add_executable(compression
    main/normalize.cpp
    main/compNormalize.cpp
    main/denormalize.cpp
    main/mgc.cpp 
    main/decompNormalize.cpp
    main/cpuUsage.c
)

# Link pthread library
target_link_libraries(compression pthread)

# Ensure C files are compiled with the C++ compiler
set_source_files_properties(main/cpuUsage.c PROPERTIES LANGUAGE CXX)
