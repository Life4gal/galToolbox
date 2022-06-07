cmake_minimum_required(VERSION 3.13...3.20)

set(
		GALTOOLBOX_MAJOR_VERSION
		0
)

set(
		GALTOOLBOX_MINOR_VERSION
		2
)

set(
		GALTOOLBOX_PATCH_VERSION
		0
)

set(
		GALTOOLBOX_VERSION
		${GALTOOLBOX_MAJOR_VERSION}.${GALTOOLBOX_MINOR_VERSION}.${GALTOOLBOX_PATCH_VERSION}
)

project(
		galToolbox
		VERSION ${GALTOOLBOX_VERSION}
		DESCRIPTION "a toy toolbox"
		HOMEPAGE_URL "https://github.com/Life4gal/galToolbox"
		LANGUAGES CXX
)

set(GAL_GLOBAL_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(GAL_CONFIG_PATH ${GAL_GLOBAL_SOURCE_PATH}/config)
set(GAL_MODULE_PATH ${GAL_GLOBAL_SOURCE_PATH}/cmake)
set(GAL_3RDPARTY_PATH ${GAL_GLOBAL_SOURCE_PATH}/3rd-party)

option(INCLUDE_GOOGLETEST_CASES "make googletest as a ${PROJECT_NAME} test cases" ON)
option(INLCUDE_RANGE_V3 "make range_v3 as a ${PROJECT_NAME} dependency" ON)
option(INLCUDE_FMTLIB "make fmtlib as a ${PROJECT_NAME} dependency (if the current compiler does not support std::format)" ON)
option(INLCUDE_SPDLOG "make spdlog as a ${PROJECT_NAME} dependency" ON)


set(
	${PROJECT_NAME}_SOURCE

	src/utils/assert.cpp
)

add_library(
	${PROJECT_NAME} 
	STATIC
	${${PROJECT_NAME}_SOURCE}
)

add_library(
		gal::toolbox
		ALIAS
		${PROJECT_NAME}
)

target_include_directories(
		${PROJECT_NAME}
		PUBLIC
		${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
	target_compile_features(
			${PROJECT_NAME}
			PRIVATE
			cxx_std_23
	)
else()
	target_compile_features(
			${PROJECT_NAME}
			PRIVATE
			cxx_std_20
	)
endif(MSVC)

if(${INCLUDE_GOOGLETEST_CASES})
	message("${PROJECT_NAME} info: build ${PROJECT_NAME}'s test cases.")
	include(${GAL_3RDPARTY_PATH}/google-test.cmake)
	#target_link_libraries(
	#	${PROJECT_NAME}
	#	PUBLIC
	#	gtest_main
	#)
	enable_testing()
	add_subdirectory(test)
else()
	message("${PROJECT_NAME} info: drop ${PROJECT_NAME}'s test cases.")
endif(${INCLUDE_GOOGLETEST_CASES})

if(${INLCUDE_RANGE_V3})
	message("${PROJECT_NAME} info: make range-v3 as a ${PROJECT_NAME} dependency.")
	include(${GAL_3RDPARTY_PATH}/range-v3.cmake)
	target_link_libraries(
			${PROJECT_NAME}
			PUBLIC
			range-v3
	)
else()
	message("${PROJECT_NAME} info: drop range-v3.")
endif(${INLCUDE_RANGE_V3})

if(${INLCUDE_FMTLIB})
	include(CheckCXXSourceCompiles)
	CHECK_CXX_SOURCE_COMPILES(
			"
			#include <format>
			#include <iostream>

			int main()
			{
				std::cout << std::format(\"hello {}!\", \"world\");
			}
		"
			COMPILER_SUPPORT_FORMAT
	)

	if (${COMPILER_SUPPORT_FORMAT})
		message("${PROJECT_NAME} info: current compiler ${CMAKE_CXX_COMPILER} support std::format.")
	else ()
		message("${PROJECT_NAME} info: current compiler does not support std::format, make fmtlib as a ${PROJECT_NAME} dependency.")
		target_compile_definitions(
				${PROJECT_NAME}
				PUBLIC # todo: should be PRIVATE
				GAL_FMT_NOT_SUPPORT
		)
		include(${GAL_3RDPARTY_PATH}/fmtlib.cmake)
		target_link_libraries(
				${PROJECT_NAME}
				PUBLIC
				fmt
		)
	endif (${COMPILER_SUPPORT_FORMAT})
else()
	message("${PROJECT_NAME} info: drop fmtlib.")
endif(${INLCUDE_FMTLIB})

if(${INLCUDE_SPDLOG})
	message("${PROJECT_NAME} info: make spdlog as a ${PROJECT_NAME} dependency.")
	include(${GAL_3RDPARTY_PATH}/spdlog.cmake)
	target_link_libraries(
				${PROJECT_NAME}
				PUBLIC
				spdlog
				# spdlog_header_only
	)
else()
	message("${PROJECT_NAME} info: drop spdlog.")
endif(${INLCUDE_SPDLOG})

include(${GAL_MODULE_PATH}/config_build_type.cmake)
BuildAsPrivate()
