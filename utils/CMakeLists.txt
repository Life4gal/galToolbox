project(utils)

set(
		SOURCE
		include/assert.hpp
		include/random.hpp
)

set(
		TEST_SOURCE
		test/test_random.cpp
)

#===============================================
# LIBRARY
#===============================================

add_library(
		${PROJECT_NAME} INTERFACE
)

target_include_directories(
		${PROJECT_NAME} INTERFACE
		${SOURCE}
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

target_compile_features(
		${PROJECT_NAME} INTERFACE
		cxx_std_20
)

#===============================================
# LIBRARY TEST CASE
#===============================================

set(
		TEST_PROJECT_NAME
		${PROJECT_NAME}_test
)

add_executable(
		${TEST_PROJECT_NAME}
		${TEST_SOURCE}
		main.cpp
)

set_target_properties(
		${TEST_PROJECT_NAME}
		PROPERTIES CXX_STANDARD 20
		OUTPUT_NAME ${TEST_PROJECT_NAME}
)

target_link_libraries(
		${TEST_PROJECT_NAME}
		PRIVATE
		gtest_main
		${PROJECT_NAME}
)
