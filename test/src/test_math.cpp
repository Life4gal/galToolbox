#include <gtest/gtest.h>

#include <array>
#include <math/math.hpp>
#include <numeric>

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

	constexpr auto			u32_one_tier1  = make<uint4>(u1, u2, u3, u4);
	auto					u32_one_tier2  = make<uint4>(u1, u2, u3, u4);
	constexpr auto			u32a_one_tier1 = make<uint4a>(u1, u2, u3, u4);
	auto					u32a_one_tier2 = make<uint4a>(u1, u2, u3, u4);

	constexpr auto			i32_one_tier1  = make<int4>(u1, u2, u3, u4);
	auto					i32_one_tier2  = make<int4>(u1, u2, u3, u4);
	constexpr auto			i32a_one_tier1 = make<int4a>(u1, u2, u3, u4);
	auto					i32a_one_tier2 = make<int4a>(u1, u2, u3, u4);

	constexpr auto			f32_one_tier1  = make<float4>(f1, f2, f3, f4);
	auto					f32_one_tier2  = make<float4>(f1, f2, f3, f4);
	constexpr auto			f32a_one_tier1 = make<float4a>(f1, f2, f3, f4);
	auto					f32a_one_tier2 = make<float4a>(f1, f2, f3, f4);

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

	constexpr auto			u32_one_tier1  = make<uint3>(u1, u2, u3);
	auto					u32_one_tier2  = make<uint3>(u1, u2, u3);
	constexpr auto			u32a_one_tier1 = make<uint3a>(u1, u2, u3);
	auto					u32a_one_tier2 = make<uint3a>(u1, u2, u3);

	constexpr auto			i32_one_tier1  = make<int3>(u1, u2, u3);
	auto					i32_one_tier2  = make<int3>(u1, u2, u3);
	constexpr auto			i32a_one_tier1 = make<int3a>(u1, u2, u3);
	auto					i32a_one_tier2 = make<int3a>(u1, u2, u3);

	constexpr auto			f32_one_tier1  = make<float3>(f1, f2, f3);
	auto					f32_one_tier2  = make<float3>(f1, f2, f3);
	constexpr auto			f32a_one_tier1 = make<float3a>(f1, f2, f3);
	auto					f32a_one_tier2 = make<float3a>(f1, f2, f3);

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

	constexpr auto			u32_one_tier1  = make<uint2>(u1, u2);
	auto					u32_one_tier2  = make<uint2>(u1, u2);
	constexpr auto			u32a_one_tier1 = make<uint2a>(u1, u2);
	auto					u32a_one_tier2 = make<uint2a>(u1, u2);

	constexpr auto			i32_one_tier1  = make<int2>(u1, u2);
	auto					i32_one_tier2  = make<int2>(u1, u2);
	constexpr auto			i32a_one_tier1 = make<int2a>(u1, u2);
	auto					i32a_one_tier2 = make<int2a>(u1, u2);

	constexpr auto			f32_one_tier1  = make<float2>(f1, f2);
	auto					f32_one_tier2  = make<float2>(f1, f2);
	constexpr auto			f32a_one_tier1 = make<float2a>(f1, f2);
	auto					f32a_one_tier2 = make<float2a>(f1, f2);

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

TEST(TestMath, TestMatrixLoad4x4)
{
	constexpr float f1			   = 123.456f;
	constexpr float f2			   = 234.567f;
	constexpr float f3			   = 345.678f;
	constexpr float f4			   = 456.789f;

	constexpr auto	f32_two_tier1  = make<float4x4>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4, f4, f3, f2, f1);
	auto			f32_two_tier2  = make<float4x4>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4, f4, f3, f2, f1);

	constexpr auto	f32a_two_tier1 = make<float4x4a>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4, f4, f3, f2, f1);
	auto			f32a_two_tier2 = make<float4x4a>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4, f4, f3, f2, f1);

	{
		constexpr auto	f32_matrix1 = matrix_load(f32_two_tier1);
		constexpr auto* f32_p1_0	= f32_matrix1.data[0].m128_f32;
		constexpr auto* f32_p1_1	= f32_matrix1.data[1].m128_f32;
		constexpr auto* f32_p1_2	= f32_matrix1.data[2].m128_f32;
		constexpr auto* f32_p1_3	= f32_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32_p1_0[0], f1));
		static_assert(float_eq(f32_p1_0[1], f2));
		static_assert(float_eq(f32_p1_0[2], f3));
		static_assert(float_eq(f32_p1_0[3], f4));

		static_assert(float_eq(f32_p1_1[0], f4));
		static_assert(float_eq(f32_p1_1[1], f3));
		static_assert(float_eq(f32_p1_1[2], f2));
		static_assert(float_eq(f32_p1_1[3], f1));

		static_assert(float_eq(f32_p1_2[0], f1));
		static_assert(float_eq(f32_p1_2[1], f2));
		static_assert(float_eq(f32_p1_2[2], f3));
		static_assert(float_eq(f32_p1_2[3], f4));

		static_assert(float_eq(f32_p1_3[0], f4));
		static_assert(float_eq(f32_p1_3[1], f3));
		static_assert(float_eq(f32_p1_3[2], f2));
		static_assert(float_eq(f32_p1_3[3], f1));

		auto  f32_matrix2 = matrix_load(f32_two_tier2);
		auto* f32_p2_0	  = f32_matrix2.data[0].m128_f32;
		auto* f32_p2_1	  = f32_matrix2.data[1].m128_f32;
		auto* f32_p2_2	  = f32_matrix2.data[2].m128_f32;
		auto* f32_p2_3	  = f32_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32_p2_0[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_0[2], f3);
		ASSERT_FLOAT_EQ(f32_p2_0[3], f4);

		ASSERT_FLOAT_EQ(f32_p2_1[0], f4);
		ASSERT_FLOAT_EQ(f32_p2_1[1], f3);
		ASSERT_FLOAT_EQ(f32_p2_1[2], f2);
		ASSERT_FLOAT_EQ(f32_p2_1[3], f1);

		ASSERT_FLOAT_EQ(f32_p2_2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32_p2_2[3], f4);

		ASSERT_FLOAT_EQ(f32_p2_3[0], f4);
		ASSERT_FLOAT_EQ(f32_p2_3[1], f3);
		ASSERT_FLOAT_EQ(f32_p2_3[2], f2);
		ASSERT_FLOAT_EQ(f32_p2_3[3], f1);
	}
	{
		constexpr auto	f32a_matrix1 = matrix_load(f32a_two_tier1);
		constexpr auto* f32a_p1_0	 = f32a_matrix1.data[0].m128_f32;
		constexpr auto* f32a_p1_1	 = f32a_matrix1.data[1].m128_f32;
		constexpr auto* f32a_p1_2	 = f32a_matrix1.data[2].m128_f32;
		constexpr auto* f32a_p1_3	 = f32a_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32a_p1_0[0], f1));
		static_assert(float_eq(f32a_p1_0[1], f2));
		static_assert(float_eq(f32a_p1_0[2], f3));
		static_assert(float_eq(f32a_p1_0[3], f4));

		static_assert(float_eq(f32a_p1_1[0], f4));
		static_assert(float_eq(f32a_p1_1[1], f3));
		static_assert(float_eq(f32a_p1_1[2], f2));
		static_assert(float_eq(f32a_p1_1[3], f1));

		static_assert(float_eq(f32a_p1_2[0], f1));
		static_assert(float_eq(f32a_p1_2[1], f2));
		static_assert(float_eq(f32a_p1_2[2], f3));
		static_assert(float_eq(f32a_p1_2[3], f4));

		static_assert(float_eq(f32a_p1_3[0], f4));
		static_assert(float_eq(f32a_p1_3[1], f3));
		static_assert(float_eq(f32a_p1_3[2], f2));
		static_assert(float_eq(f32a_p1_3[3], f1));

		auto  f32a_matrix2 = matrix_load(f32a_two_tier2);
		auto* f32a_p2_0	   = f32a_matrix2.data[0].m128_f32;
		auto* f32a_p2_1	   = f32a_matrix2.data[1].m128_f32;
		auto* f32a_p2_2	   = f32a_matrix2.data[2].m128_f32;
		auto* f32a_p2_3	   = f32a_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32a_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_0[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_0[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_0[3], f4);

		ASSERT_FLOAT_EQ(f32a_p2_1[0], f4);
		ASSERT_FLOAT_EQ(f32a_p2_1[1], f3);
		ASSERT_FLOAT_EQ(f32a_p2_1[2], f2);
		ASSERT_FLOAT_EQ(f32a_p2_1[3], f1);

		ASSERT_FLOAT_EQ(f32a_p2_2[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_2[3], f4);

		ASSERT_FLOAT_EQ(f32a_p2_3[0], f4);
		ASSERT_FLOAT_EQ(f32a_p2_3[1], f3);
		ASSERT_FLOAT_EQ(f32a_p2_3[2], f2);
		ASSERT_FLOAT_EQ(f32a_p2_3[3], f1);
	}
}

