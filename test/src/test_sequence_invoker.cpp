#include <gtest/gtest.h>

#include <array>

#include <gal/sequence_invoker.hpp>

using namespace gal::toolbox::utils;

namespace
{
	constexpr std::array array1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	constexpr std::array array2{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
}

TEST(TestSequenceInvoker, TestTernaryInvoke)
{
	{
		std::array<int, 10> array3{};
		sequence_invoker::ternary_invoke<10>(
				array3,
				array1,
				[](const int a, const int b) { return a + b; },
				8, 6, 4, 2, 0, -2, -4, -6, -8, -10
		);

		ASSERT_EQ(array3[0], array2[0]);
		ASSERT_EQ(array3[1], array2[1]);
		ASSERT_EQ(array3[2], array2[2]);
		ASSERT_EQ(array3[3], array2[3]);
		ASSERT_EQ(array3[4], array2[4]);
		ASSERT_EQ(array3[5], array2[5]);
		ASSERT_EQ(array3[6], array2[6]);
		ASSERT_EQ(array3[7], array2[7]);
		ASSERT_EQ(array3[8], array2[8]);
		ASSERT_EQ(array3[9], array2[9]);
	}
	{
		std::array<int, 10> array3{};
		sequence_invoker::ternary_invoke_seq<10>(
				array3,
				array1,
				array2,
				[](const int a, const int b) { return a + b; }
		);

		ASSERT_EQ(array3[0], array1[0] + array2[0]);
		ASSERT_EQ(array3[1], array1[1] + array2[1]);
		ASSERT_EQ(array3[2], array1[2] + array2[2]);
		ASSERT_EQ(array3[3], array1[3] + array2[3]);
		ASSERT_EQ(array3[4], array1[4] + array2[4]);
		ASSERT_EQ(array3[5], array1[5] + array2[5]);
		ASSERT_EQ(array3[6], array1[6] + array2[6]);
		ASSERT_EQ(array3[7], array1[7] + array2[7]);
		ASSERT_EQ(array3[8], array1[8] + array2[8]);
		ASSERT_EQ(array3[9], array1[9] + array2[9]);

		std::array<int, 10> array4{};
		constexpr auto func = [](int& a, const int b, const int c)
		{
			a = b + c;
		};
		sequence_invoker::ternary_invoke_seq<10>(
				array4,
				array1,
				array2,
				[&func](const int a, const int b) -> int
				{
					int ret{};
					func(ret, a, b);
					return ret;
				}
		);

		ASSERT_EQ(array3[0], array1[0] + array2[0]);
		ASSERT_EQ(array3[1], array1[1] + array2[1]);
		ASSERT_EQ(array3[2], array1[2] + array2[2]);
		ASSERT_EQ(array3[3], array1[3] + array2[3]);
		ASSERT_EQ(array3[4], array1[4] + array2[4]);
		ASSERT_EQ(array3[5], array1[5] + array2[5]);
		ASSERT_EQ(array3[6], array1[6] + array2[6]);
		ASSERT_EQ(array3[7], array1[7] + array2[7]);
		ASSERT_EQ(array3[8], array1[8] + array2[8]);
		ASSERT_EQ(array3[9], array1[9] + array2[9]);
	}
	{
		std::array<int, 10> array3{};
		sequence_invoker::ternary_invoke_dup<10>(
				array3,
				array1,
				10,
				[](const int a, const int b) { return a + b; }
		);

		ASSERT_EQ(array3[0], array1[0] + 10);
		ASSERT_EQ(array3[1], array1[1] + 10);
		ASSERT_EQ(array3[2], array1[2] + 10);
		ASSERT_EQ(array3[3], array1[3] + 10);
		ASSERT_EQ(array3[4], array1[4] + 10);
		ASSERT_EQ(array3[5], array1[5] + 10);
		ASSERT_EQ(array3[6], array1[6] + 10);
		ASSERT_EQ(array3[7], array1[7] + 10);
		ASSERT_EQ(array3[8], array1[8] + 10);
		ASSERT_EQ(array3[9], array1[9] + 10);
	}
}

TEST(TestSequenceInvoker, TestBinaryInvoke)
{
	{
		std::array<int, 10> array3{};
		sequence_invoker::binary_invoke<10>(
				array3,
				[](int& a, const int b) { a = b; },
				1, 2, 3, 4, 5, 6, 7, 8, 9, 10
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);
	}
	{
		std::array<int, 10> array3{};

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke<10, true>(
						array3,
						[](int& a, const int b) { a = b; return a > 0; },
						1, 2, 3, 4, 5, 6, 7, 8, 9, 10
						))
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke<10, true, false>(
						array3,
						[](int& a, const int b) { a = b; return a > 9; },
						1, 2, 3, 4, 5, 6, 7, 8, 9, 10
						))
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);
	}
	{
		std::array<int, 10> array3{};

		sequence_invoker::binary_invoke_seq<10>(
				array3,
				array1,
				[](int& a, const int b) { a = b; }
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);
	}
	{
		std::array<int, 10> array3{};

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke_seq<10, true>(
						array3,
						array1,
						[](int& a, const int b) { a = b; return a > 0; }
						))
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke_seq<10, true, false>(
						array3,
						array1,
						[](int& a, const int b) { a = b; return a > 9; }
						))
		);

		ASSERT_EQ(array3[0], array1[0]);
		ASSERT_EQ(array3[1], array1[1]);
		ASSERT_EQ(array3[2], array1[2]);
		ASSERT_EQ(array3[3], array1[3]);
		ASSERT_EQ(array3[4], array1[4]);
		ASSERT_EQ(array3[5], array1[5]);
		ASSERT_EQ(array3[6], array1[6]);
		ASSERT_EQ(array3[7], array1[7]);
		ASSERT_EQ(array3[8], array1[8]);
		ASSERT_EQ(array3[9], array1[9]);
	}
	{
		std::array<int, 10> array3{};

		sequence_invoker::binary_invoke_dup<10>(
				array3,
				10,
				[](int& a, const int b) { a = b; }
		);

		ASSERT_EQ(array3[0], 10);
		ASSERT_EQ(array3[1], 10);
		ASSERT_EQ(array3[2], 10);
		ASSERT_EQ(array3[3], 10);
		ASSERT_EQ(array3[4], 10);
		ASSERT_EQ(array3[5], 10);
		ASSERT_EQ(array3[6], 10);
		ASSERT_EQ(array3[7], 10);
		ASSERT_EQ(array3[8], 10);
		ASSERT_EQ(array3[9], 10);
	}
	{
		std::array<int, 10> array3{};

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke_dup<10, true>(
						array3,
						10,
						[](int& a, const int b) { a = b; return a > 9; }
						))
		);

		ASSERT_EQ(array3[0], 10);
		ASSERT_EQ(array3[1], 10);
		ASSERT_EQ(array3[2], 10);
		ASSERT_EQ(array3[3], 10);
		ASSERT_EQ(array3[4], 10);
		ASSERT_EQ(array3[5], 10);
		ASSERT_EQ(array3[6], 10);
		ASSERT_EQ(array3[7], 10);
		ASSERT_EQ(array3[8], 10);
		ASSERT_EQ(array3[9], 10);

		ASSERT_TRUE(
				(sequence_invoker::binary_invoke_dup<10, true, false>(
						array3,
						0,
						[](int& a, const int b) { a = b; return a < 1; }
						))
		);

		ASSERT_EQ(array3[0], 0);
		ASSERT_EQ(array3[1], 0);
		ASSERT_EQ(array3[2], 0);
		ASSERT_EQ(array3[3], 0);
		ASSERT_EQ(array3[4], 0);
		ASSERT_EQ(array3[5], 0);
		ASSERT_EQ(array3[6], 0);
		ASSERT_EQ(array3[7], 0);
		ASSERT_EQ(array3[8], 0);
		ASSERT_EQ(array3[9], 0);
	}
}

