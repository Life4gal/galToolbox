#pragma once

// SSE
#include <emmintrin.h>
#include <xmmintrin.h>

#include <cfloat>
#include <cmath>
#include <span>
#include <utils/assert.hpp>
#include <utils/tuple_maker.hpp>

namespace gal::toolbox::math
{
	inline namespace data_types
	{
		//=======================================================
		// for operations
		//=======================================================
		using vector = __m128;
		template<typename T>
		struct alignas(16) generic_vector;
		using f32_vector = generic_vector<float>;
		using i32_vector = generic_vector<std::int32_t>;
		using u8_vector	 = generic_vector<std::uint8_t>;
		using u32_vector = generic_vector<std::uint32_t>;
		struct alignas(16) matrix;

		//=======================================================
		// for storage
		//=======================================================
		template<typename T, std::size_t Size>
		struct generic_one_tier_container;
		using float2 = generic_one_tier_container<float, 2>;
		struct alignas(16) float2a;
		using float3 = generic_one_tier_container<float, 3>;
		struct alignas(16) float3a;
		using float4 = generic_one_tier_container<float, 4>;
		struct alignas(16) float4a;
		using int2 = generic_one_tier_container<std::int32_t, 2>;
		struct alignas(16) int2a;
		using int3 = generic_one_tier_container<std::int32_t, 3>;
		struct alignas(16) int3a;
		using int4 = generic_one_tier_container<std::int32_t, 4>;
		struct alignas(16) int4a;
		using uint2 = generic_one_tier_container<std::uint32_t, 2>;
		struct alignas(16) uint2a;
		using uint3 = generic_one_tier_container<std::uint32_t, 3>;
		struct alignas(16) uint3a;
		using uint4 = generic_one_tier_container<std::uint32_t, 4>;
		struct alignas(16) uint4a;

		template<typename T, std::size_t FirstTier, std::size_t SecondTier>
		struct generic_two_tier_container;
		using float3x3 = generic_two_tier_container<float, 3, 3>;
		struct alignas(16) float3x3a;
		using float3x4 = generic_two_tier_container<float, 3, 4>;
		struct alignas(16) float3x4a;
		using float4x3 = generic_two_tier_container<float, 4, 3>;
		struct alignas(16) float4x3a;
		using float4x4 = generic_two_tier_container<float, 4, 4>;
		struct alignas(16) float4x4a;

		template<typename T>
		struct is_vector : std::false_type
		{
		};
		template<>
		struct is_vector<f32_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<i32_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<u8_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<u32_vector> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_vector_v = is_vector<T>::value;
		template<typename T>
		concept vector_t = is_vector_v<T>;

		template<typename T>
		struct is_one_tier_container : std::false_type
		{
		};
		template<>
		struct is_one_tier_container<float2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float2a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float3a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float4> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float4a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int2a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int3a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int4> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int4a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint2a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint3a> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint4> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint4a> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_one_tier_container_v = is_one_tier_container<T>::value;
		template<typename T>
		concept one_tier_container_t = is_one_tier_container_v<T>;
		template<typename T>
		struct is_aligned_one_tier_container : std::false_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float4a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int4a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint4a> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_aligned_one_tier_container_v = is_aligned_one_tier_container<T>::value;
		template<typename T>
		concept aligned_one_tier_container_t = is_aligned_one_tier_container_v<T>;
		template<typename T>
		struct is_two_tier_container : std::false_type
		{
		};
		template<>
		struct is_two_tier_container<float3x3> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float3x3a> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float3x4> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float3x4a> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x3> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x3a> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x4> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x4a> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_two_tier_container_v = is_two_tier_container<T>::value;
		template<typename T>
		concept two_tier_container_t = is_two_tier_container_v<T>;
	}// namespace data_types