TEST(TestMath, TestMatrixLoad4x3)
{
	constexpr float f1			   = 123.456f;
	constexpr float f2			   = 234.567f;
	constexpr float f3			   = 345.678f;

	constexpr auto	f32_two_tier1  = make<float4x3>(f1, f2, f3, f3, f2, f1, f1, f2, f3, f3, f2, f1);
	auto			f32_two_tier2  = make<float4x3>(f1, f2, f3, f3, f2, f1, f1, f2, f3, f3, f2, f1);

	constexpr auto	f32a_two_tier1 = make<float4x3a>(f1, f2, f3, f3, f2, f1, f1, f2, f3, f3, f2, f1);
	auto			f32a_two_tier2 = make<float4x3a>(f1, f2, f3, f3, f2, f1, f1, f2, f3, f3, f2, f1);

	{
		constexpr auto	f32_matrix1 = matrix_load(f32_two_tier1);
		constexpr auto* f32_p1_0	= f32_matrix1.data[0].m128_f32;
		constexpr auto* f32_p1_1	= f32_matrix1.data[1].m128_f32;
		constexpr auto* f32_p1_2	= f32_matrix1.data[2].m128_f32;
		constexpr auto* f32_p1_3	= f32_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32_p1_0[0], f1));
		static_assert(float_eq(f32_p1_0[1], f2));
		static_assert(float_eq(f32_p1_0[2], f3));
		static_assert(float_eq(f32_p1_0[3], 0.f));

		static_assert(float_eq(f32_p1_1[0], f3));
		static_assert(float_eq(f32_p1_1[1], f2));
		static_assert(float_eq(f32_p1_1[2], f1));
		static_assert(float_eq(f32_p1_1[3], 0.f));

		static_assert(float_eq(f32_p1_2[0], f1));
		static_assert(float_eq(f32_p1_2[1], f2));
		static_assert(float_eq(f32_p1_2[2], f3));
		static_assert(float_eq(f32_p1_2[3], 0.f));

		static_assert(float_eq(f32_p1_3[0], f3));
		static_assert(float_eq(f32_p1_3[1], f2));
		static_assert(float_eq(f32_p1_3[2], f1));
		static_assert(float_eq(f32_p1_3[3], 1.f));

		auto  f32_matrix2 = matrix_load(f32_two_tier2);
		auto* f32_p2_0	  = f32_matrix2.data[0].m128_f32;
		auto* f32_p2_1	  = f32_matrix2.data[1].m128_f32;
		auto* f32_p2_2	  = f32_matrix2.data[2].m128_f32;
		auto* f32_p2_3	  = f32_matrix2.data[3].m128_f32;

		EXPECT_FLOAT_EQ(f32_p2_0[0], f1);
		EXPECT_FLOAT_EQ(f32_p2_0[1], f2);
		EXPECT_FLOAT_EQ(f32_p2_0[2], f3);
		EXPECT_FLOAT_EQ(f32_p2_0[3], 0.f);

		EXPECT_FLOAT_EQ(f32_p2_1[0], f3);
		EXPECT_FLOAT_EQ(f32_p2_1[1], f2);
		EXPECT_FLOAT_EQ(f32_p2_1[2], f1);
		EXPECT_FLOAT_EQ(f32_p2_1[3], 0.f);

		EXPECT_FLOAT_EQ(f32_p2_2[0], f1);
		EXPECT_FLOAT_EQ(f32_p2_2[1], f2);
		EXPECT_FLOAT_EQ(f32_p2_2[2], f3);
		EXPECT_FLOAT_EQ(f32_p2_2[3], 0.f);

		EXPECT_FLOAT_EQ(f32_p2_3[0], f3);
		EXPECT_FLOAT_EQ(f32_p2_3[1], f2);
		EXPECT_FLOAT_EQ(f32_p2_3[2], f1);
		EXPECT_FLOAT_EQ(f32_p2_3[3], 1.f);
	}
	{
		constexpr auto	f32a_matrix1 = matrix_load(f32a_two_tier1);
		constexpr auto* f32a_p1_0	 = f32a_matrix1.data[0].m128_f32;
		constexpr auto* f32a_p1_1	 = f32a_matrix1.data[1].m128_f32;
		constexpr auto* f32a_p1_2	 = f32a_matrix1.data[2].m128_f32;
		constexpr auto* f32a_p1_3	 = f32a_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32a_p1_0[0], f1));
		static_assert(float_eq(f32a_p1_0[1], f2));
		static_assert(float_eq(f32a_p1_0[2], f3));
		static_assert(float_eq(f32a_p1_0[3], 0.f));

		static_assert(float_eq(f32a_p1_1[0], f3));
		static_assert(float_eq(f32a_p1_1[1], f2));
		static_assert(float_eq(f32a_p1_1[2], f1));
		static_assert(float_eq(f32a_p1_1[3], 0.f));

		static_assert(float_eq(f32a_p1_2[0], f1));
		static_assert(float_eq(f32a_p1_2[1], f2));
		static_assert(float_eq(f32a_p1_2[2], f3));
		static_assert(float_eq(f32a_p1_2[3], 0.f));

		static_assert(float_eq(f32a_p1_3[0], f3));
		static_assert(float_eq(f32a_p1_3[1], f2));
		static_assert(float_eq(f32a_p1_3[2], f1));
		static_assert(float_eq(f32a_p1_3[3], 1.f));

		auto  f32a_matrix2 = matrix_load(f32a_two_tier2);
		auto* f32a_p2_0	   = f32a_matrix2.data[0].m128_f32;
		auto* f32a_p2_1	   = f32a_matrix2.data[1].m128_f32;
		auto* f32a_p2_2	   = f32a_matrix2.data[2].m128_f32;
		auto* f32a_p2_3	   = f32a_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32a_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_0[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_0[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_0[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_1[0], f3);
		ASSERT_FLOAT_EQ(f32a_p2_1[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_1[2], f1);
		ASSERT_FLOAT_EQ(f32a_p2_1[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_2[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_2[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_3[0], f3);
		ASSERT_FLOAT_EQ(f32a_p2_3[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_3[2], f1);
		ASSERT_FLOAT_EQ(f32a_p2_3[3], 1.f);
	}
}

TEST(TestMath, TestMatrixLoad3x4)
{
	constexpr float f1			   = 123.456f;
	constexpr float f2			   = 234.567f;
	constexpr float f3			   = 345.678f;
	constexpr float f4			   = 456.789f;

	constexpr auto	f32_two_tier1  = make<float3x4>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4);
	auto			f32_two_tier2  = make<float3x4>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4);

	constexpr auto	f32a_two_tier1 = make<float3x4a>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4);
	auto			f32a_two_tier2 = make<float3x4a>(f1, f2, f3, f4, f4, f3, f2, f1, f1, f2, f3, f4);

	{
		constexpr auto	f32_matrix1 = matrix_load(f32_two_tier1);
		constexpr auto* f32_p1_0	= f32_matrix1.data[0].m128_f32;
		constexpr auto* f32_p1_1	= f32_matrix1.data[1].m128_f32;
		constexpr auto* f32_p1_2	= f32_matrix1.data[2].m128_f32;
		constexpr auto* f32_p1_3	= f32_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32_p1_0[0], f1));
		static_assert(float_eq(f32_p1_0[1], f2));
		static_assert(float_eq(f32_p1_0[2], f3));
		static_assert(float_eq(f32_p1_0[3], f4));

		static_assert(float_eq(f32_p1_1[0], f4));
		static_assert(float_eq(f32_p1_1[1], f3));
		static_assert(float_eq(f32_p1_1[2], f2));
		static_assert(float_eq(f32_p1_1[3], f1));

		static_assert(float_eq(f32_p1_2[0], f1));
		static_assert(float_eq(f32_p1_2[1], f2));
		static_assert(float_eq(f32_p1_2[2], f3));
		static_assert(float_eq(f32_p1_2[3], f4));

		static_assert(float_eq(f32_p1_3[0], 0.f));
		static_assert(float_eq(f32_p1_3[1], 0.f));
		static_assert(float_eq(f32_p1_3[2], 0.f));
		static_assert(float_eq(f32_p1_3[3], 1.f));

		// note that matrix need to be transposed before use if source type is float3x4
		auto  f32_matrix2 = matrix_load(f32_two_tier2);
		auto* f32_p2_0	  = f32_matrix2.data[0].m128_f32;
		auto* f32_p2_1	  = f32_matrix2.data[1].m128_f32;
		auto* f32_p2_2	  = f32_matrix2.data[2].m128_f32;
		auto* f32_p2_3	  = f32_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32_p2_1[0], f2);
		ASSERT_FLOAT_EQ(f32_p2_2[0], f3);
		ASSERT_FLOAT_EQ(f32_p2_3[0], f4);

		ASSERT_FLOAT_EQ(f32_p2_0[1], f4);
		ASSERT_FLOAT_EQ(f32_p2_1[1], f3);
		ASSERT_FLOAT_EQ(f32_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_3[1], f1);

		ASSERT_FLOAT_EQ(f32_p2_0[2], f1);
		ASSERT_FLOAT_EQ(f32_p2_1[2], f2);
		ASSERT_FLOAT_EQ(f32_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32_p2_3[2], f4);

		ASSERT_FLOAT_EQ(f32_p2_0[3], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_1[3], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_2[3], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_3[3], 1.f);
	}
	{
		constexpr auto	f32a_matrix1 = matrix_load(f32a_two_tier1);
		constexpr auto* f32a_p1_0	 = f32a_matrix1.data[0].m128_f32;
		constexpr auto* f32a_p1_1	 = f32a_matrix1.data[1].m128_f32;
		constexpr auto* f32a_p1_2	 = f32a_matrix1.data[2].m128_f32;
		constexpr auto* f32a_p1_3	 = f32a_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32a_p1_0[0], f1));
		static_assert(float_eq(f32a_p1_0[1], f2));
		static_assert(float_eq(f32a_p1_0[2], f3));
		static_assert(float_eq(f32a_p1_0[3], f4));

		static_assert(float_eq(f32a_p1_1[0], f4));
		static_assert(float_eq(f32a_p1_1[1], f3));
		static_assert(float_eq(f32a_p1_1[2], f2));
		static_assert(float_eq(f32a_p1_1[3], f1));

		static_assert(float_eq(f32a_p1_2[0], f1));
		static_assert(float_eq(f32a_p1_2[1], f2));
		static_assert(float_eq(f32a_p1_2[2], f3));
		static_assert(float_eq(f32a_p1_2[3], f4));

		static_assert(float_eq(f32a_p1_3[0], 0.f));
		static_assert(float_eq(f32a_p1_3[1], 0.f));
		static_assert(float_eq(f32a_p1_3[2], 0.f));
		static_assert(float_eq(f32a_p1_3[3], 1.f));

		// note that matrix need to be transposed before use if source type is float3x4
		auto  f32a_matrix2 = matrix_load(f32a_two_tier2);
		auto* f32a_p2_0	   = f32a_matrix2.data[0].m128_f32;
		auto* f32a_p2_1	   = f32a_matrix2.data[1].m128_f32;
		auto* f32a_p2_2	   = f32a_matrix2.data[2].m128_f32;
		auto* f32a_p2_3	   = f32a_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32a_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_1[0], f2);
		ASSERT_FLOAT_EQ(f32a_p2_2[0], f3);
		ASSERT_FLOAT_EQ(f32a_p2_3[0], f4);

		ASSERT_FLOAT_EQ(f32a_p2_0[1], f4);
		ASSERT_FLOAT_EQ(f32a_p2_1[1], f3);
		ASSERT_FLOAT_EQ(f32a_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_3[1], f1);

		ASSERT_FLOAT_EQ(f32a_p2_0[2], f1);
		ASSERT_FLOAT_EQ(f32a_p2_1[2], f2);
		ASSERT_FLOAT_EQ(f32a_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_3[2], f4);

		ASSERT_FLOAT_EQ(f32a_p2_0[3], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_1[3], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_2[3], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_3[3], 1.f);
	}
}

