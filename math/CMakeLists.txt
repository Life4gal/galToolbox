project(math)

set(
		INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(
		SOURCE
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/math_fwd.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/vector.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/matrix.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/math_apply.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/basic_operations.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/constants.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/trigonometric_operations.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/numeric_and_algorithm.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/exponential_operations.hpp
)

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