	inline namespace data_types
	{
		template<typename T>
		struct alignas(16) generic_vector
		{
			// the size of the required type is a factor of 16
			static_assert(16 % sizeof(T) == 0);
			static_assert(sizeof(T) <= 16);

			using value_type				= T;
			using size_type					= std::size_t;

			constexpr static size_type size = 16 / sizeof(value_type);

			union
			{
				value_type v1[size];
				vector	   v2;
			} data;

			constexpr explicit operator const value_type*() const noexcept
			{
				return data.v1;
			}

			constexpr explicit operator vector() const noexcept
			{
				return data.v2;
			}
		};

		static_assert(f32_vector::size == 4);
		static_assert(i32_vector::size == 4);
		static_assert(u8_vector::size == 16);
		static_assert(u32_vector::size == 4);

		struct alignas(16) matrix
		{
			vector data[4];

			constexpr matrix() noexcept = default;

			constexpr matrix(
					const vector& v1,
					const vector& v2,
					const vector& v3,
					const vector& v4) noexcept
				: data{v1, v2, v3, v4} {}

			constexpr matrix(
					float f00,
					float f01,
					float f02,
					float f03,

					float f10,
					float f11,
					float f12,
					float f13,

					float f20,
					float f21,
					float f22,
					float f23,

					float f30,
					float f31,
					float f32,
					float f33) noexcept;

			constexpr explicit matrix(std::span<float, 16> s) noexcept;
		};

		template<typename T, std::size_t Size>
		struct generic_one_tier_container
		{
			using value_type					  = T;
			using size_type						  = std::size_t;

			constexpr static size_type size		  = Size;

			constexpr static size_type index_of_x = 0;
			constexpr static size_type index_of_y = 1;
			constexpr static size_type index_of_z = 2;
			constexpr static size_type index_of_w = 3;

			using reference						  = value_type&;

			value_type data[size];

			constexpr generic_one_tier_container() noexcept = default;

			template<typename... Args>
			requires(sizeof...(Args) == size) constexpr generic_one_tier_container(Args&&... args) noexcept;

			constexpr explicit generic_one_tier_container(std::span<float, size> s) noexcept;

			[[nodiscard]] constexpr reference x() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type x() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference r() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type r() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference s() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type s() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference y() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type y() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference g() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type g() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference t() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type t() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference z() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type z() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference b() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type b() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference p() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type p() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference w() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type w() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr reference a() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type a() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr reference q() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type q() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}
		};

		struct alignas(16) float2a : public float2
		{
			using float2::float2;
		};
		struct alignas(16) float3a : public float3
		{
			using float3::float3;
		};
		struct alignas(16) float4a : public float4
		{
			using float4::float4;
		};

		struct alignas(16) int2a : public int2
		{
			using int2::int2;
		};
		struct alignas(16) int3a : public int3
		{
			using int3::int3;
		};
		struct alignas(16) int4a : public int4
		{
			using int4::int4;
		};

		struct alignas(16) uint2a : public uint2
		{
			using uint2::uint2;
		};
		struct alignas(16) uint3a : public uint3
		{
			using uint3::uint3;
		};
		struct alignas(16) uint4a : public uint4
		{
			using uint4::uint4;
		};

		template<typename T, std::size_t FirstTier, std::size_t SecondTier>
		struct generic_two_tier_container
		{
			using value_type					   = T;
			using size_type						   = std::size_t;

			constexpr static size_type first_size  = FirstTier;
			constexpr static size_type second_size = SecondTier;
			constexpr static size_type size		   = first_size * second_size;

			using reference						   = value_type&;

			value_type data[first_size][second_size];

			constexpr generic_two_tier_container() noexcept = default;

			template<typename... Args>
			requires(sizeof...(Args) == size) constexpr generic_two_tier_container(Args&&... args) noexcept;

			constexpr explicit generic_two_tier_container(std::span<float, size> s) noexcept;

			constexpr reference operator()(size_type row, size_type column) noexcept
			{
				return data[row][column];
			}

			constexpr value_type operator()(size_type row, size_type column) const noexcept
			{
				return data[row][column];
			}
		};

