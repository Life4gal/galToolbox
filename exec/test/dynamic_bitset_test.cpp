#include <gtest/gtest.h>

#include <vector>
#define GAL_DYNAMIC_BITSET_USING_STD
#include <dynamic_bitset/dynamic_bitset.hpp>
#include <iterator>
namespace {
	TEST(TEST_DYNAMIC_BITSET, WE_NEED_CONSTEXPR_VECTOR) {
		using namespace gal::toolbox::dynamic_bitset;
		basic_dynamic_bitset foo1{};
		// auto deduce to unsigned int
		basic_dynamic_bitset<unsigned int, std::allocator, std::vector> foo2{std::allocator<int>{}};
		// auto deduce to unsigned int
		basic_dynamic_bitset foo3{10, 1023};

		basic_dynamic_bitset foo41{std::string{"101"}, 0, 2};
		basic_dynamic_bitset foo42{std::string{"101"}, 1};

		basic_dynamic_bitset foo51{std::string_view{"101"}, 0, 2};
		basic_dynamic_bitset foo52{std::string_view{"101"}, 1};

		basic_dynamic_bitset foo61{"101", 0, 2};
		basic_dynamic_bitset foo62{"101", 1};

		std::vector<unsigned int> vec{1, 2, 3};
		basic_dynamic_bitset<unsigned int, std::allocator, std::vector> foo7{vec.cbegin(), vec.cend()};

		basic_dynamic_bitset<unsigned int, std::allocator, std::vector> foo8{std::cin};


		// first we get a bitset which length is 50 and init by 9999
		// we got 10011100001111 (bin of 9999) and zero-fill --> 00000000000000000000000000000000000010011100001111
		basic_dynamic_bitset bits{50, 9999};
		// this will print them from back, and the output should be 11110000111001000000000000000000000000000000000000
		std::cout << "1: " << bits.count() << " bits been set(output from back to front): ";
		for (auto&& bit: bits) {
			std::cout << bit;
		}
		std::cout << '\n';

		// set bits between bits[20] and bits[20+20] to true
		bits.set(20, 20, true);
		std::cout << "2: " << bits.count() << " bits been set(output from back to front): ";
		for (auto&& bit: bits) {
			std::cout << bit;
		}
		std::cout << '\n';

		// this will print them from front
		std::cout << "3: " << bits.count() << " bits been set(output from front to back): " << bits << '\n';

		// flip them all
		bits.flip();
		std::cout << "4: " << bits.count() << " bits been set(output from back to front): ";
		for (auto&& bit: bits) {
			std::cout << bit;
		}
		std::cout << '\n';

		std::cout << "test iterator-for-loop, should be equal with above: ";
		for (auto it = bits.begin(); it != bits.end(); ++it) {
			std::cout << it.operator*();
		}
		std::cout << '\n';

		dynamic_bitset<> bits1{std::cin};
		std::cout << "before shift: \n"
				  << bits1 << '\n';
		bits1 <<= 1;

		std::cout << bits1.count() << " bits been set after left shift 1: \n"
				  << bits1 << '\n';
		bits1 >>= 2;
		std::cout << bits1.count() << " bits been set after right shift 2: \n"
				  << bits1 << '\n';

		std::cout << "cast to unsigned long: " << bits1.cast_to<unsigned long>() << std::endl;
	}
}// namespace
