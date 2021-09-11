#include <gtest/gtest.h>

#include <ranges>

#include "../src/math/basic_operations.hpp"
// using namespace gal::test::math;
namespace math = gal::test::math;

// import gal.toolbox.math.basic_operations;
// using namespace gal::toolbox::math;
// namespace math = gal::toolbox::math;

TEST(TestMathOperations, TestOperatorAdd)
{
	constexpr math::vector3<int> vec1{1, 2, 3};
	constexpr math::vector3<int> vec2{3, 2, 1};

	// constexpr math::matrix2x3<int> mat1{vec1, vec2};
	// constexpr math::matrix2x3<int> mat2{vec2, vec1};

	{
		// auto mat3 = mat1;
		//
		// // matrix + matrix + value + matrix
		// ((mat3 += mat2) += 42) += mat1;
		//
		// ASSERT_EQ(mat3[0], mat1[0] + mat2[0] + 42 + mat1[0]);
		// ASSERT_EQ(mat3[1], mat1[1] + mat2[1] + 42 + mat1[1]);
		// ASSERT_EQ(mat3[2], mat1[2] + mat2[2] + 42 + mat1[2]);
		// ASSERT_EQ(mat3[3], mat1[3] + mat2[3] + 42 + mat1[3]);
		// ASSERT_EQ(mat3[4], mat1[4] + mat2[4] + 42 + mat1[4]);
		// ASSERT_EQ(mat3[5], mat1[5] + mat2[5] + 42 + mat1[5]);

		auto vec3 = vec1;

		// vector + vector + vector_view + vector + value + vector
		(((vec3 += vec2).to_view() += vec2) += 42) += vec1;

		ASSERT_EQ(vec3[0], vec1[0] + vec2[0] + vec2[0] + 42 + vec1[0]);
		ASSERT_EQ(vec3[1], vec1[1] + vec2[1] + vec2[1] + 42 + vec1[1]);
		ASSERT_EQ(vec3[2], vec1[2] + vec2[2] + vec2[2] + 42 + vec1[2]);
	}
	{
		// // matrix + matrix + value + matrix
		// constexpr auto mat31 = mat1 + mat2 + 1 + mat1;
		//
		// static_assert(mat31[0] == mat1[0] + mat2[0] + 1 + mat1[0]);
		// static_assert(mat31[1] == mat1[1] + mat2[1] + 1 + mat1[1]);
		// static_assert(mat31[2] == mat1[2] + mat2[2] + 1 + mat1[2]);
		// static_assert(mat31[3] == mat1[3] + mat2[3] + 1 + mat1[3]);
		// static_assert(mat31[4] == mat1[4] + mat2[4] + 1 + mat1[4]);
		// static_assert(mat31[5] == mat1[5] + mat2[5] + 1 + mat1[5]);
		//
		// // value + matrix + value + matrix
		// constexpr auto mat32 = 42 + mat1 + 1 + mat2;
		//
		// static_assert(mat32[0] == 42 + mat1[0] + 1 + mat2[0]);
		// static_assert(mat32[1] == 42 + mat1[1] + 1 + mat2[1]);
		// static_assert(mat32[2] == 42 + mat1[2] + 1 + mat2[2]);
		// static_assert(mat32[3] == 42 + mat1[3] + 1 + mat2[3]);
		// static_assert(mat32[4] == 42 + mat1[4] + 1 + mat2[4]);
		// static_assert(mat32[5] == 42 + mat1[5] + 1 + mat2[5]);

		// vector + vector + value + vector_view + vector
		constexpr auto vec31 = vec1 + vec2 + 42 + vec1.to_view() + vec2;

		static_assert(vec31[0] == vec1[0] + vec2[0] + 42 + vec1[0] + vec2[0]);
		static_assert(vec31[1] == vec1[1] + vec2[1] + 42 + vec1[1] + vec2[1]);
		static_assert(vec31[2] == vec1[2] + vec2[2] + 42 + vec1[2] + vec2[2]);

		// value + vector + vector_view + vector
		constexpr auto vec32 = 42 + vec1 + vec2.to_view() + vec2;

		static_assert(vec32[0] == 42 + vec1[0] + vec2[0] + vec2[0]);
		static_assert(vec32[1] == 42 + vec1[1] + vec2[1] + vec2[1]);
		static_assert(vec32[2] == 42 + vec1[2] + vec2[2] + vec2[2]);
	}
}
//
// TEST(TestMathOperations, TestOperatorSubtract)
// {
// 	constexpr math::vector3<int> vec1{1, 2, 3};
// 	constexpr math::vector3<int> vec2{3, 2, 1};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix - matrix - value - matrix
// 		((mat3 -= mat2) -= 42) -= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] - mat2[0] - 42 - mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] - mat2[1] - 42 - mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] - mat2[2] - 42 - mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] - mat2[3] - 42 - mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] - mat2[4] - 42 - mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] - mat2[5] - 42 - mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector - vector - vector_view - vector - value - vector
// 		(((vec3 -= vec2).to_view() -= vec2) -= 42) -= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] - vec2[0] - vec2[0] - 42 - vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] - vec2[1] - vec2[1] - 42 - vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] - vec2[2] - vec2[2] - 42 - vec1[2]);
// 	}
// 	{
// 		// matrix - matrix - value - matrix
// 		constexpr auto mat31 = mat1 - mat2 - 1 - mat1;
//
// 		static_assert(mat31[0] == mat1[0] - mat2[0] - 1 - mat1[0]);
// 		static_assert(mat31[1] == mat1[1] - mat2[1] - 1 - mat1[1]);
// 		static_assert(mat31[2] == mat1[2] - mat2[2] - 1 - mat1[2]);
// 		static_assert(mat31[3] == mat1[3] - mat2[3] - 1 - mat1[3]);
// 		static_assert(mat31[4] == mat1[4] - mat2[4] - 1 - mat1[4]);
// 		static_assert(mat31[5] == mat1[5] - mat2[5] - 1 - mat1[5]);
//
// 		// value - matrix - value - matrix
// 		constexpr auto mat32 = 42 - mat1 - 1 - mat2;
//
// 		static_assert(mat32[0] == 42 - mat1[0] - 1 - mat2[0]);
// 		static_assert(mat32[1] == 42 - mat1[1] - 1 - mat2[1]);
// 		static_assert(mat32[2] == 42 - mat1[2] - 1 - mat2[2]);
// 		static_assert(mat32[3] == 42 - mat1[3] - 1 - mat2[3]);
// 		static_assert(mat32[4] == 42 - mat1[4] - 1 - mat2[4]);
// 		static_assert(mat32[5] == 42 - mat1[5] - 1 - mat2[5]);
//
// 		// vector - vector - value - vector_view - vector
// 		constexpr auto vec3 = vec1 - vec2 - 42 - vec1.to_view() - vec2;
//
// 		static_assert(vec3[0] == vec1[0] - vec2[0] - 42 - vec1[0] - vec2[0]);
// 		static_assert(vec3[1] == vec1[1] - vec2[1] - 42 - vec1[1] - vec2[1]);
// 		static_assert(vec3[2] == vec1[2] - vec2[2] - 42 - vec1[2] - vec2[2]);
//
// 		// value - vector - vector_view - vector
// 		constexpr auto vec32 = 42 - vec1 - vec2.to_view() - vec2;
//
// 		static_assert(vec32[0] == 42 - vec1[0] - vec2[0] - vec2[0]);
// 		static_assert(vec32[1] == 42 - vec1[1] - vec2[1] - vec2[1]);
// 		static_assert(vec32[2] == 42 - vec1[2] - vec2[2] - vec2[2]);
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorMultiply)
// {
// 	constexpr math::vector3<int> vec1{1, 2, 3};
// 	constexpr math::vector3<int> vec2{3, 2, 1};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix * matrix * value * matrix
// 		((mat3 *= mat2) *= 42) *= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] * mat2[0] * 42 * mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] * mat2[1] * 42 * mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] * mat2[2] * 42 * mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] * mat2[3] * 42 * mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] * mat2[4] * 42 * mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] * mat2[5] * 42 * mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector * vector * vector_view * vector * value * vector
// 		(((vec3 *= vec2).to_view() *= vec2) *= 42) *= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] * vec2[0] * vec2[0] * 42 * vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] * vec2[1] * vec2[1] * 42 * vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] * vec2[2] * vec2[2] * 42 * vec1[2]);
// 	}
// 	{
// 		// matrix * matrix * value * matrix
// 		constexpr auto mat31 = mat1 * mat2 * 1 * mat1;
//
// 		static_assert(mat31[0] == mat1[0] * mat2[0] * 1 * mat1[0]);
// 		static_assert(mat31[1] == mat1[1] * mat2[1] * 1 * mat1[1]);
// 		static_assert(mat31[2] == mat1[2] * mat2[2] * 1 * mat1[2]);
// 		static_assert(mat31[3] == mat1[3] * mat2[3] * 1 * mat1[3]);
// 		static_assert(mat31[4] == mat1[4] * mat2[4] * 1 * mat1[4]);
// 		static_assert(mat31[5] == mat1[5] * mat2[5] * 1 * mat1[5]);
//
// 		// value * matrix * value * matrix
// 		constexpr auto mat32 = 42 * mat1 * 1 * mat2;
//
// 		static_assert(mat32[0] == 42 * mat1[0] * 1 * mat2[0]);
// 		static_assert(mat32[1] == 42 * mat1[1] * 1 * mat2[1]);
// 		static_assert(mat32[2] == 42 * mat1[2] * 1 * mat2[2]);
// 		static_assert(mat32[3] == 42 * mat1[3] * 1 * mat2[3]);
// 		static_assert(mat32[4] == 42 * mat1[4] * 1 * mat2[4]);
// 		static_assert(mat32[5] == 42 * mat1[5] * 1 * mat2[5]);
//
// 		// vector * vector * value * vector_view * vector
// 		constexpr auto vec3 = vec1 * vec2 * 42 * vec1.to_view() * vec2;
//
// 		static_assert(vec3[0] == vec1[0] * vec2[0] * 42 * vec1[0] * vec2[0]);
// 		static_assert(vec3[1] == vec1[1] * vec2[1] * 42 * vec1[1] * vec2[1]);
// 		static_assert(vec3[2] == vec1[2] * vec2[2] * 42 * vec1[2] * vec2[2]);
//
// 		// value * vector * vector_view * vector
// 		constexpr auto vec32 = 42 * vec1 * vec2.to_view() * vec2;
//
// 		static_assert(vec32[0] == 42 * vec1[0] * vec2[0] * vec2[0]);
// 		static_assert(vec32[1] == 42 * vec1[1] * vec2[1] * vec2[1]);
// 		static_assert(vec32[2] == 42 * vec1[2] * vec2[2] * vec2[2]);
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorDivide)
// {
// 	constexpr math::vector3<double> vec1{1, 2, 3};
// 	constexpr math::vector3<double> vec2{3, 2, 1};
//
// 	constexpr math::matrix2x3<double> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<double> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix / matrix / value / matrix
// 		((mat3 /= mat2) /= 42) /= mat1;
//
// 		ASSERT_DOUBLE_EQ(mat3[0], mat1[0] / mat2[0] / 42 / mat1[0]);
// 		ASSERT_DOUBLE_EQ(mat3[1], mat1[1] / mat2[1] / 42 / mat1[1]);
// 		ASSERT_DOUBLE_EQ(mat3[2], mat1[2] / mat2[2] / 42 / mat1[2]);
// 		ASSERT_DOUBLE_EQ(mat3[3], mat1[3] / mat2[3] / 42 / mat1[3]);
// 		ASSERT_DOUBLE_EQ(mat3[4], mat1[4] / mat2[4] / 42 / mat1[4]);
// 		ASSERT_DOUBLE_EQ(mat3[5], mat1[5] / mat2[5] / 42 / mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector / vector / vector_view / vector / value / vector
// 		(((vec3 /= vec2).to_view() /= vec2) /= 42) /= vec1;
//
// 		ASSERT_DOUBLE_EQ(vec3[0], vec1[0] / vec2[0] / vec2[0] / 42 / vec1[0]);
// 		ASSERT_DOUBLE_EQ(vec3[1], vec1[1] / vec2[1] / vec2[1] / 42 / vec1[1]);
// 		ASSERT_DOUBLE_EQ(vec3[2], vec1[2] / vec2[2] / vec2[2] / 42 / vec1[2]);
// 	}
// 	{
// 		constexpr auto double_eq = [](const double a, const double b) constexpr
// 		{
// 			return
// 				(a - b) < 0.0000001 &&
// 				(a - b) > -0.0000001;
// 		};
//
// 		// matrix / matrix / value / matrix
// 		constexpr auto mat31 = mat1 / mat2 / 1 / mat1;
//
// 		static_assert(double_eq(mat31[0], mat1[0] / mat2[0] / 1 / mat1[0]));
// 		static_assert(double_eq(mat31[1], mat1[1] / mat2[1] / 1 / mat1[1]));
// 		static_assert(double_eq(mat31[2], mat1[2] / mat2[2] / 1 / mat1[2]));
// 		static_assert(double_eq(mat31[3], mat1[3] / mat2[3] / 1 / mat1[3]));
// 		static_assert(double_eq(mat31[4], mat1[4] / mat2[4] / 1 / mat1[4]));
// 		static_assert(double_eq(mat31[5], mat1[5] / mat2[5] / 1 / mat1[5]));
//
// 		// value / matrix / value / matrix
// 		constexpr auto mat32 = 42.0 / mat1 / 1 / mat2;
//
// 		static_assert(double_eq(mat32[0], 42 / mat1[0] / 1 / mat2[0]));
// 		static_assert(double_eq(mat32[1], 42 / mat1[1] / 1 / mat2[1]));
// 		static_assert(double_eq(mat32[2], 42 / mat1[2] / 1 / mat2[2]));
// 		static_assert(double_eq(mat32[3], 42 / mat1[3] / 1 / mat2[3]));
// 		static_assert(double_eq(mat32[4], 42 / mat1[4] / 1 / mat2[4]));
// 		static_assert(double_eq(mat32[5], 42 / mat1[5] / 1 / mat2[5]));
//
// 		// vector / vector / value / vector_view / vector
// 		constexpr auto vec3 = vec1 / vec2 / 42 / vec1.to_view() / vec2;
//
// 		static_assert(double_eq(vec3[0], vec1[0] / vec2[0] / 42 / vec1[0] / vec2[0]));
// 		static_assert(double_eq(vec3[1], vec1[1] / vec2[1] / 42 / vec1[1] / vec2[1]));
// 		static_assert(double_eq(vec3[2], vec1[2] / vec2[2] / 42 / vec1[2] / vec2[2]));
//
// 		// value / vector / vector_view / vector
// 		constexpr auto vec32 = 42.0 / vec1 / vec2.to_view() / vec2;
//
// 		static_assert(double_eq(vec32[0], 42 / vec1[0] / vec2[0] / vec2[0]));
// 		static_assert(double_eq(vec32[1], 42 / vec1[1] / vec2[1] / vec2[1]));
// 		static_assert(double_eq(vec32[2], 42 / vec1[2] / vec2[2] / vec2[2]));
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorModel)
// {
// 	constexpr math::vector3<int> vec1{123, 456, 789};
// 	constexpr math::vector3<int> vec2{987, 654, 321};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix % matrix % value % matrix
// 		((mat3 %= mat2) %= 42) %= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] % mat2[0] % 42 % mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] % mat2[1] % 42 % mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] % mat2[2] % 42 % mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] % mat2[3] % 42 % mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] % mat2[4] % 42 % mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] % mat2[5] % 42 % mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector % vector % vector_view % vector % value % vector
// 		(((vec3 %= vec2).to_view() %= vec2) %= 42) %= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] % vec2[0] % vec2[0] % 42 % vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] % vec2[1] % vec2[1] % 42 % vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] % vec2[2] % vec2[2] % 42 % vec1[2]);
// 	}
// 	{
// 		// matrix % matrix % value % matrix
// 		constexpr auto mat31 = mat1 % mat2 % 1 % mat1;
//
// 		static_assert(mat31[0] == mat1[0] % mat2[0] % 1 % mat1[0]);
// 		static_assert(mat31[1] == mat1[1] % mat2[1] % 1 % mat1[1]);
// 		static_assert(mat31[2] == mat1[2] % mat2[2] % 1 % mat1[2]);
// 		static_assert(mat31[3] == mat1[3] % mat2[3] % 1 % mat1[3]);
// 		static_assert(mat31[4] == mat1[4] % mat2[4] % 1 % mat1[4]);
// 		static_assert(mat31[5] == mat1[5] % mat2[5] % 1 % mat1[5]);
//
// 		// value % matrix % value % matrix
// 		constexpr auto mat32 = 42 % mat1 % 1 % mat2;
//
// 		static_assert(mat32[0] == 42 % mat1[0] % 1 % mat2[0]);
// 		static_assert(mat32[1] == 42 % mat1[1] % 1 % mat2[1]);
// 		static_assert(mat32[2] == 42 % mat1[2] % 1 % mat2[2]);
// 		static_assert(mat32[3] == 42 % mat1[3] % 1 % mat2[3]);
// 		static_assert(mat32[4] == 42 % mat1[4] % 1 % mat2[4]);
// 		static_assert(mat32[5] == 42 % mat1[5] % 1 % mat2[5]);
//
// 		// vector % vector % value % vector_view % vector
// 		constexpr auto vec3 = vec1 % vec2 % 42 % vec1.to_view() % vec2;
//
// 		static_assert(vec3[0] == vec1[0] % vec2[0] % 42 % vec1[0] % vec2[0]);
// 		static_assert(vec3[1] == vec1[1] % vec2[1] % 42 % vec1[1] % vec2[1]);
// 		static_assert(vec3[2] == vec1[2] % vec2[2] % 42 % vec1[2] % vec2[2]);
//
// 		// value % vector % vector_view % vector
// 		constexpr auto vec32 = 42 % vec1 % vec2.to_view() % vec2;
//
// 		static_assert(vec32[0] == 42 % vec1[0] % vec2[0] % vec2[0]);
// 		static_assert(vec32[1] == 42 % vec1[1] % vec2[1] % vec2[1]);
// 		static_assert(vec32[2] == 42 % vec1[2] % vec2[2] % vec2[2]);
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorBitAnd)
// {
// 	constexpr math::vector3<int> vec1{123, 456, 789};
// 	constexpr math::vector3<int> vec2{987, 654, 321};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix & matrix & value & matrix
// 		((mat3 &= mat2) &= 42) &= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] & mat2[0] & 42 & mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] & mat2[1] & 42 & mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] & mat2[2] & 42 & mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] & mat2[3] & 42 & mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] & mat2[4] & 42 & mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] & mat2[5] & 42 & mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector & vector & vector_view & vector & value & vector
// 		(((vec3 &= vec2).to_view() &= vec2) &= 42) &= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] & vec2[0] & vec2[0] & 42 & vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] & vec2[1] & vec2[1] & 42 & vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] & vec2[2] & vec2[2] & 42 & vec1[2]);
// 	}
// 	{
// 		// matrix & matrix & value & matrix
// 		constexpr auto mat31 = mat1 & mat2 & 1 & mat1;
//
// 		static_assert(mat31[0] == (mat1[0] & mat2[0] & 1 & mat1[0]));
// 		static_assert(mat31[1] == (mat1[1] & mat2[1] & 1 & mat1[1]));
// 		static_assert(mat31[2] == (mat1[2] & mat2[2] & 1 & mat1[2]));
// 		static_assert(mat31[3] == (mat1[3] & mat2[3] & 1 & mat1[3]));
// 		static_assert(mat31[4] == (mat1[4] & mat2[4] & 1 & mat1[4]));
// 		static_assert(mat31[5] == (mat1[5] & mat2[5] & 1 & mat1[5]));
//
// 		// value & matrix & value & matrix
// 		constexpr auto mat32 = 42 & mat1 & 1 & mat2;
//
// 		static_assert(mat32[0] == (42 & mat1[0] & 1 & mat2[0]));
// 		static_assert(mat32[1] == (42 & mat1[1] & 1 & mat2[1]));
// 		static_assert(mat32[2] == (42 & mat1[2] & 1 & mat2[2]));
// 		static_assert(mat32[3] == (42 & mat1[3] & 1 & mat2[3]));
// 		static_assert(mat32[4] == (42 & mat1[4] & 1 & mat2[4]));
// 		static_assert(mat32[5] == (42 & mat1[5] & 1 & mat2[5]));
//
// 		// vector & vector & value & vector_view & vector
// 		constexpr auto vec3 = vec1 & vec2 & 42 & vec1.to_view() & vec2;
//
// 		static_assert(vec3[0] == (vec1[0] & vec2[0] & 42 & vec1[0] & vec2[0]));
// 		static_assert(vec3[1] == (vec1[1] & vec2[1] & 42 & vec1[1] & vec2[1]));
// 		static_assert(vec3[2] == (vec1[2] & vec2[2] & 42 & vec1[2] & vec2[2]));
//
// 		// value & vector & vector_view & vector
// 		constexpr auto vec32 = 42 & vec1 & vec2.to_view() & vec2;
//
// 		static_assert(vec32[0] == (42 & vec1[0] & vec2[0] & vec2[0]));
// 		static_assert(vec32[1] == (42 & vec1[1] & vec2[1] & vec2[1]));
// 		static_assert(vec32[2] == (42 & vec1[2] & vec2[2] & vec2[2]));
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorBitOr)
// {
// 	constexpr math::vector3<int> vec1{123, 456, 789};
// 	constexpr math::vector3<int> vec2{987, 654, 321};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix | matrix | value | matrix
// 		((mat3 |= mat2) |= 42) |= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] | mat2[0] | 42 | mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] | mat2[1] | 42 | mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] | mat2[2] | 42 | mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] | mat2[3] | 42 | mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] | mat2[4] | 42 | mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] | mat2[5] | 42 | mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector | vector | vector_view | vector | value | vector
// 		(((vec3 |= vec2).to_view() |= vec2) |= 42) |= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] | vec2[0] | vec2[0] | 42 | vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] | vec2[1] | vec2[1] | 42 | vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] | vec2[2] | vec2[2] | 42 | vec1[2]);
// 	}
// 	{
// 		// matrix | matrix | value | matrix
// 		constexpr auto mat31 = mat1 | mat2 | 1 | mat1;
//
// 		static_assert(mat31[0] == (mat1[0] | mat2[0] | 1 | mat1[0]));
// 		static_assert(mat31[1] == (mat1[1] | mat2[1] | 1 | mat1[1]));
// 		static_assert(mat31[2] == (mat1[2] | mat2[2] | 1 | mat1[2]));
// 		static_assert(mat31[3] == (mat1[3] | mat2[3] | 1 | mat1[3]));
// 		static_assert(mat31[4] == (mat1[4] | mat2[4] | 1 | mat1[4]));
// 		static_assert(mat31[5] == (mat1[5] | mat2[5] | 1 | mat1[5]));
//
// 		// value | matrix | value | matrix
// 		constexpr auto mat32 = 42 | mat1 | 1 | mat2;
//
// 		static_assert(mat32[0] == (42 | mat1[0] | 1 | mat2[0]));
// 		static_assert(mat32[1] == (42 | mat1[1] | 1 | mat2[1]));
// 		static_assert(mat32[2] == (42 | mat1[2] | 1 | mat2[2]));
// 		static_assert(mat32[3] == (42 | mat1[3] | 1 | mat2[3]));
// 		static_assert(mat32[4] == (42 | mat1[4] | 1 | mat2[4]));
// 		static_assert(mat32[5] == (42 | mat1[5] | 1 | mat2[5]));
//
// 		// vector | vector | value | vector_view | vector
// 		constexpr auto vec3 = vec1 | vec2 | 42 | vec1.to_view() | vec2;
//
// 		static_assert(vec3[0] == (vec1[0] | vec2[0] | 42 | vec1[0] | vec2[0]));
// 		static_assert(vec3[1] == (vec1[1] | vec2[1] | 42 | vec1[1] | vec2[1]));
// 		static_assert(vec3[2] == (vec1[2] | vec2[2] | 42 | vec1[2] | vec2[2]));
//
// 		// value | vector | vector_view | vector
// 		constexpr auto vec32 = 42 | vec1 | vec2.to_view() | vec2;
//
// 		static_assert(vec32[0] == (42 | vec1[0] | vec2[0] | vec2[0]));
// 		static_assert(vec32[1] == (42 | vec1[1] | vec2[1] | vec2[1]));
// 		static_assert(vec32[2] == (42 | vec1[2] | vec2[2] | vec2[2]));
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorBitXor)
// {
// 	constexpr math::vector3<int> vec1{123, 456, 789};
// 	constexpr math::vector3<int> vec2{987, 654, 321};
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix ^ matrix ^ value ^ matrix
// 		((mat3 ^= mat2) ^= 42) ^= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] ^ mat2[0] ^ 42 ^ mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] ^ mat2[1] ^ 42 ^ mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] ^ mat2[2] ^ 42 ^ mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] ^ mat2[3] ^ 42 ^ mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] ^ mat2[4] ^ 42 ^ mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] ^ mat2[5] ^ 42 ^ mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector ^ vector ^ vector_view ^ vector ^ value ^ vector
// 		(((vec3 ^= vec2).to_view() ^= vec2) ^= 42) ^= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] ^ vec2[0] ^ vec2[0] ^ 42 ^ vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] ^ vec2[1] ^ vec2[1] ^ 42 ^ vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] ^ vec2[2] ^ vec2[2] ^ 42 ^ vec1[2]);
// 	}
// 	{
// 		// matrix ^ matrix ^ value ^ matrix
// 		constexpr auto mat31 = mat1 ^ mat2 ^ 1 ^ mat1;
//
// 		static_assert(mat31[0] == (mat1[0] ^ mat2[0] ^ 1 ^ mat1[0]));
// 		static_assert(mat31[1] == (mat1[1] ^ mat2[1] ^ 1 ^ mat1[1]));
// 		static_assert(mat31[2] == (mat1[2] ^ mat2[2] ^ 1 ^ mat1[2]));
// 		static_assert(mat31[3] == (mat1[3] ^ mat2[3] ^ 1 ^ mat1[3]));
// 		static_assert(mat31[4] == (mat1[4] ^ mat2[4] ^ 1 ^ mat1[4]));
// 		static_assert(mat31[5] == (mat1[5] ^ mat2[5] ^ 1 ^ mat1[5]));
//
// 		// value ^ matrix ^ value ^ matrix
// 		constexpr auto mat32 = 42 ^ mat1 ^ 1 ^ mat2;
//
// 		static_assert(mat32[0] == (42 ^ mat1[0] ^ 1 ^ mat2[0]));
// 		static_assert(mat32[1] == (42 ^ mat1[1] ^ 1 ^ mat2[1]));
// 		static_assert(mat32[2] == (42 ^ mat1[2] ^ 1 ^ mat2[2]));
// 		static_assert(mat32[3] == (42 ^ mat1[3] ^ 1 ^ mat2[3]));
// 		static_assert(mat32[4] == (42 ^ mat1[4] ^ 1 ^ mat2[4]));
// 		static_assert(mat32[5] == (42 ^ mat1[5] ^ 1 ^ mat2[5]));
//
// 		// vector ^ vector ^ value ^ vector_view ^ vector
// 		constexpr auto vec3 = vec1 ^ vec2 ^ 42 ^ vec1.to_view() ^ vec2;
//
// 		static_assert(vec3[0] == (vec1[0] ^ vec2[0] ^ 42 ^ vec1[0] ^ vec2[0]));
// 		static_assert(vec3[1] == (vec1[1] ^ vec2[1] ^ 42 ^ vec1[1] ^ vec2[1]));
// 		static_assert(vec3[2] == (vec1[2] ^ vec2[2] ^ 42 ^ vec1[2] ^ vec2[2]));
//
// 		// value ^ vector ^ vector_view ^ vector
// 		constexpr auto vec32 = 42 ^ vec1 ^ vec2.to_view() ^ vec2;
//
// 		static_assert(vec32[0] == (42 ^ vec1[0] ^ vec2[0] ^ vec2[0]));
// 		static_assert(vec32[1] == (42 ^ vec1[1] ^ vec2[1] ^ vec2[1]));
// 		static_assert(vec32[2] == (42 ^ vec1[2] ^ vec2[2] ^ vec2[2]));
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorLeftShift)
// {
// 	constexpr math::vector3<unsigned long long> vec1{7, 8, 9};
// 	constexpr math::vector3<unsigned long long> vec2{9, 8, 7};
//
// 	constexpr math::matrix2x3<unsigned long long> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<unsigned long long> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix << matrix << value << matrix
// 		((mat3 <<= mat2) <<= 5) <<= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] << mat2[0] << 5 << mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] << mat2[1] << 5 << mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] << mat2[2] << 5 << mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] << mat2[3] << 5 << mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] << mat2[4] << 5 << mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] << mat2[5] << 5 << mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector << vector << vector_view << vector << value << vector
// 		(((vec3 <<= vec2).to_view() <<= vec2) <<= 5) <<= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] << vec2[0] << vec2[0] << 5 << vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] << vec2[1] << vec2[1] << 5 << vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] << vec2[2] << vec2[2] << 5 << vec1[2]);
// 	}
// 	{
// 		// matrix << matrix << value << matrix
// 		constexpr auto mat31 = mat1 << mat2 << 1 << mat1;
//
// 		static_assert(mat31[0] == mat1[0] << mat2[0] << 1 << mat1[0]);
// 		static_assert(mat31[1] == mat1[1] << mat2[1] << 1 << mat1[1]);
// 		static_assert(mat31[2] == mat1[2] << mat2[2] << 1 << mat1[2]);
// 		static_assert(mat31[3] == mat1[3] << mat2[3] << 1 << mat1[3]);
// 		static_assert(mat31[4] == mat1[4] << mat2[4] << 1 << mat1[4]);
// 		static_assert(mat31[5] == mat1[5] << mat2[5] << 1 << mat1[5]);
//
// 		// value << matrix << value << matrix
// 		constexpr auto mat32 = 5ull << mat1 << 1 << mat2;
//
// 		static_assert(mat32[0] == 5ull << mat1[0] << 1 << mat2[0]);
// 		static_assert(mat32[1] == 5ull << mat1[1] << 1 << mat2[1]);
// 		static_assert(mat32[2] == 5ull << mat1[2] << 1 << mat2[2]);
// 		static_assert(mat32[3] == 5ull << mat1[3] << 1 << mat2[3]);
// 		static_assert(mat32[4] == 5ull << mat1[4] << 1 << mat2[4]);
// 		static_assert(mat32[5] == 5ull << mat1[5] << 1 << mat2[5]);
//
// 		// vector << vector << value << vector_view << vector
// 		constexpr auto vec3 = vec1 << vec2 << 5 << vec1.to_view() << vec2;
//
// 		static_assert(vec3[0] == vec1[0] << vec2[0] << 5 << vec1[0] << vec2[0]);
// 		static_assert(vec3[1] == vec1[1] << vec2[1] << 5 << vec1[1] << vec2[1]);
// 		static_assert(vec3[2] == vec1[2] << vec2[2] << 5 << vec1[2] << vec2[2]);
//
// 		// value << vector << vector_view << vector
// 		constexpr auto vec32 = 5ull << vec1 << vec2.to_view() << vec2;
//
// 		static_assert(vec32[0] == 5ull << vec1[0] << vec2[0] << vec2[0]);
// 		static_assert(vec32[1] == 5ull << vec1[1] << vec2[1] << vec2[1]);
// 		static_assert(vec32[2] == 5ull << vec1[2] << vec2[2] << vec2[2]);
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorRightShift)
// {
// 	constexpr math::vector3<unsigned long long> vec1{7, 8, 9};
// 	constexpr math::vector3<unsigned long long> vec2{9, 8, 7};
//
// 	constexpr math::matrix2x3<unsigned long long> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<unsigned long long> mat2{vec2, vec1};
//
// 	{
// 		auto mat3 = mat1;
//
// 		// matrix >> matrix >> value >> matrix
// 		((mat3 >>= mat2) >>= 5) >>= mat1;
//
// 		ASSERT_EQ(mat3[0], mat1[0] >> mat2[0] >> 5 >> mat1[0]);
// 		ASSERT_EQ(mat3[1], mat1[1] >> mat2[1] >> 5 >> mat1[1]);
// 		ASSERT_EQ(mat3[2], mat1[2] >> mat2[2] >> 5 >> mat1[2]);
// 		ASSERT_EQ(mat3[3], mat1[3] >> mat2[3] >> 5 >> mat1[3]);
// 		ASSERT_EQ(mat3[4], mat1[4] >> mat2[4] >> 5 >> mat1[4]);
// 		ASSERT_EQ(mat3[5], mat1[5] >> mat2[5] >> 5 >> mat1[5]);
//
// 		auto vec3 = vec1;
//
// 		// vector >> vector >> vector_view >> vector >> value >> vector
// 		(((vec3 >>= vec2).to_view() >>= vec2) >>= 5) >>= vec1;
//
// 		ASSERT_EQ(vec3[0], vec1[0] >> vec2[0] >> vec2[0] >> 5 >> vec1[0]);
// 		ASSERT_EQ(vec3[1], vec1[1] >> vec2[1] >> vec2[1] >> 5 >> vec1[1]);
// 		ASSERT_EQ(vec3[2], vec1[2] >> vec2[2] >> vec2[2] >> 5 >> vec1[2]);
// 	}
// 	{
// 		// matrix >> matrix >> value >> matrix
// 		constexpr auto mat31 = mat1 >> mat2 >> 1 >> mat1;
//
// 		static_assert(mat31[0] == mat1[0] >> mat2[0] >> 1 >> mat1[0]);
// 		static_assert(mat31[1] == mat1[1] >> mat2[1] >> 1 >> mat1[1]);
// 		static_assert(mat31[2] == mat1[2] >> mat2[2] >> 1 >> mat1[2]);
// 		static_assert(mat31[3] == mat1[3] >> mat2[3] >> 1 >> mat1[3]);
// 		static_assert(mat31[4] == mat1[4] >> mat2[4] >> 1 >> mat1[4]);
// 		static_assert(mat31[5] == mat1[5] >> mat2[5] >> 1 >> mat1[5]);
//
// 		// value >> matrix >> value >> matrix
// 		constexpr auto mat32 = 999999999ull >> mat1 >> 1 >> mat2;
//
// 		static_assert(mat32[0] == 999999999ull >> mat1[0] >> 1 >> mat2[0]);
// 		static_assert(mat32[1] == 999999999ull >> mat1[1] >> 1 >> mat2[1]);
// 		static_assert(mat32[2] == 999999999ull >> mat1[2] >> 1 >> mat2[2]);
// 		static_assert(mat32[3] == 999999999ull >> mat1[3] >> 1 >> mat2[3]);
// 		static_assert(mat32[4] == 999999999ull >> mat1[4] >> 1 >> mat2[4]);
// 		static_assert(mat32[5] == 999999999ull >> mat1[5] >> 1 >> mat2[5]);
//
// 		// vector >> vector >> value >> vector_view >> vector
// 		constexpr auto vec3 = vec1 >> vec2 >> 5 >> vec1.to_view() >> vec2;
//
// 		static_assert(vec3[0] == vec1[0] >> vec2[0] >> 5 >> vec1[0] >> vec2[0]);
// 		static_assert(vec3[1] == vec1[1] >> vec2[1] >> 5 >> vec1[1] >> vec2[1]);
// 		static_assert(vec3[2] == vec1[2] >> vec2[2] >> 5 >> vec1[2] >> vec2[2]);
//
// 		// value >> vector >> vector_view >> vector
// 		constexpr auto vec32 = 999999999ull >> vec1 >> vec2.to_view() >> vec2;
//
// 		static_assert(vec32[0] == 999999999ull >> vec1[0] >> vec2[0] >> vec2[0]);
// 		static_assert(vec32[1] == 999999999ull >> vec1[1] >> vec2[1] >> vec2[1]);
// 		static_assert(vec32[2] == 999999999ull >> vec1[2] >> vec2[2] >> vec2[2]);
// 	}
// }
//
// TEST(TestMathOperations, TestOperatorAbs)
// {
// 	constexpr math::vector3<int> vec1{ -123, -456, 789 };
// 	constexpr auto vector2 = +vec1;
//
// 	constexpr math::matrix2x3<int> mat1{ vec1, vector2 };
// 	constexpr auto mat2 = +mat1;
//
// 	static_assert(vec1[0] == -123);
// 	static_assert(vec1[1] == -456);
// 	static_assert(vec1[2] == 789);
//
// 	static_assert(vector2[0] == 123);
// 	static_assert(vector2[1] == 456);
// 	static_assert(vector2[2] == 789);
//
// 	static_assert(mat1[0] == -123);
// 	static_assert(mat1[1] == -456);
// 	static_assert(mat1[2] == 789);
// 	static_assert(mat1[3] == 123);
// 	static_assert(mat1[4] == 456);
// 	static_assert(mat1[5] == 789);
//
// 	static_assert(mat2[0] == 123);
// 	static_assert(mat2[1] == 456);
// 	static_assert(mat2[2] == 789);
// 	static_assert(mat2[3] == 123);
// 	static_assert(mat2[4] == 456);
// 	static_assert(mat2[5] == 789);
// }
//
// TEST(TestMathOperations, TestOperatorInvert)
// {
// 	constexpr math::vector3<int> vec1{123, 456, 789};
// 	constexpr auto         vec2 = -vec1;
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr auto           mat2 = -mat1;
//
// 	static_assert(vec2[0] == -vec1[0]);
// 	static_assert(vec2[1] == -vec1[1]);
// 	static_assert(vec2[2] == -vec1[2]);
//
// 	static_assert(mat2[0] == -mat1[0]);
// 	static_assert(mat2[1] == -mat1[1]);
// 	static_assert(mat2[2] == -mat1[2]);
// 	static_assert(mat2[3] == -mat1[3]);
// 	static_assert(mat2[4] == -mat1[4]);
// 	static_assert(mat2[5] == -mat1[5]);
//
// 	auto vec3 = vec1;
// 	// the same reference
// 	auto& vec4 = math::make_inverse(vec3);
//
// 	ASSERT_EQ(&vec3, &vec4);
//
// 	ASSERT_EQ(vec3[0], vec2[0]);
// 	ASSERT_EQ(vec3[1], vec2[1]);
// 	ASSERT_EQ(vec3[2], vec2[2]);
//
// 	// not the same view
// 	auto vec5 = math::make_inverse(vec3.to_view());
//
// 	ASSERT_EQ(vec5[0], vec1[0]);
// 	ASSERT_EQ(vec5[1], vec1[1]);
// 	ASSERT_EQ(vec5[2], vec1[2]);
//
// 	auto mat3 = mat1;
// 	// the same reference
// 	auto& mat4 = math::make_inverse(mat3);
//
// 	ASSERT_EQ(&mat3, &mat4);
//
// 	ASSERT_EQ(mat3[0], mat2[0]);
// 	ASSERT_EQ(mat3[1], mat2[1]);
// 	ASSERT_EQ(mat3[2], mat2[2]);
// 	ASSERT_EQ(mat3[3], mat2[3]);
// 	ASSERT_EQ(mat3[4], mat2[4]);
// 	ASSERT_EQ(mat3[5], mat2[5]);
// }
//
// TEST(TestMathOperations, TestOperatorBitwiseNot)
// {
// 	constexpr math::vector3<unsigned> vec1{123, 456, 789};
// 	constexpr auto              vec2 = ~vec1;
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr auto           mat2 = ~mat1;
//
// 	static_assert(vec2[0] == ~vec1[0]);
// 	static_assert(vec2[1] == ~vec1[1]);
// 	static_assert(vec2[2] == ~vec1[2]);
//
// 	static_assert(mat2[0] == ~mat1[0]);
// 	static_assert(mat2[1] == ~mat1[1]);
// 	static_assert(mat2[2] == ~mat1[2]);
// 	static_assert(mat2[3] == ~mat1[3]);
// 	static_assert(mat2[4] == ~mat1[4]);
// 	static_assert(mat2[5] == ~mat1[5]);
//
// 	auto vec3 = vec1;
// 	// the same reference
// 	auto& vec4 = math::make_bitwise_not(vec3);
//
// 	ASSERT_EQ(&vec3, &vec4);
//
// 	ASSERT_EQ(vec3[0], vec2[0]);
// 	ASSERT_EQ(vec3[1], vec2[1]);
// 	ASSERT_EQ(vec3[2], vec2[2]);
//
// 	// not the same view
// 	auto vec5 = math::make_bitwise_not(vec3.to_view());
//
// 	ASSERT_EQ(vec5[0], vec1[0]);
// 	ASSERT_EQ(vec5[1], vec1[1]);
// 	ASSERT_EQ(vec5[2], vec1[2]);
//
// 	auto mat3 = mat1;
// 	// the same reference
// 	auto& mat4 = math::make_bitwise_not(mat3);
//
// 	ASSERT_EQ(&mat3, &mat4);
//
// 	ASSERT_EQ(mat3[0], mat2[0]);
// 	ASSERT_EQ(mat3[1], mat2[1]);
// 	ASSERT_EQ(mat3[2], mat2[2]);
// 	ASSERT_EQ(mat3[3], mat2[3]);
// 	ASSERT_EQ(mat3[4], mat2[4]);
// 	ASSERT_EQ(mat3[5], mat2[5]);
// }
//
// TEST(TestMathOperations, TestOperatorEqualAndNotEqual)
// {
// 	constexpr math::vector3<unsigned> vec1{123, 456, 789};
// 	constexpr math::vector3<unsigned> vec2{123, 123, 123};
//
// 	constexpr auto vec31 = (vec1 == vec2);
// 	constexpr auto vec32 = (vec1 == 456);
// 	constexpr auto vec33 = (456 == vec1);
//
// 	static_assert(vec31[0]);
// 	static_assert(!vec31[1]);
// 	static_assert(!vec31[2]);
//
// 	static_assert(!vec32[0]);
// 	static_assert(vec32[1]);
// 	static_assert(!vec32[2]);
//
// 	static_assert(!vec33[0]);
// 	static_assert(vec33[1]);
// 	static_assert(!vec33[2]);
//
// 	constexpr auto vec41 = (vec1 != vec2);
// 	constexpr auto vec42 = (vec1 != 456);
// 	constexpr auto vec43 = (456 != vec1);
//
// 	static_assert(!vec41[0]);
// 	static_assert(vec41[1]);
// 	static_assert(vec41[2]);
//
// 	static_assert(vec42[0]);
// 	static_assert(!vec42[1]);
// 	static_assert(vec42[2]);
//
// 	static_assert(vec43[0]);
// 	static_assert(!vec43[1]);
// 	static_assert(vec43[2]);
//
// 	constexpr math::matrix2x3<int> mat1{vec1, vec2};
// 	constexpr math::matrix2x3<int> mat2{vec2, vec1};
//
// 	constexpr auto mat31 = (mat1 == mat2);
// 	constexpr auto mat32 = (mat1 == 123);
// 	constexpr auto mat33 = (123 == mat1);
//
// 	static_assert(mat31[0]);
// 	static_assert(!mat31[1]);
// 	static_assert(!mat31[2]);
// 	static_assert(mat31[3]);
// 	static_assert(!mat31[4]);
// 	static_assert(!mat31[5]);
//
// 	static_assert(mat32[0]);
// 	static_assert(!mat32[1]);
// 	static_assert(!mat32[2]);
// 	static_assert(mat32[3]);
// 	static_assert(mat32[4]);
// 	static_assert(mat32[5]);
//
// 	static_assert(mat33[0]);
// 	static_assert(!mat33[1]);
// 	static_assert(!mat33[2]);
// 	static_assert(mat33[3]);
// 	static_assert(mat33[4]);
// 	static_assert(mat33[5]);
//
// 	constexpr auto mat41 = (mat1 != mat2);
// 	constexpr auto mat42 = (mat1 != 123);
// 	constexpr auto mat43 = (123 != mat1);
//
// 	static_assert(!mat41[0]);
// 	static_assert(mat41[1]);
// 	static_assert(mat41[2]);
// 	static_assert(!mat41[3]);
// 	static_assert(mat41[4]);
// 	static_assert(mat41[5]);
//
// 	static_assert(!mat42[0]);
// 	static_assert(mat42[1]);
// 	static_assert(mat42[2]);
// 	static_assert(!mat42[3]);
// 	static_assert(!mat42[4]);
// 	static_assert(!mat42[5]);
//
// 	static_assert(!mat43[0]);
// 	static_assert(mat43[1]);
// 	static_assert(mat43[2]);
// 	static_assert(!mat43[3]);
// 	static_assert(!mat43[4]);
// 	static_assert(!mat43[5]);
// }
//
// TEST(TestMathOperations, TestOperatorEqualAndNotEqualFloatingPoint)
// {
// 	// +1.0e-10
// 	constexpr math::vector3<double> vec1{ 1.2300000001, 4.5600000001, 7.8900000001 };
// 	constexpr math::vector3<double> vec2{ 1.2300000001, 1.2300000001, 1.2300000001 };
//
// 	// epsilon = 1.0e-9
// 	constexpr auto epsilon = 1.0e-9;
// 	
// 	constexpr auto vec31 = math::equal(vec1, vec2, epsilon);
// 	constexpr auto vec32 = math::equal(vec1, 4.5600000001, epsilon);
// 	constexpr auto vec33 = math::equal(4.5600000001, vec1, epsilon);
//
// 	static_assert(vec31[0]);
// 	static_assert(!vec31[1]);
// 	static_assert(!vec31[2]);
//
// 	static_assert(!vec32[0]);
// 	static_assert(vec32[1]);
// 	static_assert(!vec32[2]);
//
// 	static_assert(!vec33[0]);
// 	static_assert(vec33[1]);
// 	static_assert(!vec33[2]);
//
// 	constexpr auto vec41 = math::not_equal(vec1, vec2, epsilon);
// 	constexpr auto vec42 = math::not_equal(vec1, 4.5600000001, epsilon);
// 	constexpr auto vec43 = math::not_equal(4.5600000001, vec1, epsilon);
//
// 	static_assert(!vec41[0]);
// 	static_assert(vec41[1]);
// 	static_assert(vec41[2]);
//
// 	static_assert(vec42[0]);
// 	static_assert(!vec42[1]);
// 	static_assert(vec42[2]);
//
// 	static_assert(vec43[0]);
// 	static_assert(!vec43[1]);
// 	static_assert(vec43[2]);
//
// 	constexpr math::matrix2x3<double> mat1{ vec1, vec2 };
// 	constexpr math::matrix2x3<double> mat2{ vec2, vec1 };
//
// 	constexpr auto mat31 = math::equal(mat1, mat2, epsilon);
// 	constexpr auto mat32 = math::equal(mat1, 1.2300000001, epsilon);
// 	constexpr auto mat33 = math::equal(1.2300000001, mat1, epsilon);
//
// 	static_assert(mat31[0]);
// 	static_assert(!mat31[1]);
// 	static_assert(!mat31[2]);
// 	static_assert(mat31[3]);
// 	static_assert(!mat31[4]);
// 	static_assert(!mat31[5]);
//
// 	static_assert(mat32[0]);
// 	static_assert(!mat32[1]);
// 	static_assert(!mat32[2]);
// 	static_assert(mat32[3]);
// 	static_assert(mat32[4]);
// 	static_assert(mat32[5]);
//
// 	static_assert(mat33[0]);
// 	static_assert(!mat33[1]);
// 	static_assert(!mat33[2]);
// 	static_assert(mat33[3]);
// 	static_assert(mat33[4]);
// 	static_assert(mat33[5]);
//
// 	constexpr auto mat41 = math::not_equal(mat1, mat2, epsilon);
// 	constexpr auto mat42 = math::not_equal(mat1, 1.2300000001, epsilon);
// 	constexpr auto mat43 = math::not_equal(1.2300000001, mat1, epsilon);
//
// 	static_assert(!mat41[0]);
// 	static_assert(mat41[1]);
// 	static_assert(mat41[2]);
// 	static_assert(!mat41[3]);
// 	static_assert(mat41[4]);
// 	static_assert(mat41[5]);
//
// 	static_assert(!mat42[0]);
// 	static_assert(mat42[1]);
// 	static_assert(mat42[2]);
// 	static_assert(!mat42[3]);
// 	static_assert(!mat42[4]);
// 	static_assert(!mat42[5]);
//
// 	static_assert(!mat43[0]);
// 	static_assert(mat43[1]);
// 	static_assert(mat43[2]);
// 	static_assert(!mat43[3]);
// 	static_assert(!mat43[4]);
// 	static_assert(!mat43[5]);
// }
//
// TEST(TestMathOperations, TestOperatorGreaterAndLess)
// {
// 	constexpr math::vector3<unsigned> vec1{ 123, 456, 789 };
// 	constexpr math::vector3<unsigned> vec2{ 123, 123, 123 };
//
// 	constexpr auto vec31 = vec1 > vec2;
// 	constexpr auto vec32 = vec1 > 456;
// 	constexpr auto vec33 = 456 > vec1;
//
// 	static_assert(!vec31[0]);
// 	static_assert(vec31[1]);
// 	static_assert(vec31[2]);
//
// 	static_assert(!vec32[0]);
// 	static_assert(!vec32[1]);
// 	static_assert(vec32[2]);
//
// 	static_assert(vec33[0]);
// 	static_assert(!vec33[1]);
// 	static_assert(!vec33[2]);
//
// 	constexpr auto vec41 = vec1 >= vec2;
// 	constexpr auto vec42 = vec1 >= 456;
// 	constexpr auto vec43 = 456 >= vec1;
//
// 	static_assert(vec41[0]);
// 	static_assert(vec41[1]);
// 	static_assert(vec41[2]);
//
// 	static_assert(!vec42[0]);
// 	static_assert(vec42[1]);
// 	static_assert(vec42[2]);
//
// 	static_assert(vec43[0]);
// 	static_assert(vec43[1]);
// 	static_assert(!vec43[2]);
//
// 	constexpr auto vec51 = vec1 < vec2;
// 	constexpr auto vec52 = vec1 < 456;
// 	constexpr auto vec53 = 456 < vec1;
// 	
// 	static_assert(!vec51[0]);
// 	static_assert(!vec51[1]);
// 	static_assert(!vec51[2]);
// 	
// 	static_assert(vec52[0]);
// 	static_assert(!vec52[1]);
// 	static_assert(!vec52[2]);
// 	
// 	static_assert(!vec53[0]);
// 	static_assert(!vec53[1]);
// 	static_assert(vec53[2]);
// 	
// 	constexpr auto vec61 = vec1 <= vec2;
// 	constexpr auto vec62 = vec1 <= 456;
// 	constexpr auto vec63 = 456 <= vec1;
// 	
// 	static_assert(vec61[0]);
// 	static_assert(!vec61[1]);
// 	static_assert(!vec61[2]);
// 	
// 	static_assert(vec62[0]);
// 	static_assert(vec62[1]);
// 	static_assert(!vec62[2]);
// 	
// 	static_assert(!vec63[0]);
// 	static_assert(vec63[1]);
// 	static_assert(vec63[2]);
// }
//
// TEST(TestMathOperations, TestRanges)
// {
// 	constexpr math::vector3<unsigned> vec1{ 123, 456, 789 };
// 	constexpr math::vector3<unsigned> vec2{ 123, 123, 123 };
//
// 	constexpr math::matrix2x3<int> mat1{ vec1, vec2 };
// 	constexpr math::matrix2x3<int> mat2{ vec2, vec1 };
//
// 	static_assert(std::ranges::all_of(vec1, [](const int i) {return i > 100; }));
// 	static_assert(std::ranges::any_of(vec1, [](const int i) {return i > 456; }));
// 	static_assert(std::ranges::none_of(vec2, [](const int i) {return i > 123; }));
//
// 	static_assert(std::ranges::all_of(mat1, [](const int i) {return i > 100; }));
// 	static_assert(std::ranges::any_of(mat1, [](const int i) {return i > 456; }));
// 	static_assert(std::ranges::none_of(mat2, [](const int i) {return i > 789; }));
// }
