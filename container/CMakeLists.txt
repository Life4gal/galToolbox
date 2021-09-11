project(container)

set(
		SOURCE
		src/ring_buffer.hpp
)

set(
		TEST_SOURCE
		test/test_ring_buffer.cpp
)

add_executable(
		${PROJECT_NAME}
		${SOURCE}
		${TEST_SOURCE}
		main.cpp
)

set_target_properties(
		${PROJECT_NAME}
		PROPERTIES CXX_STANDARD 20
		OUTPUT_NAME "container"
)

target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		gtest_main
)