TEST(TestMath, TestMatrixLoad3x3)
{
	constexpr float f1			   = 123.456f;
	constexpr float f2			   = 234.567f;
	constexpr float f3			   = 345.678f;

	constexpr auto	f32_two_tier1  = make<float3x3>(f1, f2, f3, f3, f2, f1, f1, f2, f3);
	auto			f32_two_tier2  = make<float3x3>(f1, f2, f3, f3, f2, f1, f1, f2, f3);

	constexpr auto	f32a_two_tier1 = make<float3x3a>(f1, f2, f3, f3, f2, f1, f1, f2, f3);
	auto			f32a_two_tier2 = make<float3x3a>(f1, f2, f3, f3, f2, f1, f1, f2, f3);

	{
		constexpr auto	f32_matrix1 = matrix_load(f32_two_tier1);
		constexpr auto* f32_p1_0	= f32_matrix1.data[0].m128_f32;
		constexpr auto* f32_p1_1	= f32_matrix1.data[1].m128_f32;
		constexpr auto* f32_p1_2	= f32_matrix1.data[2].m128_f32;
		constexpr auto* f32_p1_3	= f32_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32_p1_0[0], f1));
		static_assert(float_eq(f32_p1_0[1], f2));
		static_assert(float_eq(f32_p1_0[2], f3));
		static_assert(float_eq(f32_p1_0[3], 0.f));

		static_assert(float_eq(f32_p1_1[0], f3));
		static_assert(float_eq(f32_p1_1[1], f2));
		static_assert(float_eq(f32_p1_1[2], f1));
		static_assert(float_eq(f32_p1_1[3], 0.f));

		static_assert(float_eq(f32_p1_2[0], f1));
		static_assert(float_eq(f32_p1_2[1], f2));
		static_assert(float_eq(f32_p1_2[2], f3));
		static_assert(float_eq(f32_p1_2[3], 0.f));

		static_assert(float_eq(f32_p1_3[0], 0.f));
		static_assert(float_eq(f32_p1_3[1], 0.f));
		static_assert(float_eq(f32_p1_3[2], 0.f));
		static_assert(float_eq(f32_p1_3[3], 1.f));

		auto  f32_matrix2 = matrix_load(f32_two_tier2);
		auto* f32_p2_0	  = f32_matrix2.data[0].m128_f32;
		auto* f32_p2_1	  = f32_matrix2.data[1].m128_f32;
		auto* f32_p2_2	  = f32_matrix2.data[2].m128_f32;
		auto* f32_p2_3	  = f32_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32_p2_0[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_0[2], f3);
		ASSERT_FLOAT_EQ(f32_p2_0[3], 0.f);

		ASSERT_FLOAT_EQ(f32_p2_1[0], f3);
		ASSERT_FLOAT_EQ(f32_p2_1[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_1[2], f1);
		ASSERT_FLOAT_EQ(f32_p2_1[3], 0.f);

		ASSERT_FLOAT_EQ(f32_p2_2[0], f1);
		ASSERT_FLOAT_EQ(f32_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32_p2_2[3], 0.f);

		ASSERT_FLOAT_EQ(f32_p2_3[0], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_3[1], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_3[2], 0.f);
		ASSERT_FLOAT_EQ(f32_p2_3[3], 1.f);
	}
	{
		constexpr auto	f32a_matrix1 = matrix_load(f32a_two_tier1);
		constexpr auto* f32a_p1_0	 = f32a_matrix1.data[0].m128_f32;
		constexpr auto* f32a_p1_1	 = f32a_matrix1.data[1].m128_f32;
		constexpr auto* f32a_p1_2	 = f32a_matrix1.data[2].m128_f32;
		constexpr auto* f32a_p1_3	 = f32a_matrix1.data[3].m128_f32;

		static_assert(float_eq(f32a_p1_0[0], f1));
		static_assert(float_eq(f32a_p1_0[1], f2));
		static_assert(float_eq(f32a_p1_0[2], f3));
		static_assert(float_eq(f32a_p1_0[3], 0.f));

		static_assert(float_eq(f32a_p1_1[0], f3));
		static_assert(float_eq(f32a_p1_1[1], f2));
		static_assert(float_eq(f32a_p1_1[2], f1));
		static_assert(float_eq(f32a_p1_1[3], 0.f));

		static_assert(float_eq(f32a_p1_2[0], f1));
		static_assert(float_eq(f32a_p1_2[1], f2));
		static_assert(float_eq(f32a_p1_2[2], f3));
		static_assert(float_eq(f32a_p1_2[3], 0.f));

		static_assert(float_eq(f32a_p1_3[0], 0.f));
		static_assert(float_eq(f32a_p1_3[1], 0.f));
		static_assert(float_eq(f32a_p1_3[2], 0.f));
		static_assert(float_eq(f32a_p1_3[3], 1.f));

		auto  f32a_matrix2 = matrix_load(f32a_two_tier2);
		auto* f32a_p2_0	   = f32a_matrix2.data[0].m128_f32;
		auto* f32a_p2_1	   = f32a_matrix2.data[1].m128_f32;
		auto* f32a_p2_2	   = f32a_matrix2.data[2].m128_f32;
		auto* f32a_p2_3	   = f32a_matrix2.data[3].m128_f32;

		ASSERT_FLOAT_EQ(f32a_p2_0[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_0[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_0[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_0[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_1[0], f3);
		ASSERT_FLOAT_EQ(f32a_p2_1[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_1[2], f1);
		ASSERT_FLOAT_EQ(f32a_p2_1[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_2[0], f1);
		ASSERT_FLOAT_EQ(f32a_p2_2[1], f2);
		ASSERT_FLOAT_EQ(f32a_p2_2[2], f3);
		ASSERT_FLOAT_EQ(f32a_p2_2[3], 0.f);

		ASSERT_FLOAT_EQ(f32a_p2_3[0], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_3[1], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_3[2], 0.f);
		ASSERT_FLOAT_EQ(f32a_p2_3[3], 1.f);
	}
}

TEST(TestMath, TestVectorStore4)
{
	// todo
}

TEST(TestMath, TestVectorStore3)
{
	// todo
}

TEST(TestMath, TestVectorStore2)
{
	// todo
}

TEST(TestMath, TestVectorStore1)
{
	// todo
}

TEST(TestMath, TestMatrixStore4x4)
{
	// todo
}

TEST(TestMath, TestMatrixStore4x3)
{
	// todo
}

TEST(TestMath, TestMatrixStore3x4)
{
	// todo
}

TEST(TestMath, TestMatrixStore3x3)
{
	// todo
}

TEST(TestMath, TestVectorZero)
{
	constexpr auto	zero1 = vector_zero();
	constexpr auto* p1	  = zero1.m128_f32;

	static_assert(float_eq(p1[0], 0.f));
	static_assert(float_eq(p1[1], 0.f));
	static_assert(float_eq(p1[2], 0.f));
	static_assert(float_eq(p1[3], 0.f));

	auto  zero2 = vector_zero();
	auto* p2	= zero2.m128_f32;

	ASSERT_FLOAT_EQ(p2[0], 0.f);
	ASSERT_FLOAT_EQ(p2[1], 0.f);
	ASSERT_FLOAT_EQ(p2[2], 0.f);
	ASSERT_FLOAT_EQ(p2[3], 0.f);
}

TEST(TestMath, TestVectorSet)
{
	constexpr std::uint32_t u32_1 = 0xFF;
	constexpr std::uint32_t u32_2 = 0xFF0;
	constexpr std::uint32_t u32_3 = 0xFF00;
	constexpr std::uint32_t u32_4 = 0xFF000;

	constexpr std::int32_t	i32_1 = 0xFF;
	constexpr std::int32_t	i32_2 = 0xFF0;
	constexpr std::int32_t	i32_3 = 0xFF00;
	constexpr std::int32_t	i32_4 = 0xFF000;

	constexpr float			f32_1 = 123.456f;
	constexpr float			f32_2 = 234.456f;
	constexpr float			f32_3 = 345.678f;
	constexpr float			f32_4 = 456.789f;

	{
		constexpr auto	u32_vec1 = vector_set(u32_1, u32_2, u32_3, u32_4);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u32_1);
		static_assert(u32_p1[1] == u32_2);
		static_assert(u32_p1[2] == u32_3);
		static_assert(u32_p1[3] == u32_4);

		auto  u32_vec2 = vector_set(u32_1, u32_2, u32_3, u32_4);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], u32_1);
		ASSERT_EQ(u32_p2[1], u32_2);
		ASSERT_EQ(u32_p2[2], u32_3);
		ASSERT_EQ(u32_p2[3], u32_4);
	}
	{
		constexpr auto	i32_vec1 = vector_set(i32_1, i32_2, i32_3, i32_4);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == i32_1);
		static_assert(i32_p1[1] == i32_2);
		static_assert(i32_p1[2] == i32_3);
		static_assert(i32_p1[3] == i32_4);

		auto  i32_vec2 = vector_set(i32_1, i32_2, i32_3, i32_4);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], i32_1);
		ASSERT_EQ(i32_p2[1], i32_2);
		ASSERT_EQ(i32_p2[2], i32_3);
		ASSERT_EQ(i32_p2[3], i32_4);
	}
	{
		constexpr auto	f32_vec1 = vector_set(f32_1, f32_2, f32_3, f32_4);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f32_1));
		static_assert(float_eq(f32_p1[1], f32_2));
		static_assert(float_eq(f32_p1[2], f32_3));
		static_assert(float_eq(f32_p1[3], f32_4));

		auto  f32_vec2 = vector_set(f32_1, f32_2, f32_3, f32_4);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f32_1);
		ASSERT_FLOAT_EQ(f32_p2[1], f32_2);
		ASSERT_FLOAT_EQ(f32_p2[2], f32_3);
		ASSERT_FLOAT_EQ(f32_p2[3], f32_4);
	}
}

TEST(TestMath, TestVectorReplicate)
{
	constexpr std::uint32_t u32 = 0xFF;

	constexpr std::int32_t	i32 = 0xFF;

	constexpr float			f32 = 123.456f;

	{
		constexpr auto	u32_vec1 = vector_replicate(u32);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u32);
		static_assert(u32_p1[1] == u32);
		static_assert(u32_p1[2] == u32);
		static_assert(u32_p1[3] == u32);

		auto  u32_vec2 = vector_replicate(u32);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], u32);
		ASSERT_EQ(u32_p2[1], u32);
		ASSERT_EQ(u32_p2[2], u32);
		ASSERT_EQ(u32_p2[3], u32);
	}
	{
		constexpr auto	i32_vec1 = vector_replicate(i32);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == i32);
		static_assert(i32_p1[1] == i32);
		static_assert(i32_p1[2] == i32);
		static_assert(i32_p1[3] == i32);

		auto  i32_vec2 = vector_replicate(i32);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], i32);
		ASSERT_EQ(i32_p2[1], i32);
		ASSERT_EQ(i32_p2[2], i32);
		ASSERT_EQ(i32_p2[3], i32);
	}
	{
		constexpr auto	f32_vec1 = vector_replicate(f32);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f32));
		static_assert(float_eq(f32_p1[1], f32));
		static_assert(float_eq(f32_p1[2], f32));
		static_assert(float_eq(f32_p1[3], f32));

		auto  f32_vec2 = vector_replicate(f32);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f32);
		ASSERT_FLOAT_EQ(f32_p2[1], f32);
		ASSERT_FLOAT_EQ(f32_p2[2], f32);
		ASSERT_FLOAT_EQ(f32_p2[3], f32);
	}
}

