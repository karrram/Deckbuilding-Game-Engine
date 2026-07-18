cmake_minimum_required(VERSION 3.15)

# Project Name and Version
project(DeckBuilderEngine VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify the output directories for the compiled executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect all source files recursively from the src/ directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable target using the collected source files
add_executable(DeckBuilderEngine ${SOURCES})

# Tell the compiler where to find the header (.hpp) files
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optional: Enable strict compiler warnings to catch errors early
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()