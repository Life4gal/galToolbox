#include <gtest/gtest.h>

#include "../src/container/dynamic_bitset.hpp"

TEST(TestDynamicBitset, TestConstructAntOutput)
{
	using namespace gal::test;

	std::cout << "foo1(default constructed): \n";
	basic_dynamic_bitset foo1{};
	std::cout << foo1 << "\n\n";

	std::cout << "foo2(constructed with std::initializer_list{10, 1023}): \n";
	basic_dynamic_bitset foo2{ 10, 1023 };
	std::cout << foo2 << "\n\n";

	std::cout << "foo3(constructed with std::string): \n";
	basic_dynamic_bitset foo31{ std::string{"110"}, 0, 2 };
	basic_dynamic_bitset foo32{ std::string{"110"}, 1 };
	std::cout << foo31 << "\t(use string(\"110\")[0:2] init\n" << foo32 << "\t(use string(\"110\")[1:npos] init" << "\n\n";

	std::cout << "foo4(constructed with std::string_view): \n";
	basic_dynamic_bitset foo41{ std::string_view{"110"}, 0, 2 };
	basic_dynamic_bitset foo42{ std::string_view{"110"}, 1 };
	std::cout << foo41 << "\t(use string_view(\"110\")[0:2] init\n" << foo42 << "\t(use string_view(\"110\")[1:npos] init" << "\n\n";

	std::cout << "foo5(constructed with const char*): \n";
	basic_dynamic_bitset foo51{ "110", 0, 2 };
	basic_dynamic_bitset foo52{ "110", 1 };
	std::cout << foo51 << "\t(use const char*(\"110\")[0:2] init\n" << foo52 << "\t(use const char*(\"110\")[1:npos] init" << "\n\n";

	std::cout << "foo6(constructed with std::vector{1, 2, 3}): \n";
	std::vector vec{ 1, 2, 3 };
	basic_dynamic_bitset foo6{ vec.cbegin(), vec.cend() };
	std::cout << foo6 << "\n\n";

	std::cout << "foo7(constructed with std::basic_istream): \n";
	basic_dynamic_bitset foo7{ std::cin };
	std::cout << foo7 << "\n\n";

	// first we get a bitset which length is 50 and init by 9999
	// we got 10011100001111 (bin of 9999) and zero-fill --> 00000000000000000000000000000000000010011100001111
	// basic_dynamic_bitset bits{ 50, 9999 };
	// like vector, if we use { 50, 9999 }, compiler think we want to use std::initializer_list to construct bitset
	basic_dynamic_bitset bits( 50, 9999 );
	// this will print them from back, and the output should be 11110000111001000000000000000000000000000000000000
	std::cout << "1: " << bits.count() << " bits been set(output from back to front): \n";
	for (auto&& bit : bits) {
		std::cout << bit;
	}
	std::cout << '\n';

	// set bits between bits[20] and bits[20+20] to true
	bits.set(20, 20, true);
	std::cout << "2: " << bits.count() << " bits been set(output from back to front): \n";
	for (auto&& bit : bits) {
		std::cout << bit;
	}
	std::cout << '\n';

	// this will print them from front
	std::cout << "3: " << bits.count() << " bits been set(output from front to back): \n" << bits << '\n';

	// flip them all
	bits.flip();
	std::cout << "4: " << bits.count() << " bits been set(output from back to front): \n";
	for (auto&& bit : bits) {
		std::cout << bit;
	}
	std::cout << '\n';

	std::cout << "test iterator-for-loop, should be equal with above: \n";
	for (auto it = bits.begin(); it != bits.end(); ++it) {
		std::cout << it.operator*();
	}
	std::cout << '\n';

	std::cout << "foo7(constructed with std::basic_istream): \n";
	basic_dynamic_bitset bits1{ std::cin };
	std::cout << "before shift: \n"
		<< bits1 << '\n';
	bits1 <<= 1;

	std::cout << bits1.count() << " bits been set after left shift 1: \n"
		<< bits1 << '\n';
	bits1 >>= 2;
	std::cout << bits1.count() << " bits been set after right shift 2: \n"
		<< bits1 << '\n';

	std::cout << "cast to unsigned long: " << bits1.cast_to<unsigned long>() << "\ncast to string: " << bits1.to_string() << "\n\n";
}
