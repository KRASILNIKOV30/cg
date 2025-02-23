cmake_minimum_required(VERSION 3.26)
project(cg)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
add_library(signals INTERFACE)
target_include_directories(
        catch2 INTERFACE lib/catch2
)
target_link_directories(
        signals INTERFACE lib/signals
)

add_subdirectory(lab1)
add_subdirectory(lab2)
