#include <gtest/gtest.h>

#include "../src/math/math_operations.hpp"

using namespace gal::test::math;

TEST(TestMathOperations, TestOperatorAdd)
{
	constexpr vector3<int> vector1{1, 2, 3};
	constexpr vector3<int> vector2{3, 2, 1};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix + matrix + value + matrix
		((matrix3 += matrix2) += 42) += matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] + matrix2[0] + 42 + matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] + matrix2[1] + 42 + matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] + matrix2[2] + 42 + matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] + matrix2[3] + 42 + matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] + matrix2[4] + 42 + matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] + matrix2[5] + 42 + matrix1[5]);

		auto vector3 = vector1;

		// vector + vector + vector_view + vector + value + vector
		(((vector3 += vector2).to_view() += vector2) += 42) += vector1;

		ASSERT_EQ(vector3[0], vector1[0] + vector2[0] + vector2[0] + 42 + vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] + vector2[1] + vector2[1] + 42 + vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] + vector2[2] + vector2[2] + 42 + vector1[2]);
	}
	{
		// matrix + matrix + value + matrix
		constexpr auto matrix31 = matrix1 + matrix2 + 1 + matrix1;

		static_assert(matrix31[0] == matrix1[0] + matrix2[0] + 1 + matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] + matrix2[1] + 1 + matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] + matrix2[2] + 1 + matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] + matrix2[3] + 1 + matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] + matrix2[4] + 1 + matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] + matrix2[5] + 1 + matrix1[5]);

		// value + matrix + value + matrix
		constexpr auto matrix32 = 42 + matrix1 + 1 + matrix2;

		static_assert(matrix32[0] == 42 + matrix1[0] + 1 + matrix2[0]);
		static_assert(matrix32[1] == 42 + matrix1[1] + 1 + matrix2[1]);
		static_assert(matrix32[2] == 42 + matrix1[2] + 1 + matrix2[2]);
		static_assert(matrix32[3] == 42 + matrix1[3] + 1 + matrix2[3]);
		static_assert(matrix32[4] == 42 + matrix1[4] + 1 + matrix2[4]);
		static_assert(matrix32[5] == 42 + matrix1[5] + 1 + matrix2[5]);

		// vector + vector + value + vector_view + vector
		constexpr auto vector31 = vector1 + vector2 + 42 + vector1.to_view() + vector2;

		static_assert(vector31[0] == vector1[0] + vector2[0] + 42 + vector1[0] + vector2[0]);
		static_assert(vector31[1] == vector1[1] + vector2[1] + 42 + vector1[1] + vector2[1]);
		static_assert(vector31[2] == vector1[2] + vector2[2] + 42 + vector1[2] + vector2[2]);

		// value + vector + vector_view + vector
		constexpr auto vector32 = 42 + vector1 + vector2.to_view() + vector2;

		static_assert(vector32[0] == 42 + vector1[0] + vector2[0] + vector2[0]);
		static_assert(vector32[1] == 42 + vector1[1] + vector2[1] + vector2[1]);
		static_assert(vector32[2] == 42 + vector1[2] + vector2[2] + vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorSubtract)
{
	constexpr vector3<int> vector1{1, 2, 3};
	constexpr vector3<int> vector2{3, 2, 1};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix - matrix - value - matrix
		((matrix3 -= matrix2) -= 42) -= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] - matrix2[0] - 42 - matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] - matrix2[1] - 42 - matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] - matrix2[2] - 42 - matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] - matrix2[3] - 42 - matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] - matrix2[4] - 42 - matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] - matrix2[5] - 42 - matrix1[5]);

		auto vector3 = vector1;

		// vector - vector - vector_view - vector - value - vector
		(((vector3 -= vector2).to_view() -= vector2) -= 42) -= vector1;

		ASSERT_EQ(vector3[0], vector1[0] - vector2[0] - vector2[0] - 42 - vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] - vector2[1] - vector2[1] - 42 - vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] - vector2[2] - vector2[2] - 42 - vector1[2]);
	}
	{
		// matrix - matrix - value - matrix
		constexpr auto matrix31 = matrix1 - matrix2 - 1 - matrix1;

		static_assert(matrix31[0] == matrix1[0] - matrix2[0] - 1 - matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] - matrix2[1] - 1 - matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] - matrix2[2] - 1 - matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] - matrix2[3] - 1 - matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] - matrix2[4] - 1 - matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] - matrix2[5] - 1 - matrix1[5]);

		// value - matrix - value - matrix
		constexpr auto matrix32 = 42 - matrix1 - 1 - matrix2;

		static_assert(matrix32[0] == 42 - matrix1[0] - 1 - matrix2[0]);
		static_assert(matrix32[1] == 42 - matrix1[1] - 1 - matrix2[1]);
		static_assert(matrix32[2] == 42 - matrix1[2] - 1 - matrix2[2]);
		static_assert(matrix32[3] == 42 - matrix1[3] - 1 - matrix2[3]);
		static_assert(matrix32[4] == 42 - matrix1[4] - 1 - matrix2[4]);
		static_assert(matrix32[5] == 42 - matrix1[5] - 1 - matrix2[5]);

		// vector - vector - value - vector_view - vector
		constexpr auto vector3 = vector1 - vector2 - 42 - vector1.to_view() - vector2;

		static_assert(vector3[0] == vector1[0] - vector2[0] - 42 - vector1[0] - vector2[0]);
		static_assert(vector3[1] == vector1[1] - vector2[1] - 42 - vector1[1] - vector2[1]);
		static_assert(vector3[2] == vector1[2] - vector2[2] - 42 - vector1[2] - vector2[2]);

		// value - vector - vector_view - vector
		constexpr auto vector32 = 42 - vector1 - vector2.to_view() - vector2;

		static_assert(vector32[0] == 42 - vector1[0] - vector2[0] - vector2[0]);
		static_assert(vector32[1] == 42 - vector1[1] - vector2[1] - vector2[1]);
		static_assert(vector32[2] == 42 - vector1[2] - vector2[2] - vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorMultiply)
{
	constexpr vector3<int> vector1{1, 2, 3};
	constexpr vector3<int> vector2{3, 2, 1};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix * matrix * value * matrix
		((matrix3 *= matrix2) *= 42) *= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] * matrix2[0] * 42 * matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] * matrix2[1] * 42 * matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] * matrix2[2] * 42 * matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] * matrix2[3] * 42 * matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] * matrix2[4] * 42 * matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] * matrix2[5] * 42 * matrix1[5]);

		auto vector3 = vector1;

		// vector * vector * vector_view * vector * value * vector
		(((vector3 *= vector2).to_view() *= vector2) *= 42) *= vector1;

		ASSERT_EQ(vector3[0], vector1[0] * vector2[0] * vector2[0] * 42 * vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] * vector2[1] * vector2[1] * 42 * vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] * vector2[2] * vector2[2] * 42 * vector1[2]);
	}
	{
		// matrix * matrix * value * matrix
		constexpr auto matrix31 = matrix1 * matrix2 * 1 * matrix1;

		static_assert(matrix31[0] == matrix1[0] * matrix2[0] * 1 * matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] * matrix2[1] * 1 * matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] * matrix2[2] * 1 * matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] * matrix2[3] * 1 * matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] * matrix2[4] * 1 * matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] * matrix2[5] * 1 * matrix1[5]);

		// value * matrix * value * matrix
		constexpr auto matrix32 = 42 * matrix1 * 1 * matrix2;

		static_assert(matrix32[0] == 42 * matrix1[0] * 1 * matrix2[0]);
		static_assert(matrix32[1] == 42 * matrix1[1] * 1 * matrix2[1]);
		static_assert(matrix32[2] == 42 * matrix1[2] * 1 * matrix2[2]);
		static_assert(matrix32[3] == 42 * matrix1[3] * 1 * matrix2[3]);
		static_assert(matrix32[4] == 42 * matrix1[4] * 1 * matrix2[4]);
		static_assert(matrix32[5] == 42 * matrix1[5] * 1 * matrix2[5]);

		// vector * vector * value * vector_view * vector
		constexpr auto vector3 = vector1 * vector2 * 42 * vector1.to_view() * vector2;

		static_assert(vector3[0] == vector1[0] * vector2[0] * 42 * vector1[0] * vector2[0]);
		static_assert(vector3[1] == vector1[1] * vector2[1] * 42 * vector1[1] * vector2[1]);
		static_assert(vector3[2] == vector1[2] * vector2[2] * 42 * vector1[2] * vector2[2]);

		// value * vector * vector_view * vector
		constexpr auto vector32 = 42 * vector1 * vector2.to_view() * vector2;

		static_assert(vector32[0] == 42 * vector1[0] * vector2[0] * vector2[0]);
		static_assert(vector32[1] == 42 * vector1[1] * vector2[1] * vector2[1]);
		static_assert(vector32[2] == 42 * vector1[2] * vector2[2] * vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorDivide)
{
	constexpr vector3<double> vector1{1, 2, 3};
	constexpr vector3<double> vector2{3, 2, 1};

	constexpr matrix2x3<double> matrix1{vector1, vector2};
	constexpr matrix2x3<double> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix / matrix / value / matrix
		((matrix3 /= matrix2) /= 42) /= matrix1;

		ASSERT_DOUBLE_EQ(matrix3[0], matrix1[0] / matrix2[0] / 42 / matrix1[0]);
		ASSERT_DOUBLE_EQ(matrix3[1], matrix1[1] / matrix2[1] / 42 / matrix1[1]);
		ASSERT_DOUBLE_EQ(matrix3[2], matrix1[2] / matrix2[2] / 42 / matrix1[2]);
		ASSERT_DOUBLE_EQ(matrix3[3], matrix1[3] / matrix2[3] / 42 / matrix1[3]);
		ASSERT_DOUBLE_EQ(matrix3[4], matrix1[4] / matrix2[4] / 42 / matrix1[4]);
		ASSERT_DOUBLE_EQ(matrix3[5], matrix1[5] / matrix2[5] / 42 / matrix1[5]);

		auto vector3 = vector1;

		// vector / vector / vector_view / vector / value / vector
		(((vector3 /= vector2).to_view() /= vector2) /= 42) /= vector1;

		ASSERT_DOUBLE_EQ(vector3[0], vector1[0] / vector2[0] / vector2[0] / 42 / vector1[0]);
		ASSERT_DOUBLE_EQ(vector3[1], vector1[1] / vector2[1] / vector2[1] / 42 / vector1[1]);
		ASSERT_DOUBLE_EQ(vector3[2], vector1[2] / vector2[2] / vector2[2] / 42 / vector1[2]);
	}
	{
		constexpr auto double_eq = [](const double a, const double b) constexpr
		{
			return
				(a - b) < 0.0000001 &&
				(a - b) > -0.0000001;
		};

		// matrix / matrix / value / matrix
		constexpr auto matrix31 = matrix1 / matrix2 / 1 / matrix1;

		static_assert(double_eq(matrix31[0], matrix1[0] / matrix2[0] / 1 / matrix1[0]));
		static_assert(double_eq(matrix31[1], matrix1[1] / matrix2[1] / 1 / matrix1[1]));
		static_assert(double_eq(matrix31[2], matrix1[2] / matrix2[2] / 1 / matrix1[2]));
		static_assert(double_eq(matrix31[3], matrix1[3] / matrix2[3] / 1 / matrix1[3]));
		static_assert(double_eq(matrix31[4], matrix1[4] / matrix2[4] / 1 / matrix1[4]));
		static_assert(double_eq(matrix31[5], matrix1[5] / matrix2[5] / 1 / matrix1[5]));

		// value / matrix / value / matrix
		constexpr auto matrix32 = 42.0 / matrix1 / 1 / matrix2;

		static_assert(double_eq(matrix32[0], 42 / matrix1[0] / 1 / matrix2[0]));
		static_assert(double_eq(matrix32[1], 42 / matrix1[1] / 1 / matrix2[1]));
		static_assert(double_eq(matrix32[2], 42 / matrix1[2] / 1 / matrix2[2]));
		static_assert(double_eq(matrix32[3], 42 / matrix1[3] / 1 / matrix2[3]));
		static_assert(double_eq(matrix32[4], 42 / matrix1[4] / 1 / matrix2[4]));
		static_assert(double_eq(matrix32[5], 42 / matrix1[5] / 1 / matrix2[5]));

		// vector / vector / value / vector_view / vector
		constexpr auto vector3 = vector1 / vector2 / 42 / vector1.to_view() / vector2;

		static_assert(double_eq(vector3[0], vector1[0] / vector2[0] / 42 / vector1[0] / vector2[0]));
		static_assert(double_eq(vector3[1], vector1[1] / vector2[1] / 42 / vector1[1] / vector2[1]));
		static_assert(double_eq(vector3[2], vector1[2] / vector2[2] / 42 / vector1[2] / vector2[2]));

		// value / vector / vector_view / vector
		constexpr auto vector32 = 42.0 / vector1 / vector2.to_view() / vector2;

		static_assert(double_eq(vector32[0], 42 / vector1[0] / vector2[0] / vector2[0]));
		static_assert(double_eq(vector32[1], 42 / vector1[1] / vector2[1] / vector2[1]));
		static_assert(double_eq(vector32[2], 42 / vector1[2] / vector2[2] / vector2[2]));
	}
}

TEST(TestMathOperations, TestOperatorModel)
{
	constexpr vector3<int> vector1{123, 456, 789};
	constexpr vector3<int> vector2{987, 654, 321};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix % matrix % value % matrix
		((matrix3 %= matrix2) %= 42) %= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] % matrix2[0] % 42 % matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] % matrix2[1] % 42 % matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] % matrix2[2] % 42 % matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] % matrix2[3] % 42 % matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] % matrix2[4] % 42 % matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] % matrix2[5] % 42 % matrix1[5]);

		auto vector3 = vector1;

		// vector % vector % vector_view % vector % value % vector
		(((vector3 %= vector2).to_view() %= vector2) %= 42) %= vector1;

		ASSERT_EQ(vector3[0], vector1[0] % vector2[0] % vector2[0] % 42 % vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] % vector2[1] % vector2[1] % 42 % vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] % vector2[2] % vector2[2] % 42 % vector1[2]);
	}
	{
		// matrix % matrix % value % matrix
		constexpr auto matrix31 = matrix1 % matrix2 % 1 % matrix1;

		static_assert(matrix31[0] == matrix1[0] % matrix2[0] % 1 % matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] % matrix2[1] % 1 % matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] % matrix2[2] % 1 % matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] % matrix2[3] % 1 % matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] % matrix2[4] % 1 % matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] % matrix2[5] % 1 % matrix1[5]);

		// value % matrix % value % matrix
		constexpr auto matrix32 = 42 % matrix1 % 1 % matrix2;

		static_assert(matrix32[0] == 42 % matrix1[0] % 1 % matrix2[0]);
		static_assert(matrix32[1] == 42 % matrix1[1] % 1 % matrix2[1]);
		static_assert(matrix32[2] == 42 % matrix1[2] % 1 % matrix2[2]);
		static_assert(matrix32[3] == 42 % matrix1[3] % 1 % matrix2[3]);
		static_assert(matrix32[4] == 42 % matrix1[4] % 1 % matrix2[4]);
		static_assert(matrix32[5] == 42 % matrix1[5] % 1 % matrix2[5]);

		// vector % vector % value % vector_view % vector
		constexpr auto vector3 = vector1 % vector2 % 42 % vector1.to_view() % vector2;

		static_assert(vector3[0] == vector1[0] % vector2[0] % 42 % vector1[0] % vector2[0]);
		static_assert(vector3[1] == vector1[1] % vector2[1] % 42 % vector1[1] % vector2[1]);
		static_assert(vector3[2] == vector1[2] % vector2[2] % 42 % vector1[2] % vector2[2]);

		// value % vector % vector_view % vector
		constexpr auto vector32 = 42 % vector1 % vector2.to_view() % vector2;

		static_assert(vector32[0] == 42 % vector1[0] % vector2[0] % vector2[0]);
		static_assert(vector32[1] == 42 % vector1[1] % vector2[1] % vector2[1]);
		static_assert(vector32[2] == 42 % vector1[2] % vector2[2] % vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorBitAnd)
{
	constexpr vector3<int> vector1{123, 456, 789};
	constexpr vector3<int> vector2{987, 654, 321};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix & matrix & value & matrix
		((matrix3 &= matrix2) &= 42) &= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] & matrix2[0] & 42 & matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] & matrix2[1] & 42 & matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] & matrix2[2] & 42 & matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] & matrix2[3] & 42 & matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] & matrix2[4] & 42 & matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] & matrix2[5] & 42 & matrix1[5]);

		auto vector3 = vector1;

		// vector & vector & vector_view & vector & value & vector
		(((vector3 &= vector2).to_view() &= vector2) &= 42) &= vector1;

		ASSERT_EQ(vector3[0], vector1[0] & vector2[0] & vector2[0] & 42 & vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] & vector2[1] & vector2[1] & 42 & vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] & vector2[2] & vector2[2] & 42 & vector1[2]);
	}
	{
		// matrix & matrix & value & matrix
		constexpr auto matrix31 = matrix1 & matrix2 & 1 & matrix1;

		static_assert(matrix31[0] == (matrix1[0] & matrix2[0] & 1 & matrix1[0]));
		static_assert(matrix31[1] == (matrix1[1] & matrix2[1] & 1 & matrix1[1]));
		static_assert(matrix31[2] == (matrix1[2] & matrix2[2] & 1 & matrix1[2]));
		static_assert(matrix31[3] == (matrix1[3] & matrix2[3] & 1 & matrix1[3]));
		static_assert(matrix31[4] == (matrix1[4] & matrix2[4] & 1 & matrix1[4]));
		static_assert(matrix31[5] == (matrix1[5] & matrix2[5] & 1 & matrix1[5]));

		// value & matrix & value & matrix
		constexpr auto matrix32 = 42 & matrix1 & 1 & matrix2;

		static_assert(matrix32[0] == (42 & matrix1[0] & 1 & matrix2[0]));
		static_assert(matrix32[1] == (42 & matrix1[1] & 1 & matrix2[1]));
		static_assert(matrix32[2] == (42 & matrix1[2] & 1 & matrix2[2]));
		static_assert(matrix32[3] == (42 & matrix1[3] & 1 & matrix2[3]));
		static_assert(matrix32[4] == (42 & matrix1[4] & 1 & matrix2[4]));
		static_assert(matrix32[5] == (42 & matrix1[5] & 1 & matrix2[5]));

		// vector & vector & value & vector_view & vector
		constexpr auto vector3 = vector1 & vector2 & 42 & vector1.to_view() & vector2;

		static_assert(vector3[0] == (vector1[0] & vector2[0] & 42 & vector1[0] & vector2[0]));
		static_assert(vector3[1] == (vector1[1] & vector2[1] & 42 & vector1[1] & vector2[1]));
		static_assert(vector3[2] == (vector1[2] & vector2[2] & 42 & vector1[2] & vector2[2]));

		// value & vector & vector_view & vector
		constexpr auto vector32 = 42 & vector1 & vector2.to_view() & vector2;

		static_assert(vector32[0] == (42 & vector1[0] & vector2[0] & vector2[0]));
		static_assert(vector32[1] == (42 & vector1[1] & vector2[1] & vector2[1]));
		static_assert(vector32[2] == (42 & vector1[2] & vector2[2] & vector2[2]));
	}
}

