cmake_minimum_required(VERSION 3.12...3.17)

project(
		galToolbox
		VERSION 0.0.1
		LANGUAGES CXX
)

file(
		GLOB_RECURSE
		galToolboxConcept
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/concepts/*.hpp"
)

file(
		GLOB_RECURSE
		galToolboxDynamicBitset
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/dynamic_bitset/*.hpp"
)

file(
		GLOB_RECURSE
		galToolboxRandom
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/random/*.hpp"
)

file(
		GLOB_RECURSE
		galToolboxParser
		CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/parser/*.hpp"
)

add_library(
		${PROJECT_NAME}
		${galToolboxConcept}
		${galToolboxDynamicBitset}
		${galToolboxRandom}
		${galToolboxParser}
)

set_target_properties(
		${PROJECT_NAME} PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 20
)

target_include_directories(
		${PROJECT_NAME}
		PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${PROJECT_NAME}-${PROJECT_VERSION}>
)

# for dependencies
include(cmake/CPM.cmake)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

CPMAddPackage("gh:TheLartians/PackageProject.cmake#master")

packageProject(
		NAME ${PROJECT_NAME}
		VERSION ${PROJECT_VERSION}
		NAMESPACE ${PROJECT_NAME}
		BINARY_DIR ${PROJECT_BINARY_DIR}
		INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
		INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
		VERSION_HEADER "${VERSION_HEADER_LOCATION}"
		DEPENDENCIES "galToolBox"
)
