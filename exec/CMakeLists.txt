cmake_minimum_required(VERSION 3.12...3.17)

project(
		galToolBoxExec
		LANGUAGES CXX
)

file(GLOB_RECURSE google-test-source CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/google-test/*.cpp")
file(GLOB_RECURSE test-source CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

add_executable(
		${PROJECT_NAME}
		${google-test-source}
		${test-source}
		main.cpp
)

set_target_properties(
		${PROJECT_NAME}
		PROPERTIES CXX_STANDARD 20
		OUTPUT_NAME "galToolBoxExec"
)

include(cmake/google-test.cmake)
target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		gtest_main
)

include(../cmake/CPM.cmake)
CPMAddPackage(
		NAME galToolBox
		SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)
target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		galToolBox::galToolBox
)
