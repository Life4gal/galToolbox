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
		src/test_trigonometric_operations.cpp
		src/test_exponential_operations.cpp
		src/test_geometric_operations.cpp
)

add_executable(
		${PROJECT_NAME}
		${SOURCE}
		main.cpp
)

## todo: https://github.com/microsoft/STL/issues/1814
## it seems that the generated sln does not set <LanguageStandard>
## and <format> and <ranges> requires /std:latest
#if(MSVC)
##	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129)
##		set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++20")
##		set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++20")
##	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
#		set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
##		set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
##	endif(CMAKE_CXX_COMPILER_VERSION)
#endif(MSVC)

# it seems that using this will solve the problem
set(CMAKE_CXX_STANDARD 20)

set_target_properties(
		${PROJECT_NAME}
		PROPERTIES
#		CXX_STANDARD 20
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
