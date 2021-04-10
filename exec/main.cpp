#include <gtest/gtest.h>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	std::cout << "Hello World! " << __cplusplus << std::endl;

	return RUN_ALL_TESTS();
}
