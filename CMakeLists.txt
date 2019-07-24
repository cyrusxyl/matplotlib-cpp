cmake_minimum_required(VERSION 3.14)
project(matplotlibcpp-cmake)

set(CMAKE_CXX_STANDARD 14)

# Let CMake paths to easily find our custom CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/macros.cmake)

default_build_type("Release")
common_output_dirs()

add_subdirectory(src)