TEST(TestMath, TestVectorReplicatePtr)
{
	constexpr std::uint32_t u32[] = {0xFF, 0xFF0};

	constexpr std::int32_t	i32[] = {0xFF, 0xFF0};

	constexpr float			f32[] = {123.456f, 234.567f};

	{
		constexpr auto	u32_vec1 = vector_replicate(u32);
		constexpr auto* u32_p1	 = u32_vec1.m128_u32;

		static_assert(u32_p1[0] == u32[0]);
		static_assert(u32_p1[1] == u32[0]);
		static_assert(u32_p1[2] == u32[0]);
		static_assert(u32_p1[3] == u32[0]);

		auto  u32_vec2 = vector_replicate(u32);
		auto* u32_p2   = u32_vec2.m128_u32;

		ASSERT_EQ(u32_p2[0], u32[0]);
		ASSERT_EQ(u32_p2[1], u32[0]);
		ASSERT_EQ(u32_p2[2], u32[0]);
		ASSERT_EQ(u32_p2[3], u32[0]);
	}
	{
		constexpr auto	i32_vec1 = vector_replicate(i32);
		constexpr auto* i32_p1	 = i32_vec1.m128_i32;

		static_assert(i32_p1[0] == i32[0]);
		static_assert(i32_p1[1] == i32[0]);
		static_assert(i32_p1[2] == i32[0]);
		static_assert(i32_p1[3] == i32[0]);

		auto  i32_vec2 = vector_replicate(i32);
		auto* i32_p2   = i32_vec2.m128_i32;

		ASSERT_EQ(i32_p2[0], i32[0]);
		ASSERT_EQ(i32_p2[1], i32[0]);
		ASSERT_EQ(i32_p2[2], i32[0]);
		ASSERT_EQ(i32_p2[3], i32[0]);
	}
	{
		constexpr auto	f32_vec1 = vector_replicate(f32);
		constexpr auto* f32_p1	 = f32_vec1.m128_f32;

		static_assert(float_eq(f32_p1[0], f32[0]));
		static_assert(float_eq(f32_p1[1], f32[0]));
		static_assert(float_eq(f32_p1[2], f32[0]));
		static_assert(float_eq(f32_p1[3], f32[0]));

		auto  f32_vec2 = vector_replicate(f32);
		auto* f32_p2   = f32_vec2.m128_f32;

		ASSERT_FLOAT_EQ(f32_p2[0], f32[0]);
		ASSERT_FLOAT_EQ(f32_p2[1], f32[0]);
		ASSERT_FLOAT_EQ(f32_p2[2], f32[0]);
		ASSERT_FLOAT_EQ(f32_p2[3], f32[0]);
	}
}

TEST(TestMath, TestVectorReplicateTrueAndFalse)
{
	{
		constexpr auto	vec1 = vector_true();
		constexpr auto* p1	 = vec1.m128_u32;

		static_assert(static_cast<int>(p1[0]) == -1);
		static_assert(static_cast<int>(p1[1]) == -1);
		static_assert(static_cast<int>(p1[2]) == -1);
		static_assert(static_cast<int>(p1[3]) == -1);

		auto  vec2 = vector_true();
		auto* p2   = vec2.m128_i32;

		ASSERT_EQ(p2[0], -1);
		ASSERT_EQ(p2[1], -1);
		ASSERT_EQ(p2[2], -1);
		ASSERT_EQ(p2[3], -1);
	}
	{
		constexpr auto	vec1 = vector_false();
		constexpr auto* p1	 = vec1.m128_f32;

		static_assert(float_eq(p1[0], 0.f));
		static_assert(float_eq(p1[1], 0.f));
		static_assert(float_eq(p1[2], 0.f));
		static_assert(float_eq(p1[3], 0.f));

		auto  vec2 = vector_false();
		auto* p2   = vec2.m128_f32;

		ASSERT_FLOAT_EQ(p2[0], 0.f);
		ASSERT_FLOAT_EQ(p2[1], 0.f);
		ASSERT_FLOAT_EQ(p2[2], 0.f);
		ASSERT_FLOAT_EQ(p2[3], 0.f);
	}
}

TEST(TestMath, TestVectorSplat)
{
	constexpr auto f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	{
		constexpr auto	vec1_1 = vector_splat<0, float>(f32_vec);
		constexpr auto* p1_1   = vec1_1.m128_f32;

		static_assert(float_eq(p1_1[0], 123.f));
		static_assert(float_eq(p1_1[1], 123.f));
		static_assert(float_eq(p1_1[2], 123.f));
		static_assert(float_eq(p1_1[3], 123.f));

		auto  vec1_2 = vector_splat<0, float>(f32_vec);
		auto* p1_2	 = vec1_2.m128_f32;

		ASSERT_FLOAT_EQ(p1_2[0], 123.f);
		ASSERT_FLOAT_EQ(p1_2[1], 123.f);
		ASSERT_FLOAT_EQ(p1_2[2], 123.f);
		ASSERT_FLOAT_EQ(p1_2[3], 123.f);

		constexpr auto	vec2_1 = vector_splat<1, float>(f32_vec);
		constexpr auto* p2_1   = vec2_1.m128_f32;

		static_assert(float_eq(p2_1[0], 456.f));
		static_assert(float_eq(p2_1[1], 456.f));
		static_assert(float_eq(p2_1[2], 456.f));
		static_assert(float_eq(p2_1[3], 456.f));

		auto  vec2_2 = vector_splat<1, float>(f32_vec);
		auto* p2_2	 = vec2_2.m128_f32;

		ASSERT_FLOAT_EQ(p2_2[0], 456.f);
		ASSERT_FLOAT_EQ(p2_2[1], 456.f);
		ASSERT_FLOAT_EQ(p2_2[2], 456.f);
		ASSERT_FLOAT_EQ(p2_2[3], 456.f);

		constexpr auto	vec3_1 = vector_splat<2, float>(f32_vec);
		constexpr auto* p3_1   = vec3_1.m128_f32;

		static_assert(float_eq(p3_1[0], 789.f));
		static_assert(float_eq(p3_1[1], 789.f));
		static_assert(float_eq(p3_1[2], 789.f));
		static_assert(float_eq(p3_1[3], 789.f));

		auto  vec3_2 = vector_splat<2, float>(f32_vec);
		auto* p3_2	 = vec3_2.m128_f32;

		ASSERT_FLOAT_EQ(p3_2[0], 789.f);
		ASSERT_FLOAT_EQ(p3_2[1], 789.f);
		ASSERT_FLOAT_EQ(p3_2[2], 789.f);
		ASSERT_FLOAT_EQ(p3_2[3], 789.f);

		constexpr auto	vec4_1 = vector_splat<3, float>(f32_vec);
		constexpr auto* p4_1   = vec4_1.m128_f32;

		static_assert(float_eq(p4_1[0], 0.f));
		static_assert(float_eq(p4_1[1], 0.f));
		static_assert(float_eq(p4_1[2], 0.f));
		static_assert(float_eq(p4_1[3], 0.f));

		auto  vec4_2 = vector_splat<3, float>(f32_vec);
		auto* p4_2	 = vec4_2.m128_f32;

		ASSERT_FLOAT_EQ(p4_2[0], 0.f);
		ASSERT_FLOAT_EQ(p4_2[1], 0.f);
		ASSERT_FLOAT_EQ(p4_2[2], 0.f);
		ASSERT_FLOAT_EQ(p4_2[3], 0.f);
	}
	{
		constexpr auto	vec1_1 = vector_splat<0, std::int32_t>(i32_vec);
		constexpr auto* p1_1   = vec1_1.m128_i32;

		static_assert(p1_1[0] == 123);
		static_assert(p1_1[1] == 123);
		static_assert(p1_1[2] == 123);
		static_assert(p1_1[3] == 123);

		auto  vec1_2 = vector_splat<0, std::int32_t>(i32_vec);
		auto* p1_2	 = vec1_2.m128_i32;

		ASSERT_EQ(p1_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p1_2[1], static_cast<std::int32_t>(123));
		ASSERT_EQ(p1_2[2], static_cast<std::int32_t>(123));
		ASSERT_EQ(p1_2[3], static_cast<std::int32_t>(123));

		constexpr auto	vec2_1 = vector_splat<1, std::int32_t>(i32_vec);
		constexpr auto* p2_1   = vec2_1.m128_i32;

		static_assert(p2_1[0] == 456);
		static_assert(p2_1[1] == 456);
		static_assert(p2_1[2] == 456);
		static_assert(p2_1[3] == 456);

		auto  vec2_2 = vector_splat<1, std::int32_t>(i32_vec);
		auto* p2_2	 = vec2_2.m128_i32;

		ASSERT_EQ(p2_2[0], static_cast<std::int32_t>(456));
		ASSERT_EQ(p2_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p2_2[2], static_cast<std::int32_t>(456));
		ASSERT_EQ(p2_2[3], static_cast<std::int32_t>(456));

		constexpr auto	vec3_1 = vector_splat<2, std::int32_t>(i32_vec);
		constexpr auto* p3_1   = vec3_1.m128_i32;

		static_assert(p3_1[0] == 789);
		static_assert(p3_1[1] == 789);
		static_assert(p3_1[2] == 789);
		static_assert(p3_1[3] == 789);

		auto  vec3_2 = vector_splat<2, std::int32_t>(i32_vec);
		auto* p3_2	 = vec3_2.m128_i32;

		ASSERT_EQ(p3_2[0], static_cast<std::int32_t>(789));
		ASSERT_EQ(p3_2[1], static_cast<std::int32_t>(789));
		ASSERT_EQ(p3_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p3_2[3], static_cast<std::int32_t>(789));

		constexpr auto	vec4_1 = vector_splat<3, std::int32_t>(i32_vec);
		constexpr auto* p4_1   = vec4_1.m128_i32;

		static_assert(p4_1[0] == 0);
		static_assert(p4_1[1] == 0);
		static_assert(p4_1[2] == 0);
		static_assert(p4_1[3] == 0);

		auto  vec4_2 = vector_splat<3, std::int32_t>(i32_vec);
		auto* p4_2	 = vec4_2.m128_i32;

		ASSERT_EQ(p4_2[0], static_cast<std::int32_t>(0));
		ASSERT_EQ(p4_2[1], static_cast<std::int32_t>(0));
		ASSERT_EQ(p4_2[2], static_cast<std::int32_t>(0));
		ASSERT_EQ(p4_2[3], static_cast<std::int32_t>(0));
	}
	{
		constexpr auto	vec1_1 = vector_splat<0, std::uint32_t>(u32_vec);
		constexpr auto* p1_1   = vec1_1.m128_u32;

		static_assert(p1_1[0] == 123);
		static_assert(p1_1[1] == 123);
		static_assert(p1_1[2] == 123);
		static_assert(p1_1[3] == 123);

		auto  vec1_2 = vector_splat<0, std::uint32_t>(u32_vec);
		auto* p1_2	 = vec1_2.m128_u32;

		ASSERT_EQ(p1_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p1_2[1], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p1_2[2], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p1_2[3], static_cast<std::uint32_t>(123));

		constexpr auto	vec2_1 = vector_splat<1, std::uint32_t>(u32_vec);
		constexpr auto* p2_1   = vec2_1.m128_u32;

		static_assert(p2_1[0] == 456);
		static_assert(p2_1[1] == 456);
		static_assert(p2_1[2] == 456);
		static_assert(p2_1[3] == 456);

		auto  vec2_2 = vector_splat<1, std::uint32_t>(u32_vec);
		auto* p2_2	 = vec2_2.m128_u32;

		ASSERT_EQ(p2_2[0], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p2_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p2_2[2], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p2_2[3], static_cast<std::uint32_t>(456));

		constexpr auto	vec3_1 = vector_splat<2, std::uint32_t>(u32_vec);
		constexpr auto* p3_1   = vec3_1.m128_u32;

		static_assert(p3_1[0] == 789);
		static_assert(p3_1[1] == 789);
		static_assert(p3_1[2] == 789);
		static_assert(p3_1[3] == 789);

		auto  vec3_2 = vector_splat<2, std::uint32_t>(u32_vec);
		auto* p3_2	 = vec3_2.m128_u32;

		ASSERT_EQ(p3_2[0], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p3_2[1], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p3_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p3_2[3], static_cast<std::uint32_t>(789));

		constexpr auto	vec4_1 = vector_splat<3, std::uint32_t>(u32_vec);
		constexpr auto* p4_1   = vec4_1.m128_u32;

		static_assert(p4_1[0] == 0);
		static_assert(p4_1[1] == 0);
		static_assert(p4_1[2] == 0);
		static_assert(p4_1[3] == 0);

		auto  vec4_2 = vector_splat<3, std::uint32_t>(u32_vec);
		auto* p4_2	 = vec4_2.m128_u32;

		ASSERT_EQ(p4_2[0], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p4_2[1], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p4_2[2], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p4_2[3], static_cast<std::uint32_t>(0));
	}
}

