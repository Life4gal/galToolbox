#include <gtest/gtest.h>

#include <array>
#include <math/math.hpp>

#ifndef GALTOOLBOX_MATH_NOT_SUPPOTED

using namespace gal::toolbox::math;

namespace
{
	constexpr auto float_eq = [](float f1, float f2) noexcept
	{
		return (f1 - f2) >= -FLT_EPSILON && (f1 - f2) <= FLT_EPSILON;
	};
}

TEST(TestMath, TestVectorLoad4)
{
	constexpr std::uint32_t u1 = 0xFF;
	constexpr std::uint32_t u2 = 0xFF0;
	constexpr std::uint32_t u3 = 0xFF00;
	constexpr std::uint32_t u4 = 0xFF000;

	constexpr float			f1 = 123.456f;
	constexpr float			f2 = 234.567f;
	constexpr float			f3 = 345.678f;
	constexpr float			f4 = 456.789f;

	constexpr std::uint32_t u32_arr1[]{u1, u2, u3, u4};
	std::uint32_t			u32_arr2[]{u1, u2, u3, u4};
	constexpr std::int32_t	i32_arr1[]{u1, u2, u3, u4};
	std::int32_t			i32_arr2[]{u1, u2, u3, u4};
	constexpr float			f32_arr1[]{f1, f2, f3, f4};
	float					f32_arr2[]{f1, f2, f3, f4};

	constexpr auto			u32_one_tier1  = make_one_tier_container<uint4>(u1, u2, u3, u4);
	auto					u32_one_tier2  = make_one_tier_container<uint4>(u1, u2, u3, u4);
	constexpr auto			u32a_one_tier1 = make_one_tier_container<uint4a>(u1, u2, u3, u4);
	auto					u32a_one_tier2 = make_one_tier_container<uint4a>(u1, u2, u3, u4);

	constexpr auto			i32_one_tier1  = make_one_tier_container<int4>(u1, u2, u3, u4);
	auto					i32_one_tier2  = make_one_tier_container<int4>(u1, u2, u3, u4);
	constexpr auto			i32a_one_tier1 = make_one_tier_container<int4a>(u1, u2, u3, u4);
	auto					i32a_one_tier2 = make_one_tier_container<int4a>(u1, u2, u3, u4);

	constexpr auto			f32_one_tier1  = make_one_tier_container<float4>(f1, f2, f3, f4);
	auto					f32_one_tier2  = make_one_tier_container<float4>(f1, f2, f3, f4);
	constexpr auto			f32a_one_tier1 = make_one_tier_container<float4a>(f1, f2, f3, f4);
	auto					f32a_one_tier2 = make_one_tier_container<float4a>(f1, f2, f3, f4);

	{
		constexpr auto	u32_vec1 = vector_load<4>(u32_arr1);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u1);
		static_assert(u32_p1[1] == u2);
		static_assert(u32_p1[2] == u3);
		static_assert(u32_p1[3] == u4);

		auto  u32_vec2 = vector_load<4>(u32_arr2);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], static_cast<std::uint32_t>(u1));
		ASSERT_EQ(u32_p2[1], static_cast<std::uint32_t>(u2));
		ASSERT_EQ(u32_p2[2], static_cast<std::uint32_t>(u3));
		ASSERT_EQ(u32_p2[3], static_cast<std::uint32_t>(u4));

		constexpr auto	i32_vec1 = vector_load<4>(i32_arr1);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == u1);
		static_assert(i32_p1[1] == u2);
		static_assert(i32_p1[2] == u3);
		static_assert(i32_p1[3] == u4);

		auto  i32_vec2 = vector_load<4>(i32_arr2);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], static_cast<std::int32_t>(u1));
		ASSERT_EQ(i32_p2[1], static_cast<std::int32_t>(u2));
		ASSERT_EQ(i32_p2[2], static_cast<std::int32_t>(u3));
		ASSERT_EQ(i32_p2[3], static_cast<std::int32_t>(u4));

		constexpr auto	f32_vec1 = vector_load<4>(f32_arr1);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f1));
		static_assert(float_eq(f32_p1[1], f2));
		static_assert(float_eq(f32_p1[2], f3));
		static_assert(float_eq(f32_p1[3], f4));

		auto  f32_vec2 = vector_load<4>(f32_arr2);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2[2], f3);
		ASSERT_FLOAT_EQ(f32_p2[3], f4);
	}
	{
		{
			constexpr auto	u32_vec1 = vector_load(u32_one_tier1);
			constexpr auto* u32_p1	 = u32_vec1.m128_u32;

			static_assert(u32_p1[0] == u1);
			static_assert(u32_p1[1] == u2);
			static_assert(u32_p1[2] == u3);
			static_assert(u32_p1[3] == u4);

			auto  u32_vec2 = vector_load(u32_one_tier2);
			auto* u32_p2   = u32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(u32_p2[3], static_cast<float>(u4));

			constexpr auto	u32a_vec1 = vector_load(u32a_one_tier1);
			constexpr auto* u32a_p1	  = u32a_vec1.m128_u32;

			static_assert(u32a_p1[0] == u1);
			static_assert(u32a_p1[1] == u2);
			static_assert(u32a_p1[2] == u3);
			static_assert(u32a_p1[3] == u4);

			auto  u32a_vec2 = vector_load(u32a_one_tier2);
			auto* u32a_p2	= u32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32a_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(u32a_p2[3], static_cast<float>(u4));
		}
		{
			constexpr auto	i32_vec1 = vector_load(i32_one_tier1);
			constexpr auto* i32_p1	 = i32_vec1.m128_i32;

			static_assert(i32_p1[0] == u1);
			static_assert(i32_p1[1] == u2);
			static_assert(i32_p1[2] == u3);
			static_assert(i32_p1[3] == u4);

			auto  i32_vec2 = vector_load(i32_one_tier2);
			auto* i32_p2   = i32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(i32_p2[3], static_cast<float>(u4));

			constexpr auto	i32a_vec1 = vector_load(i32a_one_tier1);
			constexpr auto* i32a_p1	  = i32a_vec1.m128_i32;

			static_assert(i32a_p1[0] == u1);
			static_assert(i32a_p1[1] == u2);
			static_assert(i32a_p1[2] == u3);
			static_assert(i32a_p1[3] == u4);

			auto  i32a_vec2 = vector_load(i32a_one_tier2);
			auto* i32a_p2	= i32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32a_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(i32a_p2[3], static_cast<float>(u4));
		}
		{
			constexpr auto	f32_vec1 = vector_load(f32_one_tier1);
			constexpr auto* f32_p1	 = f32_vec1.m128_f32;

			static_assert(float_eq(f32_p1[0], f1));
			static_assert(float_eq(f32_p1[1], f2));
			static_assert(float_eq(f32_p1[2], f3));
			static_assert(float_eq(f32_p1[3], f4));

			auto  f32_vec2 = vector_load(f32_one_tier2);
			auto* f32_p2   = f32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32_p2[0], f1);
			ASSERT_FLOAT_EQ(f32_p2[1], f2);
			ASSERT_FLOAT_EQ(f32_p2[2], f3);
			ASSERT_FLOAT_EQ(f32_p2[3], f4);

			constexpr auto	f32a_vec1 = vector_load(f32a_one_tier1);
			constexpr auto* f32a_p1	  = f32a_vec1.m128_f32;

			static_assert(float_eq(f32a_p1[0], f1));
			static_assert(float_eq(f32a_p1[1], f2));
			static_assert(float_eq(f32a_p1[2], f3));
			static_assert(float_eq(f32a_p1[3], f4));

			auto  f32a_vec2 = vector_load(f32a_one_tier2);
			auto* f32a_p2	= f32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32a_p2[0], f1);
			ASSERT_FLOAT_EQ(f32a_p2[1], f2);
			ASSERT_FLOAT_EQ(f32a_p2[2], f3);
			ASSERT_FLOAT_EQ(f32a_p2[3], f4);
		}
	}
}

