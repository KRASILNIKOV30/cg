cmake_minimum_required(VERSION 3.26)
project(cg)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(catch2 INTERFACE)
add_library(signals INTERFACE)
add_library(tinyobjloader lib/tinyobjloader/tiny_obj_loader.cc)

target_include_directories(tinyobjloader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(
        catch2 INTERFACE lib/catch2
)
target_link_directories(
        signals INTERFACE lib/signals
)

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)
add_subdirectory(lab7)
