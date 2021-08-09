#include <gtest/gtest.h>

import gal.toolbox.dynamic_bitset;

#include <bitset>

TEST(TestDynamicBitset, TestConstructAntOutput)
{
	using namespace gal::toolbox;

	std::cout << "foo1(default constructed): \n";
	basic_dynamic_bitset foo1{};
	std::cout << foo1 << "\n\n";
	ASSERT_TRUE(foo1.empty());

	std::cout << "foo2(constructed with std::initializer_list{10, 1023}): \n";
	basic_dynamic_bitset foo2{ 10, 1023 };
	std::cout << foo2 << '\t' << "\n\n";
	ASSERT_EQ(foo2.count(), 12); // 10 -> 2 + 8 = 2bits		1023 = 1024 - 1 = 10bits
	ASSERT_EQ(foo2.size(), basic_dynamic_bitset::bits_of_type * 2);
	ASSERT_EQ(foo2.container_size(), 2);

	std::cout << "foo3(constructed with std::string): \n";
	basic_dynamic_bitset foo31{ std::string{"110"}, 0, 2 };
	basic_dynamic_bitset foo32{ std::string{"110"}, 1 };
	std::cout << foo31 << "\t(use string(\"110\")[0:2] init\n" << foo32 << "\t(use string(\"110\")[1:npos] init" << "\n\n";
	ASSERT_EQ(foo31.count(), 2); // 11
	ASSERT_EQ(foo31.size(), 2);
	ASSERT_EQ(foo31.container_size(), 1);
	ASSERT_EQ(foo32.count(), 1); // 10
	ASSERT_EQ(foo32.size(), 2);
	ASSERT_EQ(foo32.container_size(), 1);

	std::cout << "foo4(constructed with std::string_view): \n";
	basic_dynamic_bitset foo41{ std::string_view{"110"}, 0, 2 };
	basic_dynamic_bitset foo42{ std::string_view{"110"}, 1 };
	std::cout << foo41 << "\t(use string_view(\"110\")[0:2] init\n" << foo42 << "\t(use string_view(\"110\")[1:npos] init" << "\n\n";
	ASSERT_EQ(foo41.count(), 2); // 11
	ASSERT_EQ(foo41.size(), 2);
	ASSERT_EQ(foo41.container_size(), 1);
	ASSERT_EQ(foo42.count(), 1); // 10
	ASSERT_EQ(foo42.size(), 2);
	ASSERT_EQ(foo42.container_size(), 1);

	std::cout << "foo5(constructed with const char*): \n";
	basic_dynamic_bitset foo51{ "110", 0, 2 };
	basic_dynamic_bitset foo52{ "110", 1 };
	std::cout << foo51 << "\t(use const char*(\"110\")[0:2] init\n" << foo52 << "\t(use const char*(\"110\")[1:npos] init" << "\n\n";
	ASSERT_EQ(foo51.count(), 2); // 11
	ASSERT_EQ(foo51.size(), 2);
	ASSERT_EQ(foo51.container_size(), 1);
	ASSERT_EQ(foo52.count(), 1); // 10
	ASSERT_EQ(foo52.size(), 2);
	ASSERT_EQ(foo52.container_size(), 1);

	std::cout << "foo6(constructed with std::vector{1, 2, 3}): \n";
	std::vector vec{ 1, 2, 3 };
	basic_dynamic_bitset foo6{ vec.cbegin(), vec.cend() };
	std::cout << foo6 << "\n\n";
	ASSERT_EQ(foo6.count(), 4); // 1 = 1bit		2 = 1bit		3 = 2bits
	ASSERT_EQ(foo6.size(), basic_dynamic_bitset::bits_of_type * 3);
	ASSERT_EQ(foo6.container_size(), 3);

	std::cout << "foo7(constructed with std::basic_istream): \n";
	basic_dynamic_bitset foo7{ std::cin };
	std::cout << foo7 << "\n\n";

	// first we get a bitset which length is 50 and init by 9999
	// we got 10011100001111 (bin of 9999) and zero-fill --> 00000000000000000000000000000000000010011100001111
	// basic_dynamic_bitset bits{ 50, 9999 };
	// similar to std::vector, if we use { 50, 9999 } for initialization, the compiler will think we want to call std::initializer_list for construction
	basic_dynamic_bitset bits( 50, 9999 );
	ASSERT_EQ(bits.count(), 8); // 10011100001111
	ASSERT_EQ(bits.size(), 50);
	ASSERT_EQ(bits.container_size(), std::ceil(50.f / basic_dynamic_bitset::bits_of_type));
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
