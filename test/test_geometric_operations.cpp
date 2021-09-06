#include <gtest/gtest.h>

#include "../src/math/basic_operations.hpp"
#include "../src/math/geometric_operations.hpp"
#include "../src/math/trigonometric_operations.hpp"
#include "../src/math/numeric_and_algorithm.hpp"

using namespace gal::test::math;

TEST(TestMathOperations, TestDot)
{
	constexpr vector3<float> vec1{ 1.0f, 2.0f, 3.0f };
	constexpr vector3<float> vec2{ 3.0f, 2.0f, 1.0f };

	constexpr auto vec3 = vec1 * vec2;
	
	static_assert(sum(vec3) == dot(vec1, vec2));
}

TEST(TestMathOperations, TestCross)
{
	constexpr vector3<float> vec1{ 1.0f, 0.0f, 0.0f };
	constexpr vector3<float> vec2{ 0.0f, 1.0f, 0.0f };
	constexpr vector3<float> vec3{ 0.0f, 0.0f, 1.0f };

	constexpr auto vec41 = cross(vec1, vec2);
	constexpr auto vec42 = cross(vec1, vec3);
	constexpr auto vec43 = cross(vec2, vec3);

	constexpr auto float_eq = [](const float f1, const float f2) constexpr
	{
		return f1 - f2 >= -0.000001f && f1 - f2 <= 0.000001f;
	};
	
	static_assert(float_eq(vec41.x(), 0));
	static_assert(float_eq(vec41.y(), 0));
	static_assert(float_eq(vec41.z(), 1.0f));
	static_assert(float_eq(vec42.x(), 0));
	static_assert(float_eq(vec42.y(), -1.0f));
	static_assert(float_eq(vec42.z(), 0));
	static_assert(float_eq(vec43.x(), 1.0f));
	static_assert(float_eq(vec43.y(), 0));
	static_assert(float_eq(vec43.z(), 0));

	constexpr vector3<float> vec51{ 1.0f, 1.0f, 0.0f };
	constexpr vector3<float> vec52{ 0.0f, 0.0f, 1.0f };
	constexpr vector3<float> vec53{ 0.0f, 0.0f, -1.0f };

	constexpr auto vec61 = cross(vec51, vec52);
	constexpr auto vec62 = cross(vec51, vec53);
	constexpr auto vec63 = cross(vec52, vec53);

	static_assert(float_eq(vec61.x(), 1.0f));
	static_assert(float_eq(vec61.y(), -1.0f));
	static_assert(float_eq(vec61.z(), 0.0f));
	static_assert(float_eq(vec62.x(), -1.0f));
	static_assert(float_eq(vec62.y(), 1.0f));
	static_assert(float_eq(vec62.z(), 0.0f));
	static_assert(float_eq(vec63.x(), -0.0f));
	static_assert(float_eq(vec63.y(), 0.0f));
	static_assert(float_eq(vec63.z(), 0.0f));
}

TEST(TestMathOperations, TestLength)
{
	constexpr vector3<float> vec1{ 1.0f, 0.0f, 0.0f };
	constexpr vector3<float> vec2{ 0.0f, 0.0f, 1.0f };
	constexpr vector3<float> vec3{0.0f, 3.0f, 4.0f};

	const auto l1 = length(vec1);
	const auto l2 = length(vec2);
	const auto l3 = length(vec3);

	ASSERT_EQ(l1, 1.f);
	ASSERT_EQ(l1, l2);
	ASSERT_EQ(l3, 5.0f);
}

TEST(TestMathOperations, TestDistance)
{
	constexpr vector3<float> vec1{ 1.0f, 2.0f, 3.0f };
	constexpr vector3<float> vec2{ 3.0f, 2.0f, 1.0f };

	constexpr auto vec3 = vec2 - vec1;
	constexpr auto d = dot(vec3, vec3);

	const auto dis = std::sqrt(d);

	ASSERT_FLOAT_EQ(dis, distance(vec1, vec2));
	ASSERT_FLOAT_EQ(std::sqrt(2.0f * 2.0f + 0.0f * 0.0f + 2.0f * 2.0f), distance(vec1, vec2));
}

TEST(TestMathOperations, TestNormalize)
{
	constexpr vector3<float> vec1{ 1.0f, 0.0f, 0.0f };
	constexpr vector3<float> vec2{ 2.0f, 0.0f, 0.0f };
	constexpr vector3<float> vec3{ -0.6f, 0.7f, -0.5f };

	const auto n1 = normalize(vec1);
	const auto n2 = normalize(vec2);
	const auto n3 = normalize(vec3);

	// todo: the error is too large ( > 0.01)
	EXPECT_FLOAT_EQ(n1.x(), 1.0f);
	EXPECT_FLOAT_EQ(n1.y(), 0.0f);
	EXPECT_FLOAT_EQ(n1.z(), 0.0f);
	EXPECT_FLOAT_EQ(n2.x(), 1.0f);
	EXPECT_FLOAT_EQ(n2.y(), 0.0f);
	EXPECT_FLOAT_EQ(n2.z(), 0.0f);
	EXPECT_FLOAT_EQ(n3.x(), -0.6f);
	EXPECT_FLOAT_EQ(n3.y(), 0.7f);
	EXPECT_FLOAT_EQ(n3.z(), -0.5f);
}

TEST(TestMathOperations, TestRelect)
{
	constexpr vector2<float> vec1{ 1.0f, -1.0f };
	constexpr vector2<float> vec2{ 0.0f, 1.0f };
	constexpr auto vec3 = reflect(vec1, vec2);

	constexpr auto float_eq = [](const float f1, const float f2) constexpr
	{
		return f1 - f2 >= -0.000001f && f1 - f2 <= 0.000001f;
	};
	
	static_assert(float_eq(vec3.x(), 1.0f));
	static_assert(float_eq(vec3.y(), 1.0f));
}

TEST(TestMathOperations, TestRefract)
{
	constexpr vector2<float> vec1{ 0.0f, -1.0f };
	constexpr vector2<float> vec2{ 0.0f, 1.0f };

	// std::sqrt is used, cannot be constexpr
	const auto vec3 = refract(vec1, vec2, 0.5);

	ASSERT_FLOAT_EQ(vec3.x(), 0.0f);
	ASSERT_FLOAT_EQ(vec3.y(), -1.0f);
}
