project(test)

set(
		SOURCE
		src/test_ring_buffer.cpp
		src/test_fifo.cpp
		src/test_random.cpp
		src/test_dynamic_bitset.cpp
		src/test_stride_iterator.cpp
		src/test_sequence_invoker.cpp
		src/test_compile_time_matcher.cpp
		src/test_basic_operations.cpp
		src/test_trigonometric_operations.cpp src/exponential_operaions.cpp src/test_geometric_operations.cpp)

add_executable(
		${PROJECT_NAME}
		${SOURCE}
		main.cpp
)

set_target_properties(
		${PROJECT_NAME}
		PROPERTIES CXX_STANDARD 20
		OUTPUT_NAME ${PROJECT_NAME}
)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		gtest_main
		container
		utils
		iterator
		string
		math
)
