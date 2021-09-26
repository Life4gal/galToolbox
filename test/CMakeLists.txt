project(galToolboxTest)

# global set, locally set use no CMAKE_ version
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# TEST
set(
		TEST_CONTAINER_SOURCE
		src/test_ring_buffer.cpp
		src/test_fifo.cpp
		src/test_dynamic_bitset.cpp
)

set(
		TEST_UTILS_SOURCE
		src/test_random.cpp
		src/test_sequence_invoker.cpp
)

set(
		TEST_STRING_SOURCE
		src/test_compile_time_matcher.cpp
)

set(
		TEST_ITERATOR_SOURCE
		src/test_stride_iterator.cpp
)

add_executable(
		${PROJECT_NAME}
		${TEST_CONTAINER_SOURCE}
		${TEST_UTILS_SOURCE}
		${TEST_STRING_SOURCE}
		${TEST_ITERATOR_SOURCE}
)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message("Setting build type to 'RelWithDebInfo' as none was specified.")
	set(
			CMAKE_BUILD_TYPE RelWithDebInfo
			CACHE STRING "Choose the type of build." FORCE
	)
	set_property(
			CMAKE_BUILD_TYPE
			PROPERTY CACHE
			STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
	)
else()
	message("Current build type is: ${CMAKE_BUILD_TYPE}")
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(
			${PROJECT_NAME}
			PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/MDd /Zi /Ob0 /Od /RTC1 /W4 /WX>
			$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O0 -g -Wall -Wextra -Wpedantic -Werror>
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_options(
			${PROJECT_NAME}
			PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/MD /O2 /Ob2 /DNDEBUG /W4 /WX>
			$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3 -DNDEBUG -Wall -Wextra -Wpedantic -Werror>
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
	target_compile_options(
			${PROJECT_NAME}
			PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/MD /O1 /Ob1 /DNDEBUG /W4 /WX>
			$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Os -DNDEBUG -Wall -Wextra -Wpedantic -Werror>
	)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	target_compile_options(
			${PROJECT_NAME}
			PRIVATE
			$<$<CXX_COMPILER_ID:MSVC>:/MD /Zi /O2 /Ob1 /DNDEBUG /W4 /WX>
			$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O2 -g -DNDEBUG -Wall -Wextra -Wpedantic -Werror>
	)
else()
	message(FATAL_ERROR "Unsupported build type")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_dependencies(
		${PROJECT_NAME}
		galToolbox
		gtest_main
)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		galToolbox
		gtest_main
)

# for gtest_discover_tests
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