TEST(TestMath, TestVectorLoad3)
{
	constexpr std::uint32_t u1 = 0xFF;
	constexpr std::uint32_t u2 = 0xFF0;
	constexpr std::uint32_t u3 = 0xFF00;

	constexpr float			f1 = 123.456f;
	constexpr float			f2 = 234.567f;
	constexpr float			f3 = 345.678f;

	constexpr std::uint32_t u32_arr1[]{u1, u2, u3};
	std::uint32_t			u32_arr2[]{u1, u2, u3};
	constexpr std::int32_t	i32_arr1[]{u1, u2, u3};
	std::int32_t			i32_arr2[]{u1, u2, u3};
	constexpr float			f32_arr1[]{f1, f2, f3};
	float					f32_arr2[]{f1, f2, f3};

	constexpr auto			u32_one_tier1  = make_one_tier_container<uint3>(u1, u2, u3);
	auto					u32_one_tier2  = make_one_tier_container<uint3>(u1, u2, u3);
	constexpr auto			u32a_one_tier1 = make_one_tier_container<uint3a>(u1, u2, u3);
	auto					u32a_one_tier2 = make_one_tier_container<uint3a>(u1, u2, u3);

	constexpr auto			i32_one_tier1  = make_one_tier_container<int3>(u1, u2, u3);
	auto					i32_one_tier2  = make_one_tier_container<int3>(u1, u2, u3);
	constexpr auto			i32a_one_tier1 = make_one_tier_container<int3a>(u1, u2, u3);
	auto					i32a_one_tier2 = make_one_tier_container<int3a>(u1, u2, u3);

	constexpr auto			f32_one_tier1  = make_one_tier_container<float3>(f1, f2, f3);
	auto					f32_one_tier2  = make_one_tier_container<float3>(f1, f2, f3);
	constexpr auto			f32a_one_tier1 = make_one_tier_container<float3a>(f1, f2, f3);
	auto					f32a_one_tier2 = make_one_tier_container<float3a>(f1, f2, f3);

	{
		constexpr auto	u32_vec1 = vector_load<3>(u32_arr1);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u1);
		static_assert(u32_p1[1] == u2);
		static_assert(u32_p1[2] == u3);
		static_assert(u32_p1[3] == 0);

		auto  u32_vec2 = vector_load<3>(u32_arr2);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], static_cast<std::uint32_t>(u1));
		ASSERT_EQ(u32_p2[1], static_cast<std::uint32_t>(u2));
		ASSERT_EQ(u32_p2[2], static_cast<std::uint32_t>(u3));
		ASSERT_EQ(u32_p2[3], static_cast<std::uint32_t>(0));

		constexpr auto	i32_vec1 = vector_load<3>(i32_arr1);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == u1);
		static_assert(i32_p1[1] == u2);
		static_assert(i32_p1[2] == u3);
		static_assert(i32_p1[3] == 0);

		auto  i32_vec2 = vector_load<3>(i32_arr2);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], static_cast<std::int32_t>(u1));
		ASSERT_EQ(i32_p2[1], static_cast<std::int32_t>(u2));
		ASSERT_EQ(i32_p2[2], static_cast<std::int32_t>(u3));
		ASSERT_EQ(i32_p2[3], static_cast<std::int32_t>(0));

		constexpr auto	f32_vec1 = vector_load<3>(f32_arr1);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f1));
		static_assert(float_eq(f32_p1[1], f2));
		static_assert(float_eq(f32_p1[2], f3));
		static_assert(float_eq(f32_p1[3], 0));

		auto  f32_vec2 = vector_load<3>(f32_arr2);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2[2], f3);
		ASSERT_FLOAT_EQ(f32_p2[3], 0);
	}
	{
		{
			constexpr auto	u32_vec1 = vector_load(u32_one_tier1);
			constexpr auto* u32_p1	 = u32_vec1.m128_u32;

			static_assert(u32_p1[0] == u1);
			static_assert(u32_p1[1] == u2);
			static_assert(u32_p1[2] == u3);
			static_assert(u32_p1[3] == 0);

			auto  u32_vec2 = vector_load(u32_one_tier2);
			auto* u32_p2   = u32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(u32_p2[3], static_cast<float>(0));

			constexpr auto	u32a_vec1 = vector_load(u32a_one_tier1);
			constexpr auto* u32a_p1	  = u32a_vec1.m128_u32;

			static_assert(u32a_p1[0] == u1);
			static_assert(u32a_p1[1] == u2);
			static_assert(u32a_p1[2] == u3);
			static_assert(u32a_p1[3] == 0);

			auto  u32a_vec2 = vector_load(u32a_one_tier2);
			auto* u32a_p2	= u32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32a_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(u32a_p2[3], static_cast<float>(0));
		}
		{
			constexpr auto	i32_vec1 = vector_load(i32_one_tier1);
			constexpr auto* i32_p1	 = i32_vec1.m128_i32;

			static_assert(i32_p1[0] == u1);
			static_assert(i32_p1[1] == u2);
			static_assert(i32_p1[2] == u3);
			static_assert(i32_p1[3] == 0);

			auto  i32_vec2 = vector_load(i32_one_tier2);
			auto* i32_p2   = i32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(i32_p2[3], static_cast<float>(0));

			constexpr auto	i32a_vec1 = vector_load(i32a_one_tier1);
			constexpr auto* i32a_p1	  = i32a_vec1.m128_i32;

			static_assert(i32a_p1[0] == u1);
			static_assert(i32a_p1[1] == u2);
			static_assert(i32a_p1[2] == u3);
			static_assert(i32a_p1[3] == 0);

			auto  i32a_vec2 = vector_load(i32a_one_tier2);
			auto* i32a_p2	= i32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32a_p2[2], static_cast<float>(u3));
			ASSERT_FLOAT_EQ(i32a_p2[3], static_cast<float>(0));
		}
		{
			constexpr auto	f32_vec1 = vector_load(f32_one_tier1);
			constexpr auto* f32_p1	 = f32_vec1.m128_f32;

			static_assert(float_eq(f32_p1[0], f1));
			static_assert(float_eq(f32_p1[1], f2));
			static_assert(float_eq(f32_p1[2], f3));
			static_assert(float_eq(f32_p1[3], 0));

			auto  f32_vec2 = vector_load(f32_one_tier2);
			auto* f32_p2   = f32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32_p2[0], f1);
			ASSERT_FLOAT_EQ(f32_p2[1], f2);
			ASSERT_FLOAT_EQ(f32_p2[2], f3);
			ASSERT_FLOAT_EQ(f32_p2[3], 0);

			constexpr auto	f32a_vec1 = vector_load(f32a_one_tier1);
			constexpr auto* f32a_p1	  = f32a_vec1.m128_f32;

			static_assert(float_eq(f32a_p1[0], f1));
			static_assert(float_eq(f32a_p1[1], f2));
			static_assert(float_eq(f32a_p1[2], f3));
			static_assert(float_eq(f32a_p1[3], 0));

			auto  f32a_vec2 = vector_load(f32a_one_tier2);
			auto* f32a_p2	= f32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32a_p2[0], f1);
			ASSERT_FLOAT_EQ(f32a_p2[1], f2);
			ASSERT_FLOAT_EQ(f32a_p2[2], f3);
			ASSERT_FLOAT_EQ(f32a_p2[3], 0);
		}
	}
}