TEST(TestMath, TestVectorSplatOne)
{
	constexpr auto	vec1 = vector_splat_one();
	constexpr auto* p1	 = vec1.m128_f32;

	static_assert(float_eq(p1[0], 1.f));
	static_assert(float_eq(p1[1], 1.f));
	static_assert(float_eq(p1[2], 1.f));
	static_assert(float_eq(p1[3], 1.f));

	auto  vec2 = vector_splat_one();
	auto* p2   = vec2.m128_f32;

	ASSERT_FLOAT_EQ(p2[0], 1.f);
	ASSERT_FLOAT_EQ(p2[1], 1.f);
	ASSERT_FLOAT_EQ(p2[2], 1.f);
	ASSERT_FLOAT_EQ(p2[3], 1.f);
}

TEST(TestMath, TestVectorSplatInfinity)
{
	constexpr auto	vec1 = vector_splat_infinity();
	constexpr auto* p1	 = vec1.m128_i32;

	static_assert(p1[0] == 0x7F800000);
	static_assert(p1[1] == 0x7F800000);
	static_assert(p1[2] == 0x7F800000);
	static_assert(p1[3] == 0x7F800000);

	auto  vec2 = vector_splat_infinity();
	auto* p2   = vec2.m128_f32;

	ASSERT_FLOAT_EQ(p2[0], std::numeric_limits<float>::infinity());
	ASSERT_FLOAT_EQ(p2[1], std::numeric_limits<float>::infinity());
	ASSERT_FLOAT_EQ(p2[2], std::numeric_limits<float>::infinity());
	ASSERT_FLOAT_EQ(p2[3], std::numeric_limits<float>::infinity());

	ASSERT_TRUE(std::isinf(p2[0]));
	ASSERT_TRUE(std::isinf(p2[1]));
	ASSERT_TRUE(std::isinf(p2[2]));
	ASSERT_TRUE(std::isinf(p2[3]));
}

TEST(TestMath, TestVectorSplatQnan)
{
	constexpr auto	vec1 = vector_splat_qnan();
	constexpr auto* p1	 = vec1.m128_i32;

	static_assert(p1[0] == 0x7FC00000);
	static_assert(p1[1] == 0x7FC00000);
	static_assert(p1[2] == 0x7FC00000);
	static_assert(p1[3] == 0x7FC00000);

	auto  vec2 = vector_splat_qnan();
	auto* p2   = vec2.m128_f32;

	//	ASSERT_FLOAT_EQ(p2[0], std::numeric_limits<float>::quiet_NaN());
	//	ASSERT_FLOAT_EQ(p2[1], std::numeric_limits<float>::quiet_NaN());
	//	ASSERT_FLOAT_EQ(p2[2], std::numeric_limits<float>::quiet_NaN());
	//	ASSERT_FLOAT_EQ(p2[3], std::numeric_limits<float>::quiet_NaN());
	// bad trick
	ASSERT_NE(p2[0], p2[0]);
	ASSERT_NE(p2[1], p2[1]);
	ASSERT_NE(p2[2], p2[2]);
	ASSERT_NE(p2[3], p2[3]);

	ASSERT_TRUE(std::isnan(p2[0]));
	ASSERT_TRUE(std::isnan(p2[1]));
	ASSERT_TRUE(std::isnan(p2[2]));
	ASSERT_TRUE(std::isnan(p2[3]));
}

TEST(TestMath, TestVectorSplatEpsilon)
{
	constexpr auto	vec1 = vector_splat_epsilon();
	constexpr auto* p1	 = vec1.m128_f32;

	static_assert(float_eq(p1[0], FLT_EPSILON));
	static_assert(float_eq(p1[1], FLT_EPSILON));
	static_assert(float_eq(p1[2], FLT_EPSILON));
	static_assert(float_eq(p1[3], FLT_EPSILON));

	auto  vec2 = vector_splat_epsilon();
	auto* p2   = vec2.m128_f32;

	ASSERT_FLOAT_EQ(p2[0], FLT_EPSILON);
	ASSERT_FLOAT_EQ(p2[1], FLT_EPSILON);
	ASSERT_FLOAT_EQ(p2[2], FLT_EPSILON);
	ASSERT_FLOAT_EQ(p2[3], FLT_EPSILON);
}

TEST(TestMath, TestVectorSplatSignMask)
{
	constexpr auto	vec1 = vector_splat_sign_mask();
	constexpr auto* p1	 = vec1.m128_u32;

	static_assert(p1[0] == 0x80000000);
	static_assert(p1[1] == 0x80000000);
	static_assert(p1[2] == 0x80000000);
	static_assert(p1[3] == 0x80000000);

	auto  vec2 = vector_splat_sign_mask();
	auto* p2   = vec2.m128_f32;

	ASSERT_FLOAT_EQ(p2[0], -0.f);
	ASSERT_FLOAT_EQ(p2[1], -0.f);
	ASSERT_FLOAT_EQ(p2[2], -0.f);
	ASSERT_FLOAT_EQ(p2[3], -0.f);
}

TEST(TestMath, TestVectorGet)
{
	constexpr auto f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	{
		constexpr auto v1_1 = vector_get<0, float>(f32_vec);
		static_assert(float_eq(v1_1, 123.f));

		auto v1_2 = vector_get<0, float>(f32_vec);
		ASSERT_FLOAT_EQ(v1_2, 123.f);

		constexpr auto v2_1 = vector_get<1, float>(f32_vec);
		static_assert(float_eq(v2_1, 456.f));

		auto v2_2 = vector_get<1, float>(f32_vec);
		ASSERT_FLOAT_EQ(v2_2, 456.f);

		constexpr auto v3_1 = vector_get<2, float>(f32_vec);
		static_assert(float_eq(v3_1, 789.f));

		auto v3_2 = vector_get<2, float>(f32_vec);
		ASSERT_FLOAT_EQ(v3_2, 789.f);

		constexpr auto v4_1 = vector_get<3, float>(f32_vec);
		static_assert(float_eq(v4_1, 0.f));

		auto v4_2 = vector_get<3, float>(f32_vec);
		ASSERT_FLOAT_EQ(v4_2, 0.f);
	}
	{
		constexpr auto v1_1 = vector_get<0, std::int32_t>(i32_vec);
		static_assert(v1_1 == 123);

		auto v1_2 = vector_get<0, std::int32_t>(i32_vec);
		ASSERT_EQ(v1_2, static_cast<std::int32_t>(123));

		constexpr auto v2_1 = vector_get<1, std::int32_t>(i32_vec);
		static_assert(v2_1 == 456);

		auto v2_2 = vector_get<1, std::int32_t>(i32_vec);
		ASSERT_EQ(v2_2, static_cast<std::int32_t>(456));

		constexpr auto v3_1 = vector_get<2, std::int32_t>(i32_vec);
		static_assert(v3_1 == 789);

		auto v3_2 = vector_get<2, std::int32_t>(i32_vec);
		ASSERT_EQ(v3_2, static_cast<std::int32_t>(789));

		constexpr auto v4_1 = vector_get<3, std::int32_t>(i32_vec);
		static_assert(v4_1 == 0);

		auto v4_2 = vector_get<3, std::int32_t>(i32_vec);
		ASSERT_EQ(v4_2, static_cast<std::int32_t>(0));
	}
	{
		constexpr auto v1_1 = vector_get<0, std::uint32_t>(u32_vec);
		static_assert(v1_1 == 123);

		auto v1_2 = vector_get<0, std::uint32_t>(u32_vec);
		ASSERT_EQ(v1_2, static_cast<std::uint32_t>(123));

		constexpr auto v2_1 = vector_get<1, std::uint32_t>(u32_vec);
		static_assert(v2_1 == 456);

		auto v2_2 = vector_get<1, std::uint32_t>(u32_vec);
		ASSERT_EQ(v2_2, static_cast<std::uint32_t>(456));

		constexpr auto v3_1 = vector_get<2, std::uint32_t>(u32_vec);
		static_assert(v3_1 == 789);

		auto v3_2 = vector_get<2, std::uint32_t>(u32_vec);
		ASSERT_EQ(v3_2, static_cast<std::uint32_t>(789));

		constexpr auto v4_1 = vector_get<3, std::uint32_t>(u32_vec);
		static_assert(v4_1 == 0);

		auto v4_2 = vector_get<3, std::uint32_t>(u32_vec);
		ASSERT_EQ(v4_2, static_cast<std::uint32_t>(0));
	}
}

