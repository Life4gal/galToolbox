project(
	galToolboxTest
	LANGUAGES CXX
)

# global set, locally set use no CMAKE_ version
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
		src/test_string_pool.cpp
)

set(
		TEST_ITERATOR_SOURCE
		src/test_stride_iterator.cpp
)

set(
		TEST_MATH_SOURCE
		src/test_math.cpp
)

add_executable(
		${PROJECT_NAME}

		${TEST_CONTAINER_SOURCE}
		${TEST_UTILS_SOURCE}
		${TEST_STRING_SOURCE}
		${TEST_ITERATOR_SOURCE}
		${TEST_MATH_SOURCE}
)

target_compile_features(
		${PROJECT_NAME}
		PRIVATE

		$<$<CXX_COMPILER_ID:MSVC>:cxx_std_23>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:cxx_std_20>		
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

include(${GAL_MODULE_PATH}/config_build_type.cmake)
BuildAsPrivate()