TEST(TestMath, TestVectorLoad2)
{
	constexpr std::uint32_t u1 = 0xFF;
	constexpr std::uint32_t u2 = 0xFF0;

	constexpr float			f1 = 123.456f;
	constexpr float			f2 = 234.567f;

	constexpr std::uint32_t u32_arr1[]{u1, u2};
	std::uint32_t			u32_arr2[]{u1, u2};
	constexpr std::int32_t	i32_arr1[]{u1, u2};
	std::int32_t			i32_arr2[]{u1, u2};
	constexpr float			f32_arr1[]{f1, f2};
	float					f32_arr2[]{f1, f2};

	constexpr auto			u32_one_tier1  = make_one_tier_container<uint2>(u1, u2);
	auto					u32_one_tier2  = make_one_tier_container<uint2>(u1, u2);
	constexpr auto			u32a_one_tier1 = make_one_tier_container<uint2a>(u1, u2);
	auto					u32a_one_tier2 = make_one_tier_container<uint2a>(u1, u2);

	constexpr auto			i32_one_tier1  = make_one_tier_container<int2>(u1, u2);
	auto					i32_one_tier2  = make_one_tier_container<int2>(u1, u2);
	constexpr auto			i32a_one_tier1 = make_one_tier_container<int2a>(u1, u2);
	auto					i32a_one_tier2 = make_one_tier_container<int2a>(u1, u2);

	constexpr auto			f32_one_tier1  = make_one_tier_container<float2>(f1, f2);
	auto					f32_one_tier2  = make_one_tier_container<float2>(f1, f2);
	constexpr auto			f32a_one_tier1 = make_one_tier_container<float2a>(f1, f2);
	auto					f32a_one_tier2 = make_one_tier_container<float2a>(f1, f2);

	{
		constexpr auto	u32_vec1 = vector_load<2>(u32_arr1);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u1);
		static_assert(u32_p1[1] == u2);
		static_assert(u32_p1[2] == 0);
		static_assert(u32_p1[3] == 0);

		auto  u32_vec2 = vector_load<2>(u32_arr2);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], static_cast<std::uint32_t>(u1));
		ASSERT_EQ(u32_p2[1], static_cast<std::uint32_t>(u2));
		ASSERT_EQ(u32_p2[2], static_cast<std::uint32_t>(0));
		ASSERT_EQ(u32_p2[3], static_cast<std::uint32_t>(0));

		constexpr auto	i32_vec1 = vector_load<2>(i32_arr1);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == u1);
		static_assert(i32_p1[1] == u2);
		static_assert(i32_p1[2] == 0);
		static_assert(i32_p1[3] == 0);

		auto  i32_vec2 = vector_load<2>(i32_arr2);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], static_cast<std::int32_t>(u1));
		ASSERT_EQ(i32_p2[1], static_cast<std::int32_t>(u2));
		ASSERT_EQ(i32_p2[2], static_cast<std::int32_t>(0));
		ASSERT_EQ(i32_p2[3], static_cast<std::int32_t>(0));

		constexpr auto	f32_vec1 = vector_load<2>(f32_arr1);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f1));
		static_assert(float_eq(f32_p1[1], f2));
		static_assert(float_eq(f32_p1[2], 0));
		static_assert(float_eq(f32_p1[3], 0));

		auto  f32_vec2 = vector_load<2>(f32_arr2);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2[2], 0);
		ASSERT_FLOAT_EQ(f32_p2[3], 0);
	}
	{
		{
			constexpr auto	u32_vec1 = vector_load(u32_one_tier1);
			constexpr auto* u32_p1	 = u32_vec1.m128_u32;

			static_assert(u32_p1[0] == u1);
			static_assert(u32_p1[1] == u2);
			static_assert(u32_p1[2] == 0);
			static_assert(u32_p1[3] == 0);

			auto  u32_vec2 = vector_load(u32_one_tier2);
			auto* u32_p2   = u32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32_p2[2], static_cast<float>(0));
			ASSERT_FLOAT_EQ(u32_p2[3], static_cast<float>(0));

			constexpr auto	u32a_vec1 = vector_load(u32a_one_tier1);
			constexpr auto* u32a_p1	  = u32a_vec1.m128_u32;

			static_assert(u32a_p1[0] == u1);
			static_assert(u32a_p1[1] == u2);
			static_assert(u32a_p1[2] == 0);
			static_assert(u32a_p1[3] == 0);

			auto  u32a_vec2 = vector_load(u32a_one_tier2);
			auto* u32a_p2	= u32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(u32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(u32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(u32a_p2[2], static_cast<float>(0));
			ASSERT_FLOAT_EQ(u32a_p2[3], static_cast<float>(0));
		}
		{
			constexpr auto	i32_vec1 = vector_load(i32_one_tier1);
			constexpr auto* i32_p1	 = i32_vec1.m128_i32;

			static_assert(i32_p1[0] == u1);
			static_assert(i32_p1[1] == u2);
			static_assert(i32_p1[2] == 0);
			static_assert(i32_p1[3] == 0);

			auto  i32_vec2 = vector_load(i32_one_tier2);
			auto* i32_p2   = i32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32_p2[2], static_cast<float>(0));
			ASSERT_FLOAT_EQ(i32_p2[3], static_cast<float>(0));

			constexpr auto	i32a_vec1 = vector_load(i32a_one_tier1);
			constexpr auto* i32a_p1	  = i32a_vec1.m128_i32;

			static_assert(i32a_p1[0] == u1);
			static_assert(i32a_p1[1] == u2);
			static_assert(i32a_p1[2] == 0);
			static_assert(i32a_p1[3] == 0);

			auto  i32a_vec2 = vector_load(i32a_one_tier2);
			auto* i32a_p2	= i32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(i32a_p2[0], static_cast<float>(u1));
			ASSERT_FLOAT_EQ(i32a_p2[1], static_cast<float>(u2));
			ASSERT_FLOAT_EQ(i32a_p2[2], static_cast<float>(0));
			ASSERT_FLOAT_EQ(i32a_p2[3], static_cast<float>(0));
		}
		{
			constexpr auto	f32_vec1 = vector_load(f32_one_tier1);
			constexpr auto* f32_p1	 = f32_vec1.m128_f32;

			static_assert(float_eq(f32_p1[0], f1));
			static_assert(float_eq(f32_p1[1], f2));
			static_assert(float_eq(f32_p1[2], 0));
			static_assert(float_eq(f32_p1[3], 0));

			auto  f32_vec2 = vector_load(f32_one_tier2);
			auto* f32_p2   = f32_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32_p2[0], f1);
			ASSERT_FLOAT_EQ(f32_p2[1], f2);
			ASSERT_FLOAT_EQ(f32_p2[2], 0);
			ASSERT_FLOAT_EQ(f32_p2[3], 0);

			constexpr auto	f32a_vec1 = vector_load(f32a_one_tier1);
			constexpr auto* f32a_p1	  = f32a_vec1.m128_f32;

			static_assert(float_eq(f32a_p1[0], f1));
			static_assert(float_eq(f32a_p1[1], f2));
			static_assert(float_eq(f32a_p1[2], 0));
			static_assert(float_eq(f32a_p1[3], 0));

			auto  f32a_vec2 = vector_load(f32a_one_tier2);
			auto* f32a_p2	= f32a_vec2.m128_f32;

			ASSERT_FLOAT_EQ(f32a_p2[0], f1);
			ASSERT_FLOAT_EQ(f32a_p2[1], f2);
			ASSERT_FLOAT_EQ(f32a_p2[2], 0);
			ASSERT_FLOAT_EQ(f32a_p2[3], 0);
		}
	}
}

