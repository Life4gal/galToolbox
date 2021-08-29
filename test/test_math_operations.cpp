#include <gtest/gtest.h>

#include "../src/math/math_operations.hpp"

TEST(TestMathOperations, TestSelfOperator)
{
	using namespace gal::test::new_math;

	constexpr vector<int, 3> vector1{ 1, 2, 3 };
	constexpr vector<int, 3> vector2{ 3, 2, 1 };

	vector<int, 3> vector3 = vector1;

	vector3 += vector2;

	ASSERT_EQ(vector3[0], vector1[0] + vector2[0]);
	ASSERT_EQ(vector3[1], vector1[1] + vector2[1]);
	ASSERT_EQ(vector3[2], vector1[2] + vector2[2]);
}
