#include <gtest/gtest.h>

#include <dynamic_bitset/dynamic_bitset.hpp>
#include <vector>

namespace {
	TEST(TEST_DYNAMIC_BITSET, test){
		using namespace gal::toolbox::dynamic_bitset;

		// unsigned int --> 32 bits
		dynamic_bitset<unsigned int, std::vector> bits{0, 1, 3, 0, 2, 0, 4};

//		for(auto bit : bits)
//		{
//			std::cout << bit.offset << ": " << bit.value << std::endl;
//		}

		/*
		 * todo: something was wrong!!!
		 * bits[0~31] == 0 --> false
		 *
		 * bits[32~63] == 1 --> 00000000000000000000000000000010 --> bits[33] == true
		 *
		 * bit[64~95] == 3 --> 00000000000000000000000000000110 --> bits[65] == true && bits[66] == true
		 *
		 * bits[96~127] == 0 --> false
		 *
		 * bits[128~159] == 2 --> 00000000000000000000000000000100 --> bits[130] == true
		 *
		 * bits[160~191] == 0 --> false
		 *
		 * bits[192~223] == 4 --> 00000000000000000000000000001000 --> bits[195] == true
		 */
		ASSERT_EQ(std::count(bits.begin(), bits.end(), true), 5);

		bits[0] = true;
		ASSERT_EQ(std::count(bits.begin(), bits.end(), true), 6);

		for(auto bit : bits)
		{
			std::cout << bit.offset << ": " << bit.value << std::endl;
		}
	}
}// namespace