TEST(TestMath, TestVectorLoad1)
{
	constexpr std::uint32_t u1 = 0xFF;

	constexpr float			f1 = 123.456f;

	constexpr std::uint32_t u32_arr1[]{u1};
	std::uint32_t			u32_arr2[]{u1};
	constexpr std::int32_t	i32_arr1[]{u1};
	std::int32_t			i32_arr2[]{u1};
	constexpr float			f32_arr1[]{f1};
	float					f32_arr2[]{f1};

	{
		constexpr auto	u32_vec1 = vector_load<1>(u32_arr1);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u1);
		static_assert(u32_p1[1] == 0);
		static_assert(u32_p1[2] == 0);
		static_assert(u32_p1[3] == 0);

		auto  u32_vec2 = vector_load<1>(u32_arr2);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], static_cast<std::uint32_t>(u1));
		ASSERT_EQ(u32_p2[1], static_cast<std::uint32_t>(0));
		ASSERT_EQ(u32_p2[2], static_cast<std::uint32_t>(0));
		ASSERT_EQ(u32_p2[3], static_cast<std::uint32_t>(0));

		constexpr auto	i32_vec1 = vector_load<1>(i32_arr1);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == u1);
		static_assert(i32_p1[1] == 0);
		static_assert(i32_p1[2] == 0);
		static_assert(i32_p1[3] == 0);

		auto  i32_vec2 = vector_load<1>(i32_arr2);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], static_cast<std::int32_t>(u1));
		ASSERT_EQ(i32_p2[1], static_cast<std::int32_t>(0));
		ASSERT_EQ(i32_p2[2], static_cast<std::int32_t>(0));
		ASSERT_EQ(i32_p2[3], static_cast<std::int32_t>(0));

		constexpr auto	f32_vec1 = vector_load<1>(f32_arr1);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f1));
		static_assert(float_eq(f32_p1[1], 0));
		static_assert(float_eq(f32_p1[2], 0));
		static_assert(float_eq(f32_p1[3], 0));

		auto  f32_vec2 = vector_load<1>(f32_arr2);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2[1], 0);
		ASSERT_FLOAT_EQ(f32_p2[2], 0);
		ASSERT_FLOAT_EQ(f32_p2[3], 0);
	}
}

#endif
