#include <gtest/gtest.h>

#include <math/math.hpp>

#ifndef GALTOOLBOX_MATH_NOT_SUPPOTED

using namespace gal::toolbox::math;

#include <array>

TEST(TestMath, TestVectorLoad4)
{
	constexpr std::uint32_t arr1[]{0xFF, 0x00FF, 0x0000FF, 0x000000FF};
	std::uint32_t arr2[]{0xFF, 0x00FF, 0x0000FF, 0x000000FF};

	{
		// vector_load<Size>(pointer)
		constexpr auto vec1 = vector_load<4>(arr1);
		constexpr auto* p1 = vec1.m128_u32;

		static_assert(p1[0] == 0xFF);
		static_assert(p1[1] == 0x00FF);
		static_assert(p1[2] == 0x0000FF);
		static_assert(p1[3] == 0x000000FF);

		auto vec2 = vector_load<4>(arr2);
		auto* p2 = vec2.m128_u32;

		ASSERT_EQ(p2[0], static_cast<std::uint32_t>(0xFF));
		ASSERT_EQ(p2[1], static_cast<std::uint32_t>(0x00FF));
		ASSERT_EQ(p2[2], static_cast<std::uint32_t>(0x0000FF));
		ASSERT_EQ(p2[3], static_cast<std::uint32_t>(0x000000FF));
	}
	{
		// vector_load<one_tier_container_t>
		{

		}

	}
}

#endif
