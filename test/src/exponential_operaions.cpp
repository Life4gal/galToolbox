#include <gtest/gtest.h>

#include <gal/constants.hpp>
#include <gal/basic_operations.hpp>
#include <gal/numeric_and_algorithm.hpp>
#include <gal/exponential_operations.hpp>

using namespace gal::toolbox::math;

TEST(TestMathOperations, TestPow)
{
	constexpr vector3<float> vec1{1.0f, 2.0f, 3.0f};
	constexpr vector3<float> vec2{3.0f, 2.0f, 1.0f};

	const auto vec3 = pow(vec1, vec2);
	ASSERT_EQ(vec3.x(), std::pow(vec1.x(), vec2.x()));
	ASSERT_EQ(vec3.y(), std::pow(vec1.y(), vec2.y()));
	ASSERT_EQ(vec3.z(), std::pow(vec1.z(), vec2.z()));

	ASSERT_TRUE(all(vec3, [](const float f) { return f >= 1.0f; }));
	ASSERT_TRUE(any(vec3, [](const float f) { return f >= 4.0f; }));
	ASSERT_TRUE(none(vec3, [](const float f) { return f >= 5.0f; }));
}

TEST(TestMathOperations, TestExp)
{
	constexpr vector1<float> vec1{1.0f};
	constexpr vector2<float> vec2{vec1, vec1};
	constexpr vector3<float> vec3{vec2, vec2};
	constexpr vector4<float> vec4{vec3, vec2};

	const auto e1 = exp(vec1);
	const auto e2 = exp(vec2);
	const auto e3 = exp(vec3);
	const auto e4 = exp(vec4);

	ASSERT_TRUE(all(equal(e1, e<float>(), 0.000001f)));
	ASSERT_TRUE(all(equal(e2, e<float>(), 0.000001f)));
	ASSERT_TRUE(all(equal(e3, e<float>(), 0.000001f)));
	ASSERT_TRUE(all(equal(e4, e<float>(), 0.000001f)));
}

TEST(TestMathOperations, TestExp2)
{
	constexpr vector1<float> vec1{4.0f};
	constexpr vector2<float> vec2{vec1, 3.0f};
	constexpr vector3<float> vec3{vec2, 2.0f};
	constexpr vector4<float> vec4{vec3, 1.0f};

	constexpr vector1<float> vec11{16.0f};
	constexpr vector2<float> vec22{vec11, 8.0f};
	constexpr vector3<float> vec33{vec22, 4.0f};
	constexpr vector4<float> vec44{vec33, 2.0f};

	const auto e1 = exp2(vec1);
	const auto e2 = exp2(vec2);
	const auto e3 = exp2(vec3);
	const auto e4 = exp2(vec4);

	ASSERT_TRUE(all(equal(e1, vec11, 0.000001f)));
	ASSERT_TRUE(all(equal(e2, vec22, 0.000001f)));
	ASSERT_TRUE(all(equal(e3, vec33, 0.000001f)));
	ASSERT_TRUE(all(equal(e4, vec44, 0.000001f)));
}

TEST(TestMathOperations, TestLog)
{
	constexpr vector1<float> vec1{e<float>()};
	constexpr vector2<float> vec2{vec1, vec1};
	constexpr vector3<float> vec3{vec2, vec2};
	constexpr vector4<float> vec4{vec3, vec2};

	const auto l1 = log(vec1);
	const auto l2 = log(vec2);
	const auto l3 = log(vec3);
	const auto l4 = log(vec4);

	ASSERT_TRUE(all(equal(l1, 1.0f, 0.000001f)));
	ASSERT_TRUE(all(equal(l2, 1.0f, 0.000001f)));
	ASSERT_TRUE(all(equal(l3, 1.0f, 0.000001f)));
	ASSERT_TRUE(all(equal(l4, 1.0f, 0.000001f)));
}

TEST(TestMathOperations, TestLog2)
{
	constexpr vector1<float> vec1{16.0f};
	constexpr vector2<float> vec2{vec1, 8.0f};
	constexpr vector3<float> vec3{vec2, 4.0f};
	constexpr vector4<float> vec4{vec3, 2.0f};

	constexpr vector1<float> vec11{4.0f};
	constexpr vector2<float> vec22{vec11, 3.0f};
	constexpr vector3<float> vec33{vec22, 2.0f};
	constexpr vector4<float> vec44{vec33, 1.0f};

	const auto l1 = log2(vec1);
	const auto l2 = log2(vec2);
	const auto l3 = log2(vec3);
	const auto l4 = log2(vec4);

	ASSERT_TRUE(all(equal(l1, vec11, 0.000001f)));
	ASSERT_TRUE(all(equal(l2, vec22, 0.000001f)));
	ASSERT_TRUE(all(equal(l3, vec33, 0.000001f)));
	ASSERT_TRUE(all(equal(l4, vec44, 0.000001f)));
}

TEST(TestMathOperations, TestSqrt)
{
	constexpr vector1<float> vec1{25.0f};
	constexpr vector2<float> vec2{vec1, 16.0f};
	constexpr vector3<float> vec3{vec2, 9.0f};
	constexpr vector4<float> vec4{vec3, 4.0f};

	constexpr vector1<float> vec11{5.0f};
	constexpr vector2<float> vec22{vec11, 4.0f};
	constexpr vector3<float> vec33{vec22, 3.0f};
	constexpr vector4<float> vec44{vec33, 2.0f};

	const auto s1 = sqrt(vec1);
	const auto s2 = sqrt(vec2);
	const auto s3 = sqrt(vec3);
	const auto s4 = sqrt(vec4);

	ASSERT_TRUE(all(equal(s1, vec11, 0.000001f)));
	ASSERT_TRUE(all(equal(s2, vec22, 0.000001f)));
	ASSERT_TRUE(all(equal(s3, vec33, 0.000001f)));
	ASSERT_TRUE(all(equal(s4, vec44, 0.000001f)));
}

TEST(TestMathOperations, TestInverseSqrt)
{
	constexpr vector1<float> vec1{25.0f};
	constexpr vector2<float> vec2{vec1, 16.0f};
	constexpr vector3<float> vec3{vec2, 9.0f};
	constexpr vector4<float> vec4{vec3, 4.0f};

	const auto r1 = inverse_sqrt(vec1) * sqrt(vec1);
	const auto r2 = inverse_sqrt(vec2) * sqrt(vec2);
	const auto r3 = inverse_sqrt(vec3) * sqrt(vec3);
	const auto r4 = inverse_sqrt(vec4) * sqrt(vec4);

	ASSERT_TRUE(all(equal(r1, 1.0f, 0.01f)));
	ASSERT_TRUE(all(equal(r2, 1.0f, 0.01f)));
	ASSERT_TRUE(all(equal(r3, 1.0f, 0.01f)));
	ASSERT_TRUE(all(equal(r4, 1.0f, 0.01f)));
}