		struct alignas(16) float3x3a : public float3x3
		{
			using float3x3::float3x3;
		};
		struct alignas(16) float3x4a : public float3x4
		{
			using float3x4::float3x4;
		};
		struct alignas(16) float4x3a : public float4x3
		{
			using float4x3::float4x3;
		};
		struct alignas(16) float4x4a : public float4x4
		{
			using float4x4::float4x4;
		};
	}// namespace data_types

	inline namespace constants
	{
		// pi / 4
		constexpr static float		   quarter_pi	   = 0.785'398'163f;
		// pi / 2
		constexpr static float		   half_pi		   = 1.570'796'326;
		// pi
		constexpr static float		   pi			   = 3.141'592'653f;
		// 2 * pi
		constexpr static float		   two_pi		   = 6.283'185'307f;

		// 1 / pi
		constexpr static float		   one_over_pi	   = 0.318'309'886f;
		// 1 / (2 * pi)
		constexpr static float		   one_over_two_pi = 0.159'154'943f;

		constexpr static std::uint32_t select_0		   = 0x00000000;
		constexpr static std::uint32_t select_1		   = 0xFFFFFFFF;

		constexpr static f32_vector	   g_vector_sin_coefficients0{-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f};
		constexpr static f32_vector	   g_vector_sin_coefficients1{-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/};
		constexpr static f32_vector	   g_vector_cos_coefficients0{-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f};
		constexpr static f32_vector	   g_vector_cos_coefficients1{-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/};
		constexpr static f32_vector	   g_vector_tan_coefficients0{1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f};
		constexpr static f32_vector	   g_vector_tan_coefficients1{2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f};
		constexpr static f32_vector	   g_vector_tan_coefficients2{5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f};
		constexpr static f32_vector	   g_vector_arc_coefficients0{+1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f};
		constexpr static f32_vector	   g_vector_arc_coefficients1{+0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f};
		constexpr static f32_vector	   g_vector_atan_coefficients0{-0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f};
		constexpr static f32_vector	   g_vector_atan_coefficients1{-0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f};
		constexpr static f32_vector	   g_vector_atan_est_coefficients0{+0.999866f, +0.999866f, +0.999866f, +0.999866f};
		constexpr static f32_vector	   g_vector_atan_est_coefficients1{-0.3302995f, +0.180141f, -0.085133f, +0.0208351f};
		constexpr static f32_vector	   g_vector_tan_est_coefficients{2.484f, -1.954923183e-1f, 2.467401101f, pi};
		constexpr static f32_vector	   g_vector_arc_est_coefficients{+1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f};
		constexpr static f32_vector	   g_vector_identity_r0{1.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r1{0.0f, 1.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r2{0.0f, 0.0f, 1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r3{0.0f, 0.0f, 0.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r0{-1.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r1{0.0f, -1.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r2{0.0f, 0.0f, -1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r3{0.0f, 0.0f, 0.0f, -1.0f};
		constexpr static u32_vector	   g_vector_neg_zero{0x80000000, 0x80000000, 0x80000000, 0x80000000};
		constexpr static u32_vector	   g_vector_neg_xyz{0x80000000, 0x80000000, 0x80000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_xy{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_xyz{0x80000000, 0x80000000, 0x80000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_x{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_y{0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_z{0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_w{0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};
		constexpr static f32_vector	   g_vector_zero{0.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_one{1.0f, 1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_one_xyz{1.0f, 1.0f, 1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_one{-1.0f, -1.0f, -1.0f, -1.0f};
		constexpr static f32_vector	   g_vector_half_one{0.5f, 0.5f, 0.5f, 0.5f};
		constexpr static f32_vector	   g_vector_neg_half_one{-0.5f, -0.5f, -0.5f, -0.5f};
		constexpr static f32_vector	   g_vector_quarter_pi{quarter_pi, quarter_pi, quarter_pi, quarter_pi};
		constexpr static f32_vector	   g_vector_half_pi{half_pi, half_pi, half_pi, half_pi};
		constexpr static f32_vector	   g_vector_pi{pi, pi, pi, pi};
		constexpr static f32_vector	   g_vector_two_pi{two_pi, two_pi, two_pi, two_pi};
		constexpr static f32_vector	   g_vector_neg_quarter_pi{-quarter_pi, -quarter_pi, -quarter_pi, -quarter_pi};
		constexpr static f32_vector	   g_vector_neg_half_pi{-half_pi, -half_pi, -half_pi, -half_pi};
		constexpr static f32_vector	   g_vector_neg_pi{-pi, -pi, -pi, -pi};
		constexpr static f32_vector	   g_vector_neg_two_pi{-two_pi, -two_pi, -two_pi, -two_pi};
		constexpr static f32_vector	   g_vector_reciprocal_pi{one_over_pi, one_over_pi, one_over_pi, one_over_pi};
		constexpr static f32_vector	   g_vector_reciprocal_two_pi{one_over_two_pi, one_over_two_pi, one_over_two_pi, one_over_two_pi};
		constexpr static f32_vector	   g_vector_neg_reciprocal_pi{-one_over_pi, -one_over_pi, -one_over_pi, -one_over_pi};
		constexpr static f32_vector	   g_vector_neg_reciprocal_two_pi{-one_over_two_pi, -one_over_two_pi, -one_over_two_pi, -one_over_two_pi};
		constexpr static f32_vector	   g_vector_epsilon{1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f};
		constexpr static i32_vector	   g_vector_half_one_minus_epsilon{0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD};
		constexpr static i32_vector	   g_vector_infinity{0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
		constexpr static i32_vector	   g_vector_nan{0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};
		constexpr static i32_vector	   g_vector_nan_test{0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF};
		constexpr static i32_vector	   g_vector_abs_mask{0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};
		constexpr static i32_vector	   g_vector_float_min{0x00800000, 0x00800000, 0x00800000, 0x00800000};
		constexpr static i32_vector	   g_vector_float_max{0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF};
		constexpr static u32_vector	   g_vector_neg_one_mask{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
		constexpr static u32_vector	   g_vector_mask_a8r8g8b8{0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};
		constexpr static u32_vector	   g_vector_flip_a8r8g8b8{0x00000000, 0x00000000, 0x00000000, 0x80000000};
		constexpr static f32_vector	   g_vector_fix_a8r8g8b8{0.0f, 0.0f, 0.0f, float(0x80000000U)};
		constexpr static f32_vector	   g_vector_normalize_a8r8g8b8{1.0f / (255.0f * float(0x10000)), 1.0f / (255.0f * float(0x100)), 1.0f / 255.0f, 1.0f / (255.0f * float(0x1000000))};
		constexpr static u32_vector	   g_vector_mask_a2b10g10r10{0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000};
		constexpr static u32_vector	   g_vector_flip_a2b10g10r10{0x00000200, 0x00080000, 0x20000000, 0x80000000};
		constexpr static f32_vector	   g_vector_fix_a2b10g10r10{-512.0f, -512.0f * float(0x400), -512.0f * float(0x100000), float(0x80000000U)};
		constexpr static f32_vector	   g_vector_normalize_a2b10g10r10{1.0f / 511.0f, 1.0f / (511.0f * float(0x400)), 1.0f / (511.0f * float(0x100000)), 1.0f / (3.0f * float(0x40000000))};
		constexpr static u32_vector	   g_vector_mask_x16y16{0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000};
		constexpr static i32_vector	   g_vector_flip_x16y16{0x00008000, 0x00000000, 0x00000000, 0x00000000};
		constexpr static f32_vector	   g_vector_fix_x16y16{-32768.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_normalize_x16y16{1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f};
		constexpr static u32_vector	   g_vector_mask_x16y16z16w16{0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000};
		constexpr static i32_vector	   g_vector_flip_x16y16z16w16{0x00008000, 0x00008000, 0x00000000, 0x00000000};
		constexpr static f32_vector	   g_vector_fix_x16y16z16w16{-32768.0f, -32768.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_normalize_x16y16z16w16{1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f)};
		constexpr static f32_vector	   g_vector_no_fraction{8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f};
		constexpr static i32_vector	   g_vector_mask_byte{0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
		constexpr static f32_vector	   g_vector_neg_x{-1.0f, 1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_y{1.0f, -1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_z{1.0f, 1.0f, -1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_w{1.0f, 1.0f, 1.0f, -1.0f};
		constexpr static u32_vector	   g_vector_select_0101{select_0, select_1, select_0, select_1};
		constexpr static u32_vector	   g_vector_select_1010{select_1, select_0, select_1, select_0};
		constexpr static u32_vector	   g_vector_select_1000{select_1, select_0, select_0, select_0};
		constexpr static u32_vector	   g_vector_select_1100{select_1, select_1, select_0, select_0};
		constexpr static u32_vector	   g_vector_select_1110{select_1, select_1, select_1, select_0};
		constexpr static u32_vector	   g_vector_select_1011{select_1, select_0, select_1, select_1};
		constexpr static f32_vector	   g_vector_fixup_y16{1.0f, 1.0f / 65536.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_fixup_y16w16{1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f};
		constexpr static u32_vector	   g_vector_flip_y{0, 0x80000000, 0, 0};
		constexpr static u32_vector	   g_vector_flip_z{0, 0, 0x80000000, 0};
		constexpr static u32_vector	   g_vector_flip_w{0, 0, 0, 0x80000000};
		constexpr static u32_vector	   g_vector_flip_yz{0, 0x80000000, 0x80000000, 0};
		constexpr static u32_vector	   g_vector_flip_zw{0, 0, 0x80000000, 0x80000000};
		constexpr static u32_vector	   g_vector_flip_yw{0, 0x80000000, 0, 0x80000000};
		constexpr static i32_vector	   g_vector_mask_dec4{0x3FF, 0x3FF << 10, 0x3FF << 20, static_cast<int>(0xC0000000)};
		constexpr static i32_vector	   g_vector_xor_dec4{0x200, 0x200 << 10, 0x200 << 20, 0};
		constexpr static f32_vector	   g_vector_add_u_dec4{0, 0, 0, 32768.0f * 65536.0f};
		constexpr static f32_vector	   g_vector_add_dec4{-512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0};
		constexpr static f32_vector	   g_vector_mul_dec4{1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f)};
		constexpr static u32_vector	   g_vector_mask_byte4{0xFF, 0xFF00, 0xFF0000, 0xFF000000};
		constexpr static i32_vector	   g_vector_xor_byte4{0x80, 0x8000, 0x800000, 0x00000000};
		constexpr static f32_vector	   g_vector_add_byte4{-128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0};
		constexpr static f32_vector	   g_vector_fix_unsigned{32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f};
		constexpr static f32_vector	   g_vector_max_int{65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f};
		constexpr static f32_vector	   g_vector_max_uint{65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f};
		constexpr static f32_vector	   g_vector_unsigned_fix{32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f};


	}// namespace constants

	inline namespace operations
	{
		using vector = data_types::vector;
		using matrix = data_types::matrix;

		[[nodiscard]] constexpr vector operator+(const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v) noexcept;

		constexpr vector&			   operator+=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator-=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator*=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator/=(vector& v1, const vector& v2) noexcept;

		constexpr vector&			   operator+=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator-=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator*=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator/=(vector& v, float scalar) noexcept;

		[[nodiscard]] constexpr vector operator+(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator*(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator/(const vector& v1, const vector& v2) noexcept;

		[[nodiscard]] constexpr vector operator+(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator*(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator/(const vector& v, float scalar) noexcept;

		[[nodiscard]] constexpr vector operator+(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator-(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator*(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator/(float scalar, const vector& v) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m) noexcept;

		constexpr matrix&			   operator+=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator-=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator*=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator/=(matrix& m1, const matrix& m2) noexcept;

		constexpr matrix&			   operator+=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator-=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator*=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator/=(matrix& m, float scalar) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator*(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator/(const matrix& m1, const matrix& m2) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator*(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator/(const matrix& m, float scalar) noexcept;

		[[nodiscard]] constexpr matrix operator+(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator-(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator*(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator/(float scalar, const matrix& m) noexcept;
	}// namespace operations

	inline namespace utils
	{
		constexpr float degrees2radians(float degrees) noexcept
		{
			return degrees * (constants::pi / 180.f);
		}

		constexpr float radians2degrees(float radians) noexcept
		{
			return radians * (180.f / constants::pi);
		}

		[[nodiscard]] constexpr vector vector_i2f(const vector& v, std::uint32_t div_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_f2i(const vector& v, std::uint32_t mul_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_ui2f(const vector& v, std::uint32_t div_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_f2ui(const vector& v, std::uint32_t mul_exponent) noexcept;

		template<one_tier_container_t T>
		constexpr vector vector_load(std::span<const float, T::size> source) noexcept;
		template<one_tier_container_t T>
		constexpr vector vector_load(const T& source) noexcept;

		template<two_tier_container_t T>
		constexpr matrix matrix_load(std::span<const float, T::size> source) noexcept;
		template<two_tier_container_t T>
		constexpr matrix matrix_load(const T& source) noexcept;

		template<one_tier_container_t T>
		constexpr void vector_store(std::span<float, T::size> dest, const vector& source) noexcept;
		template<one_tier_container_t T>
		constexpr void vector_store(T& dest, const vector& source) noexcept;

		template<two_tier_container_t T>
		constexpr void matrix_store(std::span<float, T::size> dest, const matrix& source) noexcept;
		template<two_tier_container_t T>
		constexpr void matrix_store(T& dest, const matrix& source) noexcept;
	}// namespace utils
}// namespace gal::toolbox::math

#include <math/details/math_utils.inl>