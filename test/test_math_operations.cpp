#include <gtest/gtest.h>

#include "../src/math/math_operations.hpp"

using namespace gal::test::new_math;

TEST(TestMathOperations, TestOperatorAdd)
{
	constexpr vector<int, 3> vector1{1, 2, 3};
	constexpr vector<int, 3> vector2{3, 2, 1};

	constexpr matrix<int, 2, 3> matrix1{vector1, vector2};
	constexpr matrix<int, 2, 3> matrix2{vector2, vector1};

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

		// vector + vector + value + vector_view + vector
		constexpr auto vector3 = vector1 + vector2 + 42 + vector1.to_view() + vector2;

		static_assert(vector3[0] == vector1[0] + vector2[0] + 42 + vector1[0] + vector2[0]);
		static_assert(vector3[1] == vector1[1] + vector2[1] + 42 + vector1[1] + vector2[1]);
		static_assert(vector3[2] == vector1[2] + vector2[2] + 42 + vector1[2] + vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorSubtract)
{
	constexpr vector<int, 3> vector1{ 1, 2, 3 };
	constexpr vector<int, 3> vector2{ 3, 2, 1 };

	constexpr matrix<int, 2, 3> matrix1{ vector1, vector2 };
	constexpr matrix<int, 2, 3> matrix2{ vector2, vector1 };

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

		// vector - vector - value - vector_view - vector
		constexpr auto vector3 = vector1 - vector2 - 42 - vector1.to_view() - vector2;

		static_assert(vector3[0] == vector1[0] - vector2[0] - 42 - vector1[0] - vector2[0]);
		static_assert(vector3[1] == vector1[1] - vector2[1] - 42 - vector1[1] - vector2[1]);
		static_assert(vector3[2] == vector1[2] - vector2[2] - 42 - vector1[2] - vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorMultiply)
{
	constexpr vector<int, 3> vector1{ 1, 2, 3 };
	constexpr vector<int, 3> vector2{ 3, 2, 1 };

	constexpr matrix<int, 2, 3> matrix1{ vector1, vector2 };
	constexpr matrix<int, 2, 3> matrix2{ vector2, vector1 };

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

		// vector * vector * value * vector_view * vector
		constexpr auto vector3 = vector1 * vector2 * 42 * vector1.to_view() * vector2;

		static_assert(vector3[0] == vector1[0] * vector2[0] * 42 * vector1[0] * vector2[0]);
		static_assert(vector3[1] == vector1[1] * vector2[1] * 42 * vector1[1] * vector2[1]);
		static_assert(vector3[2] == vector1[2] * vector2[2] * 42 * vector1[2] * vector2[2]);
	}
}

TEST(TestMathOperations, TestOperatorDivide)
{
	constexpr vector<double, 3> vector1{ 1, 2, 3 };
	constexpr vector<double, 3> vector2{ 3, 2, 1 };

	constexpr matrix<double, 2, 3> matrix1{ vector1, vector2 };
	constexpr matrix<double, 2, 3> matrix2{ vector2, vector1 };

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

		// vector / vector / value / vector_view / vector
		constexpr auto vector3 = vector1 / vector2 / 42 / vector1.to_view() / vector2;

		static_assert(double_eq(vector3[0], vector1[0] / vector2[0] / 42 / vector1[0] / vector2[0]));
		static_assert(double_eq(vector3[1], vector1[1] / vector2[1] / 42 / vector1[1] / vector2[1]));
		static_assert(double_eq(vector3[2], vector1[2] / vector2[2] / 42 / vector1[2] / vector2[2]));
	}
}

TEST(TestMathOperations, TestOperatorModel)
{
	constexpr vector<int, 3> vector1{ 123, 456, 789 };
	constexpr vector<int, 3> vector2{ 987, 654, 321 };

	constexpr matrix<int, 2, 3> matrix1{ vector1, vector2 };
	constexpr matrix<int, 2, 3> matrix2{ vector2, vector1 };

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

		// vector % vector % value % vector_view % vector
		constexpr auto vector3 = vector1 % vector2 % 42 % vector1.to_view() % vector2;

		static_assert(vector3[0] == vector1[0] % vector2[0] % 42 % vector1[0] % vector2[0]);
		static_assert(vector3[1] == vector1[1] % vector2[1] % 42 % vector1[1] % vector2[1]);
		static_assert(vector3[2] == vector1[2] % vector2[2] % 42 % vector1[2] % vector2[2]);
	}
}
