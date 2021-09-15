project(container)

set(
		INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(
		SOURCE
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/ring_buffer.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/fifo.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/dynamic_bitset.hpp)

add_library(
		${PROJECT_NAME} INTERFACE
)

target_include_directories(
		${PROJECT_NAME} INTERFACE
		${SOURCE}
		$<BUILD_INTERFACE:${INCLUDE_DIR}>
		$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${PROJECT_NAME}-${PROJECT_VERSION}>
)

target_compile_features(
		${PROJECT_NAME} INTERFACE
		cxx_std_20
)

target_link_libraries(
		${PROJECT_NAME} INTERFACE
		utils
)