TEST(TestMath, TestVectorGetPtr)
{
	constexpr auto f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	{
		float v1;
		vector_get<0, float>(&v1, f32_vec);
		EXPECT_FLOAT_EQ(v1, 123.f);

		float v2;
		vector_get<1, float>(&v2, f32_vec);
		EXPECT_FLOAT_EQ(v2, 456.f);

		float v3;
		vector_get<2, float>(&v3, f32_vec);
		EXPECT_FLOAT_EQ(v3, 789.f);

		float v4;
		vector_get<3, float>(&v4, f32_vec);
		EXPECT_FLOAT_EQ(v4, 0.f);
	}
	{
		std::int32_t v1;
		vector_get<0, std::int32_t>(&v1, i32_vec);
		ASSERT_EQ(v1, static_cast<std::int32_t>(123));

		std::int32_t v2;
		vector_get<1, std::int32_t>(&v2, i32_vec);
		ASSERT_EQ(v2, static_cast<std::int32_t>(456));

		std::int32_t v3;
		vector_get<2, std::int32_t>(&v3, i32_vec);
		ASSERT_EQ(v3, static_cast<std::int32_t>(789));

		std::int32_t v4;
		vector_get<3, std::int32_t>(&v4, i32_vec);
		ASSERT_EQ(v4, static_cast<std::int32_t>(0));
	}
	{
		std::uint32_t v1;
		vector_get<0, std::uint32_t>(&v1, u32_vec);
		ASSERT_EQ(v1, static_cast<std::uint32_t>(123));

		std::uint32_t v2;
		vector_get<1, std::uint32_t>(&v2, u32_vec);
		ASSERT_EQ(v2, static_cast<std::uint32_t>(456));

		std::uint32_t v3;
		vector_get<2, std::uint32_t>(&v3, u32_vec);
		ASSERT_EQ(v3, static_cast<std::uint32_t>(789));

		std::uint32_t v4;
		vector_get<3, std::uint32_t>(&v4, u32_vec);
		ASSERT_EQ(v4, static_cast<std::uint32_t>(0));
	}
}

TEST(TestMath, TestVectorSet2)
{
	constexpr auto f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	{
		constexpr auto	vec1_1 = vector_set<0, float>(f32_vec, 321.f);
		constexpr auto* p1_1   = vec1_1.m128_f32;

		static_assert(float_eq(p1_1[0], 321.f));
		static_assert(float_eq(p1_1[1], 456.f));
		static_assert(float_eq(p1_1[2], 789.f));
		static_assert(float_eq(p1_1[3], 0.f));

		auto  vec1_2 = vector_set<0, float>(f32_vec, 321.f);
		auto* p1_2	 = vec1_2.m128_f32;

		ASSERT_FLOAT_EQ(p1_2[0], 321.f);
		ASSERT_FLOAT_EQ(p1_2[1], 456.f);
		ASSERT_FLOAT_EQ(p1_2[2], 789.f);
		ASSERT_FLOAT_EQ(p1_2[3], 0.f);

		constexpr auto	vec2_1 = vector_set<1, float>(f32_vec, 654.f);
		constexpr auto* p2_1   = vec2_1.m128_f32;

		static_assert(float_eq(p2_1[0], 123.f));
		static_assert(float_eq(p2_1[1], 654.f));
		static_assert(float_eq(p2_1[2], 789.f));
		static_assert(float_eq(p2_1[3], 0.f));

		auto  vec2_2 = vector_set<1, float>(f32_vec, 654.f);
		auto* p2_2	 = vec2_2.m128_f32;

		ASSERT_FLOAT_EQ(p2_2[0], 123.f);
		ASSERT_FLOAT_EQ(p2_2[1], 654.f);
		ASSERT_FLOAT_EQ(p2_2[2], 789.f);
		ASSERT_FLOAT_EQ(p2_2[3], 0.f);

		constexpr auto	vec3_1 = vector_set<2, float>(f32_vec, 987.f);
		constexpr auto* p3_1   = vec3_1.m128_f32;

		static_assert(float_eq(p3_1[0], 123.f));
		static_assert(float_eq(p3_1[1], 456.f));
		static_assert(float_eq(p3_1[2], 987.f));
		static_assert(float_eq(p3_1[3], 0.f));

		auto  vec3_2 = vector_set<2, float>(f32_vec, 987.f);
		auto* p3_2	 = vec3_2.m128_f32;

		ASSERT_FLOAT_EQ(p3_2[0], 123.f);
		ASSERT_FLOAT_EQ(p3_2[1], 456.f);
		ASSERT_FLOAT_EQ(p3_2[2], 987.f);
		ASSERT_FLOAT_EQ(p3_2[3], 0.f);

		constexpr auto	vec4_1 = vector_set<3, float>(f32_vec, 3.14f);
		constexpr auto* p4_1   = vec4_1.m128_f32;

		static_assert(float_eq(p4_1[0], 123.f));
		static_assert(float_eq(p4_1[1], 456.f));
		static_assert(float_eq(p4_1[2], 789.f));
		static_assert(float_eq(p4_1[3], 3.14f));

		auto  vec4_2 = vector_set<3, float>(f32_vec, 3.14f);
		auto* p4_2	 = vec4_2.m128_f32;

		ASSERT_FLOAT_EQ(p4_2[0], 123.f);
		ASSERT_FLOAT_EQ(p4_2[1], 456.f);
		ASSERT_FLOAT_EQ(p4_2[2], 789.f);
		ASSERT_FLOAT_EQ(p4_2[3], 3.14f);
	}
	{
		constexpr auto	vec1_1 = vector_set<0, std::int32_t>(i32_vec, static_cast<std::int32_t>(321));
		constexpr auto* p1_1   = vec1_1.m128_i32;

		static_assert(p1_1[0] == 321);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_set<0, std::int32_t>(i32_vec, static_cast<std::int32_t>(321));
		auto* p1_2	 = vec1_2.m128_i32;

		ASSERT_EQ(p1_2[0], static_cast<std::int32_t>(321));
		ASSERT_EQ(p1_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec2_1 = vector_set<1, std::int32_t>(i32_vec, static_cast<std::int32_t>(654));
		constexpr auto* p2_1   = vec2_1.m128_i32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 654);
		static_assert(p2_1[2] == 789);
		static_assert(p2_1[3] == 0);

		auto  vec2_2 = vector_set<1, std::int32_t>(i32_vec, static_cast<std::int32_t>(654));
		auto* p2_2	 = vec2_2.m128_i32;

		ASSERT_EQ(p2_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::int32_t>(654));
		ASSERT_EQ(p2_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p2_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec3_1 = vector_set<2, std::int32_t>(i32_vec, static_cast<std::int32_t>(987));
		constexpr auto* p3_1   = vec3_1.m128_i32;

		static_assert(p3_1[0] == 123);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 987);
		static_assert(p3_1[3] == 0);

		auto  vec3_2 = vector_set<2, std::int32_t>(i32_vec, static_cast<std::int32_t>(987));
		auto* p3_2	 = vec3_2.m128_i32;

		ASSERT_EQ(p3_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p3_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::int32_t>(987));
		ASSERT_EQ(p3_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec4_1 = vector_set<3, std::int32_t>(i32_vec, static_cast<std::int32_t>(42));
		constexpr auto* p4_1   = vec4_1.m128_i32;

		static_assert(p4_1[0] == 123);
		static_assert(p4_1[1] == 456);
		static_assert(p4_1[2] == 789);
		static_assert(p4_1[3] == 42);

		auto  vec4_2 = vector_set<3, std::int32_t>(i32_vec, static_cast<std::int32_t>(42));
		auto* p4_2	 = vec4_2.m128_i32;

		ASSERT_EQ(p4_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p4_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p4_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p4_2[3], static_cast<std::int32_t>(42));
	}
	{
		constexpr auto	vec1_1 = vector_set<0, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(321));
		constexpr auto* p1_1   = vec1_1.m128_u32;

		static_assert(p1_1[0] == 321);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_set<0, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(321));
		auto* p1_2	 = vec1_2.m128_u32;

		ASSERT_EQ(p1_2[0], static_cast<std::uint32_t>(321));
		ASSERT_EQ(p1_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec2_1 = vector_set<1, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(654));
		constexpr auto* p2_1   = vec2_1.m128_u32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 654);
		static_assert(p2_1[2] == 789);
		static_assert(p2_1[3] == 0);

		auto  vec2_2 = vector_set<1, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(654));
		auto* p2_2	 = vec2_2.m128_u32;

		ASSERT_EQ(p2_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::uint32_t>(654));
		ASSERT_EQ(p2_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p2_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec3_1 = vector_set<2, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(987));
		constexpr auto* p3_1   = vec3_1.m128_u32;

		static_assert(p3_1[0] == 123);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 987);
		static_assert(p3_1[3] == 0);

		auto  vec3_2 = vector_set<2, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(987));
		auto* p3_2	 = vec3_2.m128_u32;

		ASSERT_EQ(p3_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p3_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::uint32_t>(987));
		ASSERT_EQ(p3_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec4_1 = vector_set<3, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(42));
		constexpr auto* p4_1   = vec4_1.m128_u32;

		static_assert(p4_1[0] == 123);
		static_assert(p4_1[1] == 456);
		static_assert(p4_1[2] == 789);
		static_assert(p4_1[3] == 42);

		auto  vec4_2 = vector_set<3, std::uint32_t>(u32_vec, static_cast<std::uint32_t>(42));
		auto* p4_2	 = vec4_2.m128_u32;

		ASSERT_EQ(p4_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p4_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p4_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p4_2[3], static_cast<std::uint32_t>(42));
	}
}

