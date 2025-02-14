cmake_minimum_required(VERSION 3.26)
project(cg)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)

add_subdirectory(lab1)