TEST(TestMathOperations, TestOperatorBitOr)
{
	constexpr vector3<int> vector1{123, 456, 789};
	constexpr vector3<int> vector2{987, 654, 321};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix | matrix | value | matrix
		((matrix3 |= matrix2) |= 42) |= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] | matrix2[0] | 42 | matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] | matrix2[1] | 42 | matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] | matrix2[2] | 42 | matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] | matrix2[3] | 42 | matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] | matrix2[4] | 42 | matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] | matrix2[5] | 42 | matrix1[5]);

		auto vector3 = vector1;

		// vector | vector | vector_view | vector | value | vector
		(((vector3 |= vector2).to_view() |= vector2) |= 42) |= vector1;

		ASSERT_EQ(vector3[0], vector1[0] | vector2[0] | vector2[0] | 42 | vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] | vector2[1] | vector2[1] | 42 | vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] | vector2[2] | vector2[2] | 42 | vector1[2]);
	}
	{
		// matrix | matrix | value | matrix
		constexpr auto matrix31 = matrix1 | matrix2 | 1 | matrix1;

		static_assert(matrix31[0] == (matrix1[0] | matrix2[0] | 1 | matrix1[0]));
		static_assert(matrix31[1] == (matrix1[1] | matrix2[1] | 1 | matrix1[1]));
		static_assert(matrix31[2] == (matrix1[2] | matrix2[2] | 1 | matrix1[2]));
		static_assert(matrix31[3] == (matrix1[3] | matrix2[3] | 1 | matrix1[3]));
		static_assert(matrix31[4] == (matrix1[4] | matrix2[4] | 1 | matrix1[4]));
		static_assert(matrix31[5] == (matrix1[5] | matrix2[5] | 1 | matrix1[5]));

		// value | matrix | value | matrix
		constexpr auto matrix32 = 42 | matrix1 | 1 | matrix2;

		static_assert(matrix32[0] == (42 | matrix1[0] | 1 | matrix2[0]));
		static_assert(matrix32[1] == (42 | matrix1[1] | 1 | matrix2[1]));
		static_assert(matrix32[2] == (42 | matrix1[2] | 1 | matrix2[2]));
		static_assert(matrix32[3] == (42 | matrix1[3] | 1 | matrix2[3]));
		static_assert(matrix32[4] == (42 | matrix1[4] | 1 | matrix2[4]));
		static_assert(matrix32[5] == (42 | matrix1[5] | 1 | matrix2[5]));

		// vector | vector | value | vector_view | vector
		constexpr auto vector3 = vector1 | vector2 | 42 | vector1.to_view() | vector2;

		static_assert(vector3[0] == (vector1[0] | vector2[0] | 42 | vector1[0] | vector2[0]));
		static_assert(vector3[1] == (vector1[1] | vector2[1] | 42 | vector1[1] | vector2[1]));
		static_assert(vector3[2] == (vector1[2] | vector2[2] | 42 | vector1[2] | vector2[2]));

		// value | vector | vector_view | vector
		constexpr auto vector32 = 42 | vector1 | vector2.to_view() | vector2;

		static_assert(vector32[0] == (42 | vector1[0] | vector2[0] | vector2[0]));
		static_assert(vector32[1] == (42 | vector1[1] | vector2[1] | vector2[1]));
		static_assert(vector32[2] == (42 | vector1[2] | vector2[2] | vector2[2]));
	}
}