TEST(TestMath, TestVectorSetPtr2)
{
	constexpr auto			f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto			i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto			u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	constexpr float			f32_arr[]{321.f, 654.f, 987.f, 3.14f};
	constexpr std::int32_t	i32_arr[]{321, 654, 987, 42};
	constexpr std::uint32_t u32_arr[]{321, 654, 987, 42};

	{
		constexpr auto	vec1_1 = vector_set<0, float>(f32_vec, f32_arr + 0);
		constexpr auto* p1_1   = vec1_1.m128_f32;

		static_assert(float_eq(p1_1[0], 321.f));
		static_assert(float_eq(p1_1[1], 456.f));
		static_assert(float_eq(p1_1[2], 789.f));
		static_assert(float_eq(p1_1[3], 0.f));

		auto  vec1_2 = vector_set<0, float>(f32_vec, f32_arr + 0);
		auto* p1_2	 = vec1_2.m128_f32;

		ASSERT_FLOAT_EQ(p1_2[0], 321.f);
		ASSERT_FLOAT_EQ(p1_2[1], 456.f);
		ASSERT_FLOAT_EQ(p1_2[2], 789.f);
		ASSERT_FLOAT_EQ(p1_2[3], 0.f);

		constexpr auto	vec2_1 = vector_set<1, float>(f32_vec, f32_arr + 1);
		constexpr auto* p2_1   = vec2_1.m128_f32;

		static_assert(float_eq(p2_1[0], 123.f));
		static_assert(float_eq(p2_1[1], 654.f));
		static_assert(float_eq(p2_1[2], 789.f));
		static_assert(float_eq(p2_1[3], 0.f));

		auto  vec2_2 = vector_set<1, float>(f32_vec, f32_arr + 1);
		auto* p2_2	 = vec2_2.m128_f32;

		ASSERT_FLOAT_EQ(p2_2[0], 123.f);
		ASSERT_FLOAT_EQ(p2_2[1], 654.f);
		ASSERT_FLOAT_EQ(p2_2[2], 789.f);
		ASSERT_FLOAT_EQ(p2_2[3], 0.f);

		constexpr auto	vec3_1 = vector_set<2, float>(f32_vec, f32_arr + 2);
		constexpr auto* p3_1   = vec3_1.m128_f32;

		static_assert(float_eq(p3_1[0], 123.f));
		static_assert(float_eq(p3_1[1], 456.f));
		static_assert(float_eq(p3_1[2], 987.f));
		static_assert(float_eq(p3_1[3], 0.f));

		auto  vec3_2 = vector_set<2, float>(f32_vec, f32_arr + 2);
		auto* p3_2	 = vec3_2.m128_f32;

		ASSERT_FLOAT_EQ(p3_2[0], 123.f);
		ASSERT_FLOAT_EQ(p3_2[1], 456.f);
		ASSERT_FLOAT_EQ(p3_2[2], 987.f);
		ASSERT_FLOAT_EQ(p3_2[3], 0.f);

		constexpr auto	vec4_1 = vector_set<3, float>(f32_vec, f32_arr + 3);
		constexpr auto* p4_1   = vec4_1.m128_f32;

		static_assert(float_eq(p4_1[0], 123.f));
		static_assert(float_eq(p4_1[1], 456.f));
		static_assert(float_eq(p4_1[2], 789.f));
		static_assert(float_eq(p4_1[3], 3.14f));

		auto  vec4_2 = vector_set<3, float>(f32_vec, f32_arr + 3);
		auto* p4_2	 = vec4_2.m128_f32;

		ASSERT_FLOAT_EQ(p4_2[0], 123.f);
		ASSERT_FLOAT_EQ(p4_2[1], 456.f);
		ASSERT_FLOAT_EQ(p4_2[2], 789.f);
		ASSERT_FLOAT_EQ(p4_2[3], 3.14f);
	}
	{
		constexpr auto	vec1_1 = vector_set<0, std::int32_t>(i32_vec, i32_arr + 0);
		constexpr auto* p1_1   = vec1_1.m128_i32;

		static_assert(p1_1[0] == 321);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_set<0, std::int32_t>(i32_vec, i32_arr + 0);
		auto* p1_2	 = vec1_2.m128_i32;

		ASSERT_EQ(p1_2[0], static_cast<std::int32_t>(321));
		ASSERT_EQ(p1_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec2_1 = vector_set<1, std::int32_t>(i32_vec, i32_arr + 1);
		constexpr auto* p2_1   = vec2_1.m128_i32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 654);
		static_assert(p2_1[2] == 789);
		static_assert(p2_1[3] == 0);

		auto  vec2_2 = vector_set<1, std::int32_t>(i32_vec, i32_arr + 1);
		auto* p2_2	 = vec2_2.m128_i32;

		ASSERT_EQ(p2_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::int32_t>(654));
		ASSERT_EQ(p2_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p2_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec3_1 = vector_set<2, std::int32_t>(i32_vec, i32_arr + 2);
		constexpr auto* p3_1   = vec3_1.m128_i32;

		static_assert(p3_1[0] == 123);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 987);
		static_assert(p3_1[3] == 0);

		auto  vec3_2 = vector_set<2, std::int32_t>(i32_vec, i32_arr + 2);
		auto* p3_2	 = vec3_2.m128_i32;

		ASSERT_EQ(p3_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p3_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::int32_t>(987));
		ASSERT_EQ(p3_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec4_1 = vector_set<3, std::int32_t>(i32_vec, i32_arr + 3);
		constexpr auto* p4_1   = vec4_1.m128_i32;

		static_assert(p4_1[0] == 123);
		static_assert(p4_1[1] == 456);
		static_assert(p4_1[2] == 789);
		static_assert(p4_1[3] == 42);

		auto  vec4_2 = vector_set<3, std::int32_t>(i32_vec, i32_arr + 3);
		auto* p4_2	 = vec4_2.m128_i32;

		ASSERT_EQ(p4_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p4_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p4_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p4_2[3], static_cast<std::int32_t>(42));
	}
	{
		constexpr auto	vec1_1 = vector_set<0, std::uint32_t>(u32_vec, u32_arr + 0);
		constexpr auto* p1_1   = vec1_1.m128_u32;

		static_assert(p1_1[0] == 321);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_set<0, std::uint32_t>(u32_vec, u32_arr + 0);
		auto* p1_2	 = vec1_2.m128_u32;

		ASSERT_EQ(p1_2[0], static_cast<std::uint32_t>(321));
		ASSERT_EQ(p1_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec2_1 = vector_set<1, std::uint32_t>(u32_vec, u32_arr + 1);
		constexpr auto* p2_1   = vec2_1.m128_u32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 654);
		static_assert(p2_1[2] == 789);
		static_assert(p2_1[3] == 0);

		auto  vec2_2 = vector_set<1, std::uint32_t>(u32_vec, u32_arr + 1);
		auto* p2_2	 = vec2_2.m128_u32;

		ASSERT_EQ(p2_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::uint32_t>(654));
		ASSERT_EQ(p2_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p2_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec3_1 = vector_set<2, std::uint32_t>(u32_vec, u32_arr + 2);
		constexpr auto* p3_1   = vec3_1.m128_u32;

		static_assert(p3_1[0] == 123);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 987);
		static_assert(p3_1[3] == 0);

		auto  vec3_2 = vector_set<2, std::uint32_t>(u32_vec, u32_arr + 2);
		auto* p3_2	 = vec3_2.m128_u32;

		ASSERT_EQ(p3_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p3_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::uint32_t>(987));
		ASSERT_EQ(p3_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec4_1 = vector_set<3, std::uint32_t>(u32_vec, u32_arr + 3);
		constexpr auto* p4_1   = vec4_1.m128_u32;

		static_assert(p4_1[0] == 123);
		static_assert(p4_1[1] == 456);
		static_assert(p4_1[2] == 789);
		static_assert(p4_1[3] == 42);

		auto  vec4_2 = vector_set<3, std::uint32_t>(u32_vec, u32_arr + 3);
		auto* p4_2	 = vec4_2.m128_u32;

		ASSERT_EQ(p4_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p4_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p4_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p4_2[3], static_cast<std::uint32_t>(42));
	}
}

TEST(TestMath, TestVectorSwizzle)
{
	constexpr auto f32_vec = vector_set(123.f, 456, 789, 0);
	constexpr auto i32_vec = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto u32_vec = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);

	{
		constexpr auto	vec1_1 = vector_swizzle<float>(f32_vec, 0, 1, 2, 3);
		constexpr auto* p1_1   = vec1_1.m128_f32;

		static_assert(float_eq(p1_1[0], 123.f));
		static_assert(float_eq(p1_1[1], 456.f));
		static_assert(float_eq(p1_1[2], 789.f));
		static_assert(float_eq(p1_1[3], 0.f));

		auto  vec1_2 = vector_swizzle<float>(f32_vec, 0, 1, 2, 3);
		auto* p1_2	 = vec1_2.m128_f32;

		ASSERT_FLOAT_EQ(p1_2[0], 123.f);
		ASSERT_FLOAT_EQ(p1_2[1], 456.f);
		ASSERT_FLOAT_EQ(p1_2[2], 789.f);
		ASSERT_FLOAT_EQ(p1_2[3], 0.f);

		constexpr auto	vec2_1 = vector_swizzle<float>(f32_vec, 3, 2, 1, 0);
		constexpr auto* p2_1   = vec2_1.m128_f32;

		static_assert(float_eq(p2_1[0], 0.f));
		static_assert(float_eq(p2_1[1], 789.f));
		static_assert(float_eq(p2_1[2], 456.f));
		static_assert(float_eq(p2_1[3], 123.f));

		auto  vec2_2 = vector_swizzle<float>(f32_vec, 3, 2, 1, 0);
		auto* p2_2	 = vec2_2.m128_f32;

		ASSERT_FLOAT_EQ(p2_2[0], 0.f);
		ASSERT_FLOAT_EQ(p2_2[1], 789.f);
		ASSERT_FLOAT_EQ(p2_2[2], 456.f);
		ASSERT_FLOAT_EQ(p2_2[3], 123.f);

		constexpr auto	vec3_1 = vector_swizzle<float>(f32_vec, 1, 1, 2, 2);
		constexpr auto* p3_1   = vec3_1.m128_f32;

		static_assert(float_eq(p3_1[0], 456.f));
		static_assert(float_eq(p3_1[1], 456.f));
		static_assert(float_eq(p3_1[2], 789.f));
		static_assert(float_eq(p3_1[3], 789.f));

		auto  vec3_2 = vector_swizzle<float>(f32_vec, 1, 1, 2, 2);
		auto* p3_2	 = vec3_2.m128_f32;

		ASSERT_FLOAT_EQ(p3_2[0], 456.f);
		ASSERT_FLOAT_EQ(p3_2[1], 456.f);
		ASSERT_FLOAT_EQ(p3_2[2], 789.f);
		ASSERT_FLOAT_EQ(p3_2[3], 789.f);

		constexpr auto	vec4_1 = vector_swizzle<float>(f32_vec, 3, 3, 0, 0);
		constexpr auto* p4_1   = vec4_1.m128_f32;

		static_assert(float_eq(p4_1[0], 0.f));
		static_assert(float_eq(p4_1[1], 0.f));
		static_assert(float_eq(p4_1[2], 123.f));
		static_assert(float_eq(p4_1[3], 123.f));

		auto  vec4_2 = vector_swizzle<float>(f32_vec, 3, 3, 0, 0);
		auto* p4_2	 = vec4_2.m128_f32;

		ASSERT_FLOAT_EQ(p4_2[0], 0.f);
		ASSERT_FLOAT_EQ(p4_2[1], 0.f);
		ASSERT_FLOAT_EQ(p4_2[2], 123.f);
		ASSERT_FLOAT_EQ(p4_2[3], 123.f);
	}
	{
		constexpr auto	vec1_1 = vector_swizzle<std::int32_t>(i32_vec, 0, 1, 2, 3);
		constexpr auto* p1_1   = vec1_1.m128_i32;

		static_assert(p1_1[0] == 123);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_swizzle<std::int32_t>(i32_vec, 0, 1, 2, 3);
		auto* p1_2	 = vec1_2.m128_i32;

		ASSERT_EQ(p1_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p1_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::int32_t>(0));

		constexpr auto	vec2_1 = vector_swizzle<std::int32_t>(i32_vec, 3, 2, 1, 0);
		constexpr auto* p2_1   = vec2_1.m128_i32;

		static_assert(p2_1[0] == 0);
		static_assert(p2_1[1] == 789);
		static_assert(p2_1[2] == 456);
		static_assert(p2_1[3] == 123);

		auto  vec2_2 = vector_swizzle<std::int32_t>(i32_vec, 3, 2, 1, 0);
		auto* p2_2	 = vec2_2.m128_i32;

		ASSERT_EQ(p2_2[0], static_cast<std::int32_t>(0));
		ASSERT_EQ(p2_2[1], static_cast<std::int32_t>(789));
		ASSERT_EQ(p2_2[2], static_cast<std::int32_t>(456));
		ASSERT_EQ(p2_2[3], static_cast<std::int32_t>(123));

		constexpr auto	vec3_1 = vector_swizzle<std::int32_t>(i32_vec, 1, 1, 2, 2);
		constexpr auto* p3_1   = vec3_1.m128_i32;

		static_assert(p3_1[0] == 456);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 789);
		static_assert(p3_1[3] == 789);

		auto  vec3_2 = vector_swizzle<std::int32_t>(i32_vec, 1, 1, 2, 2);
		auto* p3_2	 = vec3_2.m128_i32;

		ASSERT_EQ(p3_2[0], static_cast<std::int32_t>(456));
		ASSERT_EQ(p3_2[1], static_cast<std::int32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::int32_t>(789));
		ASSERT_EQ(p3_2[3], static_cast<std::int32_t>(789));

		constexpr auto	vec4_1 = vector_swizzle<std::int32_t>(i32_vec, 3, 3, 0, 0);
		constexpr auto* p4_1   = vec4_1.m128_i32;

		static_assert(p4_1[0] == 0);
		static_assert(p4_1[1] == 0);
		static_assert(p4_1[2] == 123);
		static_assert(p4_1[3] == 123);

		auto  vec4_2 = vector_swizzle<std::int32_t>(i32_vec, 3, 3, 0, 0);
		auto* p4_2	 = vec4_2.m128_i32;

		ASSERT_EQ(p4_2[0], static_cast<std::int32_t>(0));
		ASSERT_EQ(p4_2[1], static_cast<std::int32_t>(0));
		ASSERT_EQ(p4_2[2], static_cast<std::int32_t>(123));
		ASSERT_EQ(p4_2[3], static_cast<std::int32_t>(123));
	}
	{
		constexpr auto	vec1_1 = vector_swizzle<std::uint32_t>(u32_vec, 0, 1, 2, 3);
		constexpr auto* p1_1   = vec1_1.m128_u32;

		static_assert(p1_1[0] == 123);
		static_assert(p1_1[1] == 456);
		static_assert(p1_1[2] == 789);
		static_assert(p1_1[3] == 0);

		auto  vec1_2 = vector_swizzle<std::uint32_t>(u32_vec, 0, 1, 2, 3);
		auto* p1_2	 = vec1_2.m128_u32;

		ASSERT_EQ(p1_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p1_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p1_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p1_2[3], static_cast<std::uint32_t>(0));

		constexpr auto	vec2_1 = vector_swizzle<std::uint32_t>(u32_vec, 3, 2, 1, 0);
		constexpr auto* p2_1   = vec2_1.m128_u32;

		static_assert(p2_1[0] == 0);
		static_assert(p2_1[1] == 789);
		static_assert(p2_1[2] == 456);
		static_assert(p2_1[3] == 123);

		auto  vec2_2 = vector_swizzle<std::uint32_t>(u32_vec, 3, 2, 1, 0);
		auto* p2_2	 = vec2_2.m128_u32;

		ASSERT_EQ(p2_2[0], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p2_2[1], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p2_2[2], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p2_2[3], static_cast<std::uint32_t>(123));

		constexpr auto	vec3_1 = vector_swizzle<std::uint32_t>(u32_vec, 1, 1, 2, 2);
		constexpr auto* p3_1   = vec3_1.m128_u32;

		static_assert(p3_1[0] == 456);
		static_assert(p3_1[1] == 456);
		static_assert(p3_1[2] == 789);
		static_assert(p3_1[3] == 789);

		auto  vec3_2 = vector_swizzle<std::uint32_t>(u32_vec, 1, 1, 2, 2);
		auto* p3_2	 = vec3_2.m128_u32;

		ASSERT_EQ(p3_2[0], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p3_2[1], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p3_2[2], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p3_2[3], static_cast<std::uint32_t>(789));

		constexpr auto	vec4_1 = vector_swizzle<std::uint32_t>(u32_vec, 3, 3, 0, 0);
		constexpr auto* p4_1   = vec4_1.m128_u32;

		static_assert(p4_1[0] == 0);
		static_assert(p4_1[1] == 0);
		static_assert(p4_1[2] == 123);
		static_assert(p4_1[3] == 123);

		auto  vec4_2 = vector_swizzle<std::uint32_t>(u32_vec, 3, 3, 0, 0);
		auto* p4_2	 = vec4_2.m128_u32;

		ASSERT_EQ(p4_2[0], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p4_2[1], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p4_2[2], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p4_2[3], static_cast<std::uint32_t>(123));
	}
}

