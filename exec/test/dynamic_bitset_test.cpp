#include <gtest/gtest.h>

#include <vector>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <array>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

namespace {
	TEST(TEST_DYNAMIC_BITSET, test){
		using namespace gal::toolbox::dynamic_bitset;
		// unsigned int --> 32 bits
		dynamic_bitset<> bits{0, 1, 3, 0, 2, 0, 4};


	}
}// namespace
