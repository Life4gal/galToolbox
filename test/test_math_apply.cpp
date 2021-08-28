#include <gtest/gtest.h>

#include "../src/math/math_apply.hpp"

TEST(TestViewOperations, TestApply)
{
	using namespace gal::test::new_math;

	constexpr vector<int, 4> vector1{ 1, 2, 3, 4 };
	constexpr vector<int, 4> vector2{ 4, 3, 2, 1 };
	constexpr matrix<int, 4, 4> matrix1{ vector1, vector1, vector2, vector2 };
	constexpr matrix<int, 4, 4> matrix2{ vector2, vector2, vector1, vector1 };
	
	{
		{
			constexpr auto vector3 = ternary_apply<int>(
				vector1,
				[](const int a, const int b) { return a + b; },
				3, 1, -1, -3
				);

			static_assert(vector3[0] == vector2[0]);
			static_assert(vector3[1] == vector2[1]);
			static_assert(vector3[2] == vector2[2]);
			static_assert(vector3[3] == vector2[3]);

			constexpr auto matrix3 = ternary_apply<int>(
				matrix1,
				[](const int a, const int b) { return a + b; },
				3, 1, -1, -3, 3, 1, -1, -3, -3, -1, 1, 3, -3, -1, 1, 3
				);

			static_assert(matrix3[0] == matrix2[0]);
			static_assert(matrix3[1] == matrix2[1]);
			static_assert(matrix3[2] == matrix2[2]);
			static_assert(matrix3[3] == matrix2[3]);
			static_assert(matrix3[4] == matrix2[4]);
			static_assert(matrix3[5] == matrix2[5]);
			static_assert(matrix3[6] == matrix2[6]);
			static_assert(matrix3[7] == matrix2[7]);
			static_assert(matrix3[8] == matrix2[8]);
			static_assert(matrix3[9] == matrix2[9]);
			static_assert(matrix3[10] == matrix2[10]);
			static_assert(matrix3[11] == matrix2[11]);
			static_assert(matrix3[12] == matrix2[12]);
			static_assert(matrix3[13] == matrix2[13]);
			static_assert(matrix3[14] == matrix2[14]);
			static_assert(matrix3[15] == matrix2[15]);
		}
		{
			constexpr auto vector3 = ternary_apply_seq<int>(
				vector1,
				vector2,
				[](const int a, const int b) { return a + b; }
			);

			static_assert(vector3[0] == vector1[0] + vector2[0]);
			static_assert(vector3[1] == vector1[1] + vector2[1]);
			static_assert(vector3[2] == vector1[2] + vector2[2]);
			static_assert(vector3[3] == vector1[3] + vector2[3]);

			constexpr auto matrix3 = ternary_apply_seq<int>(
				matrix1,
				matrix2,
				[](const int a, const int b) { return a + b; }
			);

			static_assert(matrix3[0] == matrix1[0] + matrix2[0]);
			static_assert(matrix3[1] == matrix1[1] + matrix2[1]);
			static_assert(matrix3[2] == matrix1[2] + matrix2[2]);
			static_assert(matrix3[3] == matrix1[3] + matrix2[3]);
			static_assert(matrix3[4] == matrix1[4] + matrix2[4]);
			static_assert(matrix3[5] == matrix1[5] + matrix2[5]);
			static_assert(matrix3[6] == matrix1[6] + matrix2[6]);
			static_assert(matrix3[7] == matrix1[7] + matrix2[7]);
			static_assert(matrix3[8] == matrix1[8] + matrix2[8]);
			static_assert(matrix3[9] == matrix1[9] + matrix2[9]);
			static_assert(matrix3[10] == matrix1[10] + matrix2[10]);
			static_assert(matrix3[11] == matrix1[11] + matrix2[11]);
			static_assert(matrix3[12] == matrix1[12] + matrix2[12]);
			static_assert(matrix3[13] == matrix1[13] + matrix2[13]);
			static_assert(matrix3[14] == matrix1[14] + matrix2[14]);
			static_assert(matrix3[15] == matrix1[15] + matrix2[15]);
		}
		{
			constexpr auto vector3 = ternary_apply_dup<int>(
				vector1,
				10,
				[](const int a, const int b) { return a + b; }
			);

			static_assert(vector3[0] == vector1[0] +10);
			static_assert(vector3[1] == vector1[1] +10);
			static_assert(vector3[2] == vector1[2] +10);
			static_assert(vector3[3] == vector1[3] +10);

			constexpr auto matrix3 = ternary_apply_dup<int>(
				matrix1,
				10,
				[](const int a, const int b) { return a + b; }
			);

			static_assert(matrix3[0] == matrix1[0] + 10);
			static_assert(matrix3[1] == matrix1[1] + 10);
			static_assert(matrix3[2] == matrix1[2] + 10);
			static_assert(matrix3[3] == matrix1[3] + 10);
			static_assert(matrix3[4] == matrix1[4] + 10);
			static_assert(matrix3[5] == matrix1[5] + 10);
			static_assert(matrix3[6] == matrix1[6] + 10);
			static_assert(matrix3[7] == matrix1[7] + 10);
			static_assert(matrix3[8] == matrix1[8] + 10);
			static_assert(matrix3[9] == matrix1[9] + 10);
			static_assert(matrix3[10] == matrix1[10] + 10);
			static_assert(matrix3[11] == matrix1[11] + 10);
			static_assert(matrix3[12] == matrix1[12] + 10);
			static_assert(matrix3[13] == matrix1[13] + 10);
			static_assert(matrix3[14] == matrix1[14] + 10);
			static_assert(matrix3[15] == matrix1[15] + 10);
		}
	}
	{
		{
			auto vector3 = vector1;
			binary_apply<false>(
				vector3,
				[](int& a, const int b) { a += b; },
				3, 1, -1, -3
				);

			ASSERT_EQ(vector3[0], vector2[0]);
			ASSERT_EQ(vector3[1], vector2[1]);
			ASSERT_EQ(vector3[2], vector2[2]);
			ASSERT_EQ(vector3[3], vector2[3]);

			ASSERT_TRUE(
				(binary_apply<true, true>(
					vector3,
					[](int& a, const int b) { a += b; return a >= 0; },
					-3, -1, 1, 3
					))
			);

			ASSERT_EQ(vector3[0], vector1[0]);
			ASSERT_EQ(vector3[1], vector1[1]);
			ASSERT_EQ(vector3[2], vector1[2]);
			ASSERT_EQ(vector3[3], vector1[3]);

			ASSERT_TRUE(
				(binary_apply<true, false>(
					vector3,
					[](int& a, const int b) { a += b; return a > 3; },
					3, 1, -1, -3
					))
			);

			ASSERT_EQ(vector3[0], vector2[0]);
			ASSERT_EQ(vector3[1], vector2[1]);
			ASSERT_EQ(vector3[2], vector2[2]);
			ASSERT_EQ(vector3[3], vector2[3]);

			auto matrix3 = matrix1;
			binary_apply<false>(
				matrix3,
				[](int& a, const int b) { a += b; },
				3, 1, -1, -3, 3, 1, -1, -3, -3, -1, 1, 3, -3, -1, 1, 3
				);

			ASSERT_EQ(matrix3[0], matrix2[0]);
			ASSERT_EQ(matrix3[1], matrix2[1]);
			ASSERT_EQ(matrix3[2], matrix2[2]);
			ASSERT_EQ(matrix3[3], matrix2[3]);
			ASSERT_EQ(matrix3[4], matrix2[4]);
			ASSERT_EQ(matrix3[5], matrix2[5]);
			ASSERT_EQ(matrix3[6], matrix2[6]);
			ASSERT_EQ(matrix3[7], matrix2[7]);
			ASSERT_EQ(matrix3[8], matrix2[8]);
			ASSERT_EQ(matrix3[9], matrix2[9]);
			ASSERT_EQ(matrix3[10], matrix2[10]);
			ASSERT_EQ(matrix3[11], matrix2[11]);
			ASSERT_EQ(matrix3[12], matrix2[12]);
			ASSERT_EQ(matrix3[13], matrix2[13]);
			ASSERT_EQ(matrix3[14], matrix2[14]);
			ASSERT_EQ(matrix3[15], matrix2[15]);

			ASSERT_TRUE(
				(binary_apply<true, true>(
					matrix3,
					[](int& a, const int b) { a += b; return a >= 0; },
					-3, -1, 1, 3, -3, -1, 1, 3, 3, 1, -1, -3, 3, 1, -1, -3
					))
			);

			ASSERT_EQ(matrix3[0], matrix1[0]);
			ASSERT_EQ(matrix3[1], matrix1[1]);
			ASSERT_EQ(matrix3[2], matrix1[2]);
			ASSERT_EQ(matrix3[3], matrix1[3]);
			ASSERT_EQ(matrix3[4], matrix1[4]);
			ASSERT_EQ(matrix3[5], matrix1[5]);
			ASSERT_EQ(matrix3[6], matrix1[6]);
			ASSERT_EQ(matrix3[7], matrix1[7]);
			ASSERT_EQ(matrix3[8], matrix1[8]);
			ASSERT_EQ(matrix3[9], matrix1[9]);
			ASSERT_EQ(matrix3[10], matrix1[10]);
			ASSERT_EQ(matrix3[11], matrix1[11]);
			ASSERT_EQ(matrix3[12], matrix1[12]);
			ASSERT_EQ(matrix3[13], matrix1[13]);
			ASSERT_EQ(matrix3[14], matrix1[14]);
			ASSERT_EQ(matrix3[15], matrix1[15]);

			ASSERT_TRUE(
				(binary_apply<true, false>(
					matrix3,
					[](int& a, const int b) { a += b; return a >= 3; },
					3, 1, -1, -3, 3, 1, -1, -3, -3, -1, 1, 3, -3, -1, 1, 3
					))
			);

			ASSERT_EQ(matrix3[0], matrix2[0]);
			ASSERT_EQ(matrix3[1], matrix2[1]);
			ASSERT_EQ(matrix3[2], matrix2[2]);
			ASSERT_EQ(matrix3[3], matrix2[3]);
			ASSERT_EQ(matrix3[4], matrix2[4]);
			ASSERT_EQ(matrix3[5], matrix2[5]);
			ASSERT_EQ(matrix3[6], matrix2[6]);
			ASSERT_EQ(matrix3[7], matrix2[7]);
			ASSERT_EQ(matrix3[8], matrix2[8]);
			ASSERT_EQ(matrix3[9], matrix2[9]);
			ASSERT_EQ(matrix3[10], matrix2[10]);
			ASSERT_EQ(matrix3[11], matrix2[11]);
			ASSERT_EQ(matrix3[12], matrix2[12]);
			ASSERT_EQ(matrix3[13], matrix2[13]);
			ASSERT_EQ(matrix3[14], matrix2[14]);
			ASSERT_EQ(matrix3[15], matrix2[15]);
		}
		{
			auto vector3 = vector1;
			binary_apply_seq<false>(
				vector3,
				vector2,
				[](int& a, const int b) { a += b; }
			);

			ASSERT_EQ(vector3[0], vector1[0] + vector2[0]);
			ASSERT_EQ(vector3[1], vector1[1] + vector2[1]);
			ASSERT_EQ(vector3[2], vector1[2] + vector2[2]);
			ASSERT_EQ(vector3[3], vector1[3] + vector2[3]);

			ASSERT_TRUE(
				(binary_apply_seq<true, true>(
						vector3,
						vector2,
						[](int& a, const int b) { a -= b; return a >= 0; }
						))
			);

			ASSERT_EQ(vector3[0], vector1[0]);
			ASSERT_EQ(vector3[1], vector1[1]);
			ASSERT_EQ(vector3[2], vector1[2]);
			ASSERT_EQ(vector3[3], vector1[3]);

			ASSERT_TRUE(
				(binary_apply_seq<true, false>(
					vector3,
					vector2,
					[](int& a, const int b) { a += b; return a >= 3; }
					))
			);

			ASSERT_EQ(vector3[0], vector1[0] + vector2[0]);
			ASSERT_EQ(vector3[1], vector1[1] + vector2[1]);
			ASSERT_EQ(vector3[2], vector1[2] + vector2[2]);
			ASSERT_EQ(vector3[3], vector1[3] + vector2[3]);
		}
		{
			auto matrix3 = matrix1;
			binary_apply_dup<false>(
				matrix3,
				10,
				[](int& a, const int b) { a += b; }
			);

			ASSERT_EQ(matrix3[0], matrix1[0] + 10);
			ASSERT_EQ(matrix3[1], matrix1[1] + 10);
			ASSERT_EQ(matrix3[2], matrix1[2] + 10);
			ASSERT_EQ(matrix3[3], matrix1[3] + 10);
			ASSERT_EQ(matrix3[4], matrix1[4] + 10);
			ASSERT_EQ(matrix3[5], matrix1[5] + 10);
			ASSERT_EQ(matrix3[6], matrix1[6] + 10);
			ASSERT_EQ(matrix3[7], matrix1[7] + 10);
			ASSERT_EQ(matrix3[8], matrix1[8] + 10);
			ASSERT_EQ(matrix3[9], matrix1[9] + 10);
			ASSERT_EQ(matrix3[10], matrix1[10] + 10);
			ASSERT_EQ(matrix3[11], matrix1[11] + 10);
			ASSERT_EQ(matrix3[12], matrix1[12] + 10);
			ASSERT_EQ(matrix3[13], matrix1[13] + 10);
			ASSERT_EQ(matrix3[14], matrix1[14] + 10);
			ASSERT_EQ(matrix3[15], matrix1[15] + 10);

			ASSERT_TRUE(
				(binary_apply_dup<true, true>(
					matrix3,
					10,
					[](int& a, const int b) { a -= b; return a <= 10; }
					))
			);

			ASSERT_EQ(matrix3[0], matrix1[0]);
			ASSERT_EQ(matrix3[1], matrix1[1]);
			ASSERT_EQ(matrix3[2], matrix1[2]);
			ASSERT_EQ(matrix3[3], matrix1[3]);
			ASSERT_EQ(matrix3[4], matrix1[4]);
			ASSERT_EQ(matrix3[5], matrix1[5]);
			ASSERT_EQ(matrix3[6], matrix1[6]);
			ASSERT_EQ(matrix3[7], matrix1[7]);
			ASSERT_EQ(matrix3[8], matrix1[8]);
			ASSERT_EQ(matrix3[9], matrix1[9]);
			ASSERT_EQ(matrix3[10], matrix1[10]);
			ASSERT_EQ(matrix3[11], matrix1[11]);
			ASSERT_EQ(matrix3[12], matrix1[12]);
			ASSERT_EQ(matrix3[13], matrix1[13]);
			ASSERT_EQ(matrix3[14], matrix1[14]);
			ASSERT_EQ(matrix3[15], matrix1[15]);

			ASSERT_TRUE(
				(binary_apply_dup<true, false>(
					matrix3,
					10,
					[](int& a, const int b) { a += b; return a >= 10; }
					))
			);

			ASSERT_EQ(matrix3[0], matrix1[0] + 10);
			ASSERT_EQ(matrix3[1], matrix1[1] + 10);
			ASSERT_EQ(matrix3[2], matrix1[2] + 10);
			ASSERT_EQ(matrix3[3], matrix1[3] + 10);
			ASSERT_EQ(matrix3[4], matrix1[4] + 10);
			ASSERT_EQ(matrix3[5], matrix1[5] + 10);
			ASSERT_EQ(matrix3[6], matrix1[6] + 10);
			ASSERT_EQ(matrix3[7], matrix1[7] + 10);
			ASSERT_EQ(matrix3[8], matrix1[8] + 10);
			ASSERT_EQ(matrix3[9], matrix1[9] + 10);
			ASSERT_EQ(matrix3[10], matrix1[10] + 10);
			ASSERT_EQ(matrix3[11], matrix1[11] + 10);
			ASSERT_EQ(matrix3[12], matrix1[12] + 10);
			ASSERT_EQ(matrix3[13], matrix1[13] + 10);
			ASSERT_EQ(matrix3[14], matrix1[14] + 10);
			ASSERT_EQ(matrix3[15], matrix1[15] + 10);
		}
	}
	{
		auto vector3 = vector1;
		unary_apply<false>(
			vector3,
			[](int& i) {i *= 2; }
		);

		ASSERT_EQ(vector3[0], vector1[0] * 2);
		ASSERT_EQ(vector3[1], vector1[1] * 2);
		ASSERT_EQ(vector3[2], vector1[2] * 2);
		ASSERT_EQ(vector3[3], vector1[3] * 2);

		ASSERT_TRUE(
			(unary_apply<true, true>(
				vector3,
				[](int& i) {i -= 1; return i >= 0; }
				))
		);

		ASSERT_EQ(vector3[0], vector1[0] * 2 - 1);
		ASSERT_EQ(vector3[1], vector1[1] * 2 - 1);
		ASSERT_EQ(vector3[2], vector1[2] * 2 - 1);
		ASSERT_EQ(vector3[3], vector1[3] * 2 - 1);

		ASSERT_TRUE(
			(unary_apply<true, false>(
				vector3,
				[](int& i) {i -= 1; return i >= 5; }
				))
		);

		ASSERT_EQ(vector3[0], vector1[0] * 2 - 2);
		ASSERT_EQ(vector3[1], vector1[1] * 2 - 2);
		ASSERT_EQ(vector3[2], vector1[2] * 2 - 2);
		ASSERT_EQ(vector3[3], vector1[3] * 2 - 2);
	}
}