TEST(TestSequenceInvoker, TestUnaryInvoke)
{
	{
		auto array3 = array1;

		sequence_invoker::unary_invoke<10>(
				array3,
				[](int& i) { i = -i; }
		);

		ASSERT_EQ(array3[0], -array1[0]);
		ASSERT_EQ(array3[1], -array1[1]);
		ASSERT_EQ(array3[2], -array1[2]);
		ASSERT_EQ(array3[3], -array1[3]);
		ASSERT_EQ(array3[4], -array1[4]);
		ASSERT_EQ(array3[5], -array1[5]);
		ASSERT_EQ(array3[6], -array1[6]);
		ASSERT_EQ(array3[7], -array1[7]);
		ASSERT_EQ(array3[8], -array1[8]);
		ASSERT_EQ(array3[9], -array1[9]);
	}
	{
		auto array3 = array2;

		ASSERT_TRUE(
				(sequence_invoker::unary_invoke<10, true>(
						array3,
						[](int& i) { i *= 2; return i >= 0; }
						))
		);

		ASSERT_EQ(array3[0], array2[0] * 2);
		ASSERT_EQ(array3[1], array2[1] * 2);
		ASSERT_EQ(array3[2], array2[2] * 2);
		ASSERT_EQ(array3[3], array2[3] * 2);
		ASSERT_EQ(array3[4], array2[4] * 2);
		ASSERT_EQ(array3[5], array2[5] * 2);
		ASSERT_EQ(array3[6], array2[6] * 2);
		ASSERT_EQ(array3[7], array2[7] * 2);
		ASSERT_EQ(array3[8], array2[8] * 2);
		ASSERT_EQ(array3[9], array2[9] * 2);

		ASSERT_TRUE(
				(sequence_invoker::unary_invoke<10, true, false>(
						array3,
						[](int& i) { i -= 10; return i > 5; }
						))
		);

		ASSERT_EQ(array3[0], array2[0] * 2 - 10);
		ASSERT_EQ(array3[1], array2[1] * 2 - 10);
		ASSERT_EQ(array3[2], array2[2] * 2 - 10);
		ASSERT_EQ(array3[3], array2[3] * 2 - 10);
		ASSERT_EQ(array3[4], array2[4] * 2 - 10);
		ASSERT_EQ(array3[5], array2[5] * 2 - 10);
		ASSERT_EQ(array3[6], array2[6] * 2 - 10);
		ASSERT_EQ(array3[7], array2[7] * 2 - 10);
		ASSERT_EQ(array3[8], array2[8] * 2 - 10);
		ASSERT_EQ(array3[9], array2[9] * 2 - 10);
	}
}
