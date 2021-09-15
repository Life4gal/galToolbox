project(utils)

set(
		INCLUDE_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(
		SOURCE
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/assert.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/random.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/sequence_invoker.hpp
		${INCLUDE_DIR}/${TOOLBOX_PREFIX}/tuple_maker.hpp
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
