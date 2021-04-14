#include <gtest/gtest.h>

#include <array>
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <vector>

namespace {
	TEST(TEST_DYNAMIC_BITSET, test_bit_ref) {
		using namespace gal::toolbox::dynamic_bitset;
		// first we get a bitset which length is 5 and init by 5 --> 00101
		dynamic_bitset<> bits(5, 5);
		// this will print them from back, and the output should be 10100
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}

		std::cout << '\n';

		// 00100
		bits[0] = 0;
		// 00110
		bits[1] = 1;
		// 10110
		bits[4] = 1;
		// this will print them from back, and the output should be 01101
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}

		// this will print them from back, and the output should be 10110
		std::cout << '\n' << bits << std::endl;

		// flip them all
		for(auto i = 0; i < bits.size(); ++i)
		{
			bits[i].flip();
		}
		// this will print them from back, and the output should be 10010
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}
	}
}// namespace
