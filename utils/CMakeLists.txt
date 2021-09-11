project(utils)

set(
		SOURCE
		src/assert.hpp
		src/random.hpp
)

set(
		TEST_SOURCE
		test/test_random.cpp
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
		OUTPUT_NAME "utils"
)

include(${CMAKE_3RDPARTY_PATH}/googletest/cmake/google-test.cmake)
target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		gtest_main
)
