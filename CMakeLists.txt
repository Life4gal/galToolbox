cmake_minimum_required(VERSION 3.12...3.17)

project(
		galToolbox
		VERSION 0.0.1
		LANGUAGES CXX
)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIG_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include(${CMAKE_CONFIG_DIR}/basic.cmake)
include(${CMAKE_CONFIG_DIR}/boost.cmake)

add_subdirectory(test)
