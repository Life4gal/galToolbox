cmake_minimum_required(VERSION 3.13...3.20)

project(
		galToolbox
		VERSION 0.0.5
		LANGUAGES CXX
)

set(CMAKE_3RDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)
include(${CMAKE_3RDPARTY_PATH}/googletest/cmake/google-test.cmake)

set(
		TOOLBOX_PREFIX
		gal
)

add_subdirectory(utils)
add_subdirectory(container)
add_subdirectory(iterator)

add_subdirectory(test)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
			FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()