TEST(TestMath, TestVectorPermute)
{
	constexpr auto f32_vec1 = vector_set(123.f, 456, 789, 0);
	constexpr auto f32_vec2 = vector_set(1.f, 987, 654, 321);
	constexpr auto i32_vec1 = vector_set(static_cast<std::int32_t>(123), 456, 789, 0);
	constexpr auto i32_vec2 = vector_set(static_cast<std::int32_t>(1), 987, 654, 321);
	constexpr auto u32_vec1 = vector_set(static_cast<std::uint32_t>(123), 456, 789, 0);
	constexpr auto u32_vec2 = vector_set(static_cast<std::uint32_t>(1), 987, 654, 321);

	{
		constexpr auto	vec1_1 = vector_permute<float>(f32_vec1, f32_vec2, 1, 3, 5, 7);
		constexpr auto* p1_1   = vec1_1.m128_f32;

		static_assert(float_eq(p1_1[0], 456.f));
		static_assert(float_eq(p1_1[1], 0.f));
		static_assert(float_eq(p1_1[2], 987.f));
		static_assert(float_eq(p1_1[3], 321.f));

		auto  vec1_2 = vector_permute<float>(f32_vec1, f32_vec2, 1, 3, 5, 7);
		auto* p1_2	 = vec1_2.m128_f32;

		ASSERT_FLOAT_EQ(p1_2[0], 456.f);
		ASSERT_FLOAT_EQ(p1_2[1], 0.f);
		ASSERT_FLOAT_EQ(p1_2[2], 987.f);
		ASSERT_FLOAT_EQ(p1_2[3], 321.f);

		constexpr auto	vec2_1 = vector_permute<float>(f32_vec1, f32_vec2, 0, 2, 4, 6);
		constexpr auto* p2_1   = vec2_1.m128_f32;

		static_assert(float_eq(p2_1[0], 123.f));
		static_assert(float_eq(p2_1[1], 789.f));
		static_assert(float_eq(p2_1[2], 1.f));
		static_assert(float_eq(p2_1[3], 654.f));

		auto  vec2_2 = vector_permute<float>(f32_vec1, f32_vec2, 0, 2, 4, 6);
		auto* p2_2	 = vec2_2.m128_f32;

		ASSERT_FLOAT_EQ(p2_2[0], 123.f);
		ASSERT_FLOAT_EQ(p2_2[1], 789.f);
		ASSERT_FLOAT_EQ(p2_2[2], 1.f);
		ASSERT_FLOAT_EQ(p2_2[3], 654.f);
	}
	{
		constexpr auto	vec1_1 = vector_permute<std::int32_t>(i32_vec1, i32_vec2, 1, 3, 5, 7);
		constexpr auto* p1_1   = vec1_1.m128_i32;

		static_assert(p1_1[0] == 456);
		static_assert(p1_1[1] == 0);
		static_assert(p1_1[2] == 987);
		static_assert(p1_1[3] == 321);

		auto  vec1_2 = vector_permute<std::int32_t>(i32_vec1, i32_vec2, 1, 3, 5, 7);
		auto* p1_2	 = vec1_2.m128_i32;

		ASSERT_EQ(p1_2[0], static_cast<std::int32_t>(456));
		ASSERT_EQ(p1_2[1], static_cast<std::int32_t>(0));
		ASSERT_EQ(p1_2[2], static_cast<std::int32_t>(987));
		ASSERT_EQ(p1_2[3], static_cast<std::int32_t>(321));

		constexpr auto	vec2_1 = vector_permute<std::int32_t>(i32_vec1, i32_vec2, 0, 2, 4, 6);
		constexpr auto* p2_1   = vec2_1.m128_i32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 789);
		static_assert(p2_1[2] == 1);
		static_assert(p2_1[3] == 654);

		auto  vec2_2 = vector_permute<std::int32_t>(i32_vec1, i32_vec2, 0, 2, 4, 6);
		auto* p2_2	 = vec2_2.m128_i32;

		ASSERT_EQ(p2_2[0], static_cast<std::int32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::int32_t>(789));
		ASSERT_EQ(p2_2[2], static_cast<std::int32_t>(1));
		ASSERT_EQ(p2_2[3], static_cast<std::int32_t>(654));
	}
	{
		constexpr auto	vec1_1 = vector_permute<std::uint32_t>(u32_vec1, u32_vec2, 1, 3, 5, 7);
		constexpr auto* p1_1   = vec1_1.m128_u32;

		static_assert(p1_1[0] == 456);
		static_assert(p1_1[1] == 0);
		static_assert(p1_1[2] == 987);
		static_assert(p1_1[3] == 321);

		auto  vec1_2 = vector_permute<std::uint32_t>(u32_vec1, u32_vec2, 1, 3, 5, 7);
		auto* p1_2	 = vec1_2.m128_u32;

		ASSERT_EQ(p1_2[0], static_cast<std::uint32_t>(456));
		ASSERT_EQ(p1_2[1], static_cast<std::uint32_t>(0));
		ASSERT_EQ(p1_2[2], static_cast<std::uint32_t>(987));
		ASSERT_EQ(p1_2[3], static_cast<std::uint32_t>(321));

		constexpr auto	vec2_1 = vector_permute<std::uint32_t>(u32_vec1, u32_vec2, 0, 2, 4, 6);
		constexpr auto* p2_1   = vec2_1.m128_u32;

		static_assert(p2_1[0] == 123);
		static_assert(p2_1[1] == 789);
		static_assert(p2_1[2] == 1);
		static_assert(p2_1[3] == 654);

		auto  vec2_2 = vector_permute<std::uint32_t>(u32_vec1, u32_vec2, 0, 2, 4, 6);
		auto* p2_2	 = vec2_2.m128_u32;

		ASSERT_EQ(p2_2[0], static_cast<std::uint32_t>(123));
		ASSERT_EQ(p2_2[1], static_cast<std::uint32_t>(789));
		ASSERT_EQ(p2_2[2], static_cast<std::uint32_t>(1));
		ASSERT_EQ(p2_2[3], static_cast<std::uint32_t>(654));
	}
}

#endif
