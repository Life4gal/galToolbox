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
		9
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

set(GALTOOLBOX_GLOBAL_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(GALTOOLBOX_CONFIG_PATH ${GALTOOLBOX_GLOBAL_SOURCE_PATH}/config)
set(GALTOOLBOX_MODULE_PATH ${GALTOOLBOX_GLOBAL_SOURCE_PATH}/cmake)

set(
		GALTOOLBOX_LIBRARY_HEADERS

		# utils
		include/utils/assert.hpp
		include/utils/random.hpp
		include/utils/sequence_invoker.hpp
		include/utils/tuple_maker.hpp

		# string
		include/string/compile_time_matcher.hpp
		include/string/string_pool.hpp

		# iterator
		include/iterator/stride_iterator.hpp

		# container
		include/container/ring_buffer.hpp
		include/container/fifo.hpp
		include/container/dynamic_bitset.hpp

		#math
		include/math/math.hpp
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
		${PROJECT_NAME}
		INTERFACE
		$<BUILD_INTERFACE:${GALTOOLBOX_GLOBAL_SOURCE_PATH}/include>
		$<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_compile_options(
		${PROJECT_NAME}
		INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:/std:c++latest /W4 /WX>
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

#--- Package
include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} PACKAGE_NAME)

write_basic_package_version_file(
		${PROJECT_BINARY_DIR}/cmake/${PACKAGE_NAME}-config-version.cmake
		VERSION ${GALTOOLBOX_VERSION}
		COMPATIBILITY AnyNewerVersion
)

install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
)

configure_package_config_file(
		${GALTOOLBOX_CONFIG_PATH}/${PROJECT_NAME}-config.cmake.in
		${PROJECT_BINARY_DIR}/cmake/${PACKAGE_NAME}-config.cmake
		INSTALL_DESTINATION cmake/}
)

install(
		EXPORT ${PROJECT_NAME}-targets
		FILE ${PROJECT_NAME}-targets.cmake
		NAMESPACE gal::
		DESTINATION cmake/
)

install(
		FILES ${GALTOOLBOX_LIBRARY_HEADERS}
		DESTINATION include/${PROJECT_NAME}
)

install(
		FILES
		${PROJECT_BINARY_DIR}/cmake/${PACKAGE_NAME}-config.cmake
		${PROJECT_BINARY_DIR}/cmake/${PACKAGE_NAME}-config-version.cmake
		DESTINATION cmake/
)

