cmake_minimum_required(VERSION 3.13...3.20)

project(
		galToolbox
		VERSION 0.0.1
		LANGUAGES CXX
)

set(CMAKE_3RDPARTY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd-party")

include(${CMAKE_3RDPARTY_PATH}/googletest/cmake/google-test.cmake)

add_subdirectory(utils)
add_subdirectory(container)
