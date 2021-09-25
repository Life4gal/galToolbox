#include <gtest/gtest.h>

#include <math/constants.hpp>
#include <math/trigonometric_operations.hpp>

using namespace gal::toolbox::math;

TEST(TestMathOperations, TestRadians)
{
	constexpr vector4<float> vec1{45, 90, 180, 360};

	constexpr auto vec2 = radians(vec1);

	constexpr auto float_eq = [](const float f1, const float f2) constexpr
	{
		return f1 - f2 >= -0.000001f && f1 - f2 <= 0.000001f;
	};

	static_assert(float_eq(vec2.x(), quarter_pi<float>()));
	static_assert(float_eq(vec2.y(), half_pi<float>()));
	static_assert(float_eq(vec2.z(), pi<float>()));
	static_assert(float_eq(vec2.w(), two_pi<float>()));
}

TEST(TestMathOperations, TestDegrees)
{
	constexpr vector4<float> vec1{quarter_pi<float>(), half_pi<float>(), pi<float>(), two_pi<float>()};

	constexpr auto vec2 = degrees(vec1);

	constexpr auto float_eq = [](const float f1, const float f2) constexpr
	{
		return f1 - f2 >= -0.000001f && f1 - f2 <= 0.000001f;
	};

	static_assert(float_eq(vec2.x(), 45.f));
	static_assert(float_eq(vec2.y(), 90.f));
	static_assert(float_eq(vec2.z(), 180.f));
	static_assert(float_eq(vec2.w(), 360.f));
}

TEST(TestMathOperations, TestSin)
{
	constexpr vector4<float> vec1{30, 45, 60, 90};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = sin(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), 0.5f);
	ASSERT_FLOAT_EQ(vec3.y(), root_two<float>() / 2);
	ASSERT_FLOAT_EQ(vec3.z(), root_three<float>() / 2);
	ASSERT_FLOAT_EQ(vec3.w(), 1.0f);
}

TEST(TestMathOperations, TestHyperbolicSin)
{
	constexpr vector4<float> vec1{45, 90, 180, 360};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = sinh(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), std::sinh(quarter_pi<float>()));
	ASSERT_FLOAT_EQ(vec3.y(), std::sinh(half_pi<float>()));
	ASSERT_FLOAT_EQ(vec3.z(), std::sinh(pi<float>()));
	ASSERT_FLOAT_EQ(vec3.w(), std::sinh(two_pi<float>()));
}

TEST(TestMathOperations, TestCos)
{
	constexpr vector4<float> vec1{30, 45, 60, 90};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = cos(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), root_three<float>() / 2);
	ASSERT_FLOAT_EQ(vec3.y(), root_two<float>() / 2);
	ASSERT_FLOAT_EQ(vec3.z(), 0.5f);
	// todo: not equal -> w = -4.3711388e-08 (???)
	EXPECT_FLOAT_EQ(vec3.w(), 0.0f);
}

TEST(TestMathOperations, TestHyperbolicCos)
{
	constexpr vector4<float> vec1{45, 90, 180, 360};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = cosh(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), std::cosh(quarter_pi<float>()));
	ASSERT_FLOAT_EQ(vec3.y(), std::cosh(half_pi<float>()));
	ASSERT_FLOAT_EQ(vec3.z(), std::cosh(pi<float>()));
	ASSERT_FLOAT_EQ(vec3.w(), std::cosh(two_pi<float>()));
}

TEST(TestMathOperations, TestTan)
{
	constexpr vector3<float> vec1{30, 45, 60};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = tan(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), root_three<float>() / 3);
	ASSERT_FLOAT_EQ(vec3.y(), 1.0f);
	ASSERT_FLOAT_EQ(vec3.z(), root_three<float>());
}

TEST(TestMathOperations, TestHyperbolicTan)
{
	constexpr vector3<float> vec1{45, 180, 360};

	constexpr auto vec2 = radians(vec1);

	const auto vec3 = tanh(vec2);

	ASSERT_FLOAT_EQ(vec3.x(), std::tanh(quarter_pi<float>()));
	ASSERT_FLOAT_EQ(vec3.y(), std::tanh(pi<float>()));
	ASSERT_FLOAT_EQ(vec3.z(), std::tanh(two_pi<float>()));
}

TEST(TestMathOperations, TestArcSin)
{
	constexpr vector4<float> vec1{0.5f, root_two<float>() / 2, root_three<float>() / 2, 1.0f};

	const auto vec2 = asin(vec1);

	ASSERT_FLOAT_EQ(vec2.x(), pi<float>() / 6);
	ASSERT_FLOAT_EQ(vec2.y(), quarter_pi<float>());
	ASSERT_FLOAT_EQ(vec2.z(), pi<float>() / 3);
	ASSERT_FLOAT_EQ(vec2.w(), half_pi<float>());
}

TEST(TestMathOperations, TestArcCos)
{
	constexpr vector4<float> vec1{ root_three<float>() / 2 , root_two<float>() / 2 , 0.5f , 0.0f };

	const auto vec2 = acos(vec1);

	ASSERT_FLOAT_EQ(vec2.x(), pi<float>() / 6);
	ASSERT_FLOAT_EQ(vec2.y(), quarter_pi<float>());
	ASSERT_FLOAT_EQ(vec2.z(), pi<float>() / 3);
	ASSERT_FLOAT_EQ(vec2.w(), half_pi<float>());
}

TEST(TestMathOperations, TestArcTan)
{
	constexpr vector3<float> vec1{ root_three<float>() / 3 , 1.0f, root_three<float>() };

	const auto vec2 = atan(vec1);

	ASSERT_FLOAT_EQ(vec2.x(), pi<float>() / 6);
	ASSERT_FLOAT_EQ(vec2.y(), quarter_pi<float>());
	ASSERT_FLOAT_EQ(vec2.z(), pi<float>() / 3);
}

TEST(TestMathOperations, TestHyperbolicArcTan)
{
	constexpr vector3<float> vec1{ root_three<float>() / 3 , root_two<float>() / 2, root_five<float>() / 5 };

	const auto vec2 = atanh(tanh(vec1));
	const auto vec3 = tanh(atanh(vec1));

	ASSERT_FLOAT_EQ(vec2.x(), vec1.x());
	ASSERT_FLOAT_EQ(vec2.y(), vec1.y());
	ASSERT_FLOAT_EQ(vec2.z(), vec1.z());
	ASSERT_FLOAT_EQ(vec3.x(), vec1.x());
	ASSERT_FLOAT_EQ(vec3.y(), vec1.y());
	ASSERT_FLOAT_EQ(vec3.z(), vec1.z());
}

TEST(TestMathOperations, TestArcTan2)
{
	constexpr vector3<float> vec1{ root_three<float>() , 2.0f, root_three<float>() / 2 };
	constexpr vector3<float> vec2{ 3.0f, 2.0f, 0.5f };

	const auto vec3 = atan2(vec1, vec2);

	ASSERT_FLOAT_EQ(vec3.x(), pi<float>() / 6);
	ASSERT_FLOAT_EQ(vec3.y(), quarter_pi<float>());
	ASSERT_FLOAT_EQ(vec3.z(), pi<float>() / 3);
}
