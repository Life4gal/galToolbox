#include <gtest/gtest.h>

#include <dynamic_bitset/dynamic_bitset.hpp>

namespace {
	TEST(TEST_DYNAMIC_BITSET, WE_NEED_CONSTEXPR_VECTOR) {
		using namespace gal::toolbox::dynamic_bitset;
		// first we get a bitset which length is 5 and init by 5 --> 00101
		dynamic_bitset<> bits(5, 5);
		// this will print them from back, and the output should be 10100
		std::cout << bits.count() << " bits been set: ";
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}
		std::cout << '\n';

		// 00100
		bits[0] = false;
		// 00110
		bits[1] = true;
		// 10110
		bits[4] = true;
		// this will print them from back, and the output should be 01101
		std::cout << bits.count() << " bits been set: ";
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}
		std::cout << '\n';

		// this will print them from back, and the output should be 10110
		std::cout << bits.count() << " bits been set: ";
		std::cout << bits;
		std::cout << '\n';

		// flip them all
		for(auto i = 0; i < bits.size(); ++i)
		{
			bits[i].flip();
		}
		// this will print them from back, and the output should be 10010
		std::cout << bits.count() << " bits been set: ";
		for(auto i = 0; i < bits.size(); ++i)
		{
			std::cout << bits[i];
		}
		std::cout << '\n';

		std::cout << "test for-loop, should be equal with above: ";
		for(auto it = bits.begin(); it != bits.end(); ++it)
		{
			std::cout << it.operator*();
		}
		std::cout << '\n';

		dynamic_bitset<> bits1{255, 511, 1023};
//		std::cin >> bits1;
		std::cout << "before shift: \n" << bits1 << '\n';
		bits1 <<= 1;

		std::cout << bits.count() << " bits been set after left shift 1: \n" << bits1 << '\n';
		bits1 >>= 2;
		std::cout << bits.count() << " bits been set after right shift 2: \n" << bits1 << '\n';
	}
}// namespace