TEST(TestMathOperations, TestOperatorBitXor)
{
	constexpr vector3<int> vector1{123, 456, 789};
	constexpr vector3<int> vector2{987, 654, 321};

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix ^ matrix ^ value ^ matrix
		((matrix3 ^= matrix2) ^= 42) ^= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] ^ matrix2[0] ^ 42 ^ matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] ^ matrix2[1] ^ 42 ^ matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] ^ matrix2[2] ^ 42 ^ matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] ^ matrix2[3] ^ 42 ^ matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] ^ matrix2[4] ^ 42 ^ matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] ^ matrix2[5] ^ 42 ^ matrix1[5]);

		auto vector3 = vector1;

		// vector ^ vector ^ vector_view ^ vector ^ value ^ vector
		(((vector3 ^= vector2).to_view() ^= vector2) ^= 42) ^= vector1;

		ASSERT_EQ(vector3[0], vector1[0] ^ vector2[0] ^ vector2[0] ^ 42 ^ vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] ^ vector2[1] ^ vector2[1] ^ 42 ^ vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] ^ vector2[2] ^ vector2[2] ^ 42 ^ vector1[2]);
	}
	{
		// matrix ^ matrix ^ value ^ matrix
		constexpr auto matrix31 = matrix1 ^ matrix2 ^ 1 ^ matrix1;

		static_assert(matrix31[0] == (matrix1[0] ^ matrix2[0] ^ 1 ^ matrix1[0]));
		static_assert(matrix31[1] == (matrix1[1] ^ matrix2[1] ^ 1 ^ matrix1[1]));
		static_assert(matrix31[2] == (matrix1[2] ^ matrix2[2] ^ 1 ^ matrix1[2]));
		static_assert(matrix31[3] == (matrix1[3] ^ matrix2[3] ^ 1 ^ matrix1[3]));
		static_assert(matrix31[4] == (matrix1[4] ^ matrix2[4] ^ 1 ^ matrix1[4]));
		static_assert(matrix31[5] == (matrix1[5] ^ matrix2[5] ^ 1 ^ matrix1[5]));

		// value ^ matrix ^ value ^ matrix
		constexpr auto matrix32 = 42 ^ matrix1 ^ 1 ^ matrix2;

		static_assert(matrix32[0] == (42 ^ matrix1[0] ^ 1 ^ matrix2[0]));
		static_assert(matrix32[1] == (42 ^ matrix1[1] ^ 1 ^ matrix2[1]));
		static_assert(matrix32[2] == (42 ^ matrix1[2] ^ 1 ^ matrix2[2]));
		static_assert(matrix32[3] == (42 ^ matrix1[3] ^ 1 ^ matrix2[3]));
		static_assert(matrix32[4] == (42 ^ matrix1[4] ^ 1 ^ matrix2[4]));
		static_assert(matrix32[5] == (42 ^ matrix1[5] ^ 1 ^ matrix2[5]));

		// vector ^ vector ^ value ^ vector_view ^ vector
		constexpr auto vector3 = vector1 ^ vector2 ^ 42 ^ vector1.to_view() ^ vector2;

		static_assert(vector3[0] == (vector1[0] ^ vector2[0] ^ 42 ^ vector1[0] ^ vector2[0]));
		static_assert(vector3[1] == (vector1[1] ^ vector2[1] ^ 42 ^ vector1[1] ^ vector2[1]));
		static_assert(vector3[2] == (vector1[2] ^ vector2[2] ^ 42 ^ vector1[2] ^ vector2[2]));

		// value ^ vector ^ vector_view ^ vector
		constexpr auto vector32 = 42 ^ vector1 ^ vector2.to_view() ^ vector2;

		static_assert(vector32[0] == (42 ^ vector1[0] ^ vector2[0] ^ vector2[0]));
		static_assert(vector32[1] == (42 ^ vector1[1] ^ vector2[1] ^ vector2[1]));
		static_assert(vector32[2] == (42 ^ vector1[2] ^ vector2[2] ^ vector2[2]));
	}
}

