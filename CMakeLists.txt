cmake_minimum_required(VERSION 3.13...3.20)

set(
		GALTOOLBOX_MAJOR_VERSION
		0
)

set(
		GALTOOLBOX_MINOR_VERSION
		0
)

set(
		GALTOOLBOX_PATCH_VERSION
		7
)

set(
		GALTOOLBOX_VERSION
		${GALTOOLBOX_MAJOR_VERSION}.${GALTOOLBOX_MINOR_VERSION}.${GALTOOLBOX_PATCH_VERSION}
)

project(
		galToolbox
		VERSION ${GALTOOLBOX_VERSION}
		LANGUAGES CXX
)

set(GALTOOLBOX_GLOBAL_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(GALTOOLBOX_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
		${PROJECT_NAME}
		INTERFACE
		${PROJECT_SOURCE_DIR}/include
)

target_compile_options(
		${PROJECT_NAME}
		INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:/std:c++latest /W4 /WX /Wall>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-std=c++2a -Wall -Wextra -Wpedantic -Werror>
)

option(
		GALTOOLBOX_NEED_BUILD_TEST
		"build galToolbox test cases"
		ON
)

if(${GALTOOLBOX_NEED_BUILD_TEST})
	include(${GALTOOLBOX_MODULE_PATH}/prepare-3rd-party.cmake)
	# enable test
	enable_testing()
	# Include google test
	include(${GOOGLE_TEST_PATH}/google-test.cmake)
	add_subdirectory(test)
endif(${GALTOOLBOX_NEED_BUILD_TEST})