TEST(TestMathOperations, TestOperatorLeftShift)
{
	constexpr vector3<unsigned long long> vector1{7, 8, 9};
	constexpr vector3<unsigned long long> vector2{9, 8, 7};

	constexpr matrix2x3<unsigned long long> matrix1{vector1, vector2};
	constexpr matrix2x3<unsigned long long> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix << matrix << value << matrix
		((matrix3 <<= matrix2) <<= 5) <<= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] << matrix2[0] << 5 << matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] << matrix2[1] << 5 << matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] << matrix2[2] << 5 << matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] << matrix2[3] << 5 << matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] << matrix2[4] << 5 << matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] << matrix2[5] << 5 << matrix1[5]);

		auto vector3 = vector1;

		// vector << vector << vector_view << vector << value << vector
		(((vector3 <<= vector2).to_view() <<= vector2) <<= 5) <<= vector1;

		ASSERT_EQ(vector3[0], vector1[0] << vector2[0] << vector2[0] << 5 << vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] << vector2[1] << vector2[1] << 5 << vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] << vector2[2] << vector2[2] << 5 << vector1[2]);
	}
	{
		// matrix << matrix << value << matrix
		constexpr auto matrix31 = matrix1 << matrix2 << 1 << matrix1;

		static_assert(matrix31[0] == matrix1[0] << matrix2[0] << 1 << matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] << matrix2[1] << 1 << matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] << matrix2[2] << 1 << matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] << matrix2[3] << 1 << matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] << matrix2[4] << 1 << matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] << matrix2[5] << 1 << matrix1[5]);

		// value << matrix << value << matrix
		constexpr auto matrix32 = 5ull << matrix1 << 1 << matrix2;

		static_assert(matrix32[0] == 5ull << matrix1[0] << 1 << matrix2[0]);
		static_assert(matrix32[1] == 5ull << matrix1[1] << 1 << matrix2[1]);
		static_assert(matrix32[2] == 5ull << matrix1[2] << 1 << matrix2[2]);
		static_assert(matrix32[3] == 5ull << matrix1[3] << 1 << matrix2[3]);
		static_assert(matrix32[4] == 5ull << matrix1[4] << 1 << matrix2[4]);
		static_assert(matrix32[5] == 5ull << matrix1[5] << 1 << matrix2[5]);

		// vector << vector << value << vector_view << vector
		constexpr auto vector3 = vector1 << vector2 << 5 << vector1.to_view() << vector2;

		static_assert(vector3[0] == vector1[0] << vector2[0] << 5 << vector1[0] << vector2[0]);
		static_assert(vector3[1] == vector1[1] << vector2[1] << 5 << vector1[1] << vector2[1]);
		static_assert(vector3[2] == vector1[2] << vector2[2] << 5 << vector1[2] << vector2[2]);

		// value << vector << vector_view << vector
		constexpr auto vector32 = 5ull << vector1 << vector2.to_view() << vector2;

		static_assert(vector32[0] == 5ull << vector1[0] << vector2[0] << vector2[0]);
		static_assert(vector32[1] == 5ull << vector1[1] << vector2[1] << vector2[1]);
		static_assert(vector32[2] == 5ull << vector1[2] << vector2[2] << vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorRightShift)
{
	constexpr vector3<unsigned long long> vector1{7, 8, 9};
	constexpr vector3<unsigned long long> vector2{9, 8, 7};

	constexpr matrix2x3<unsigned long long> matrix1{vector1, vector2};
	constexpr matrix2x3<unsigned long long> matrix2{vector2, vector1};

	{
		auto matrix3 = matrix1;

		// matrix >> matrix >> value >> matrix
		((matrix3 >>= matrix2) >>= 5) >>= matrix1;

		ASSERT_EQ(matrix3[0], matrix1[0] >> matrix2[0] >> 5 >> matrix1[0]);
		ASSERT_EQ(matrix3[1], matrix1[1] >> matrix2[1] >> 5 >> matrix1[1]);
		ASSERT_EQ(matrix3[2], matrix1[2] >> matrix2[2] >> 5 >> matrix1[2]);
		ASSERT_EQ(matrix3[3], matrix1[3] >> matrix2[3] >> 5 >> matrix1[3]);
		ASSERT_EQ(matrix3[4], matrix1[4] >> matrix2[4] >> 5 >> matrix1[4]);
		ASSERT_EQ(matrix3[5], matrix1[5] >> matrix2[5] >> 5 >> matrix1[5]);

		auto vector3 = vector1;

		// vector >> vector >> vector_view >> vector >> value >> vector
		(((vector3 >>= vector2).to_view() >>= vector2) >>= 5) >>= vector1;

		ASSERT_EQ(vector3[0], vector1[0] >> vector2[0] >> vector2[0] >> 5 >> vector1[0]);
		ASSERT_EQ(vector3[1], vector1[1] >> vector2[1] >> vector2[1] >> 5 >> vector1[1]);
		ASSERT_EQ(vector3[2], vector1[2] >> vector2[2] >> vector2[2] >> 5 >> vector1[2]);
	}
	{
		// matrix >> matrix >> value >> matrix
		constexpr auto matrix31 = matrix1 >> matrix2 >> 1 >> matrix1;

		static_assert(matrix31[0] == matrix1[0] >> matrix2[0] >> 1 >> matrix1[0]);
		static_assert(matrix31[1] == matrix1[1] >> matrix2[1] >> 1 >> matrix1[1]);
		static_assert(matrix31[2] == matrix1[2] >> matrix2[2] >> 1 >> matrix1[2]);
		static_assert(matrix31[3] == matrix1[3] >> matrix2[3] >> 1 >> matrix1[3]);
		static_assert(matrix31[4] == matrix1[4] >> matrix2[4] >> 1 >> matrix1[4]);
		static_assert(matrix31[5] == matrix1[5] >> matrix2[5] >> 1 >> matrix1[5]);

		// value >> matrix >> value >> matrix
		constexpr auto matrix32 = 999999999ull >> matrix1 >> 1 >> matrix2;

		static_assert(matrix32[0] == 999999999ull >> matrix1[0] >> 1 >> matrix2[0]);
		static_assert(matrix32[1] == 999999999ull >> matrix1[1] >> 1 >> matrix2[1]);
		static_assert(matrix32[2] == 999999999ull >> matrix1[2] >> 1 >> matrix2[2]);
		static_assert(matrix32[3] == 999999999ull >> matrix1[3] >> 1 >> matrix2[3]);
		static_assert(matrix32[4] == 999999999ull >> matrix1[4] >> 1 >> matrix2[4]);
		static_assert(matrix32[5] == 999999999ull >> matrix1[5] >> 1 >> matrix2[5]);

		// vector >> vector >> value >> vector_view >> vector
		constexpr auto vector3 = vector1 >> vector2 >> 5 >> vector1.to_view() >> vector2;

		static_assert(vector3[0] == vector1[0] >> vector2[0] >> 5 >> vector1[0] >> vector2[0]);
		static_assert(vector3[1] == vector1[1] >> vector2[1] >> 5 >> vector1[1] >> vector2[1]);
		static_assert(vector3[2] == vector1[2] >> vector2[2] >> 5 >> vector1[2] >> vector2[2]);

		// value >> vector >> vector_view >> vector
		constexpr auto vector32 = 999999999ull >> vector1 >> vector2.to_view() >> vector2;

		static_assert(vector32[0] == 999999999ull >> vector1[0] >> vector2[0] >> vector2[0]);
		static_assert(vector32[1] == 999999999ull >> vector1[1] >> vector2[1] >> vector2[1]);
		static_assert(vector32[2] == 999999999ull >> vector1[2] >> vector2[2] >> vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorInvert)
{
	constexpr vector3<int> vector1{123, 456, 789};
	constexpr auto         vector2 = -vector1;

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr auto           matrix2 = -matrix1;

	static_assert(vector2[0] == -vector1[0]);
	static_assert(vector2[1] == -vector1[1]);
	static_assert(vector2[2] == -vector1[2]);

	static_assert(matrix2[0] == -matrix1[0]);
	static_assert(matrix2[1] == -matrix1[1]);
	static_assert(matrix2[2] == -matrix1[2]);
	static_assert(matrix2[3] == -matrix1[3]);
	static_assert(matrix2[4] == -matrix1[4]);
	static_assert(matrix2[5] == -matrix1[5]);

	auto vector3 = vector1;
	// the same reference
	auto& vector4 = make_inverse(vector3);

	ASSERT_EQ(&vector3, &vector4);

	ASSERT_EQ(vector3[0], vector2[0]);
	ASSERT_EQ(vector3[1], vector2[1]);
	ASSERT_EQ(vector3[2], vector2[2]);

	// not the same view
	auto vector5 = make_inverse(vector3.to_view());

	ASSERT_EQ(vector5[0], vector1[0]);
	ASSERT_EQ(vector5[1], vector1[1]);
	ASSERT_EQ(vector5[2], vector1[2]);

	auto matrix3 = matrix1;
	// the same reference
	auto& matrix4 = make_inverse(matrix3);

	ASSERT_EQ(&matrix3, &matrix4);

	ASSERT_EQ(matrix3[0], matrix2[0]);
	ASSERT_EQ(matrix3[1], matrix2[1]);
	ASSERT_EQ(matrix3[2], matrix2[2]);
	ASSERT_EQ(matrix3[3], matrix2[3]);
	ASSERT_EQ(matrix3[4], matrix2[4]);
	ASSERT_EQ(matrix3[5], matrix2[5]);
}

TEST(TestMathOperations, TestOperatorBitwiseNot)
{
	constexpr vector3<unsigned> vector1{123, 456, 789};
	constexpr auto              vector2 = ~vector1;

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr auto           matrix2 = ~matrix1;

	static_assert(vector2[0] == ~vector1[0]);
	static_assert(vector2[1] == ~vector1[1]);
	static_assert(vector2[2] == ~vector1[2]);

	static_assert(matrix2[0] == ~matrix1[0]);
	static_assert(matrix2[1] == ~matrix1[1]);
	static_assert(matrix2[2] == ~matrix1[2]);
	static_assert(matrix2[3] == ~matrix1[3]);
	static_assert(matrix2[4] == ~matrix1[4]);
	static_assert(matrix2[5] == ~matrix1[5]);

	auto vector3 = vector1;
	// the same reference
	auto& vector4 = make_bitwise_not(vector3);

	ASSERT_EQ(&vector3, &vector4);

	ASSERT_EQ(vector3[0], vector2[0]);
	ASSERT_EQ(vector3[1], vector2[1]);
	ASSERT_EQ(vector3[2], vector2[2]);

	// not the same view
	auto vector5 = make_bitwise_not(vector3.to_view());

	ASSERT_EQ(vector5[0], vector1[0]);
	ASSERT_EQ(vector5[1], vector1[1]);
	ASSERT_EQ(vector5[2], vector1[2]);

	auto matrix3 = matrix1;
	// the same reference
	auto& matrix4 = make_bitwise_not(matrix3);

	ASSERT_EQ(&matrix3, &matrix4);

	ASSERT_EQ(matrix3[0], matrix2[0]);
	ASSERT_EQ(matrix3[1], matrix2[1]);
	ASSERT_EQ(matrix3[2], matrix2[2]);
	ASSERT_EQ(matrix3[3], matrix2[3]);
	ASSERT_EQ(matrix3[4], matrix2[4]);
	ASSERT_EQ(matrix3[5], matrix2[5]);
}

TEST(TestMathOperations, TestOperatorEqualAndNotEqual)
{
	constexpr vector3<unsigned> vector1{123, 456, 789};
	constexpr vector3<unsigned> vector2{123, 123, 123};

	constexpr auto vector31 = (vector1 == vector2);
	constexpr auto vector32 = (vector1 == 456);
	constexpr auto vector33 = (456 == vector1);

	static_assert(vector31[0]);
	static_assert(!vector31[1]);
	static_assert(!vector31[2]);

	static_assert(!vector32[0]);
	static_assert(vector32[1]);
	static_assert(!vector32[2]);

	static_assert(!vector33[0]);
	static_assert(vector33[1]);
	static_assert(!vector33[2]);

	constexpr auto vector41 = (vector1 != vector2);
	constexpr auto vector42 = (vector1 != 456);
	constexpr auto vector43 = (456 != vector1);

	static_assert(!vector41[0]);
	static_assert(vector41[1]);
	static_assert(vector41[2]);

	static_assert(vector42[0]);
	static_assert(!vector42[1]);
	static_assert(vector42[2]);

	static_assert(vector43[0]);
	static_assert(!vector43[1]);
	static_assert(vector43[2]);

	constexpr matrix2x3<int> matrix1{vector1, vector2};
	constexpr matrix2x3<int> matrix2{vector2, vector1};

	constexpr auto matrix31 = (matrix1 == matrix2);
	constexpr auto matrix32 = (matrix1 == 123);
	constexpr auto matrix33 = (123 == matrix1);

	static_assert(matrix31[0]);
	static_assert(!matrix31[1]);
	static_assert(!matrix31[2]);
	static_assert(matrix31[3]);
	static_assert(!matrix31[4]);
	static_assert(!matrix31[5]);

	static_assert(matrix32[0]);
	static_assert(!matrix32[1]);
	static_assert(!matrix32[2]);
	static_assert(matrix32[3]);
	static_assert(matrix32[4]);
	static_assert(matrix32[5]);

	static_assert(matrix33[0]);
	static_assert(!matrix33[1]);
	static_assert(!matrix33[2]);
	static_assert(matrix33[3]);
	static_assert(matrix33[4]);
	static_assert(matrix33[5]);

	constexpr auto matrix41 = (matrix1 != matrix2);
	constexpr auto matrix42 = (matrix1 != 123);
	constexpr auto matrix43 = (123 != matrix1);

	static_assert(!matrix41[0]);
	static_assert(matrix41[1]);
	static_assert(matrix41[2]);
	static_assert(!matrix41[3]);
	static_assert(matrix41[4]);
	static_assert(matrix41[5]);

	static_assert(!matrix42[0]);
	static_assert(matrix42[1]);
	static_assert(matrix42[2]);
	static_assert(!matrix42[3]);
	static_assert(!matrix42[4]);
	static_assert(!matrix42[5]);

	static_assert(!matrix43[0]);
	static_assert(matrix43[1]);
	static_assert(matrix43[2]);
	static_assert(!matrix43[3]);
	static_assert(!matrix43[4]);
	static_assert(!matrix43[5]);
}
