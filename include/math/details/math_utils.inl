#pragma once

#include <utils/assert.hpp>

// SSE3
#include <pmmintrin.h>
// SSE4
#include <smmintrin.h>
// AVX
#include <immintrin.h>

namespace gal::toolbox::math
{
	inline namespace utils
	{
		[[nodiscard]] constexpr vector vector_i2f(const vector& v, std::uint32_t div_exponent) noexcept
		{
			gal::toolbox::utils::gal_assert(div_exponent < 32);

			// Convert to floats
			auto		  ret	  = _mm_cvtepi32_ps(_mm_castps_si128(v));
			// Convert div_exponent into 1.0f / (1 << div_exponent)
			std::uint32_t scalar1 = 0x3F800000U - (div_exponent << 23);
			// Splat the scalar value
			auto		  scalar2 = _mm_set1_epi32(static_cast<int>(scalar1));
			ret					  = _mm_mul_ps(ret, _mm_castsi128_ps(scalar2));
			return ret;
		}

		[[nodiscard]] constexpr vector vector_f2i(const vector& v, std::uint32_t mul_exponent) noexcept
		{
			gal::toolbox::utils::gal_assert(mul_exponent < 32);

			auto ret	  = _mm_set_ps1(static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent));
			ret			  = _mm_mul_ps(ret, v);
			// In case of positive overflow, detect it
			auto overflow = _mm_cmpgt_ps(ret, constants::g_vector_max_int.operator vector());
			// Float to int conversion
			auto result	  = _mm_cvttps_epi32(ret);
			// If there was positive overflow, set to 0x7FFFFFFF
			ret			  = _mm_and_ps(overflow, constants::g_vector_abs_mask.operator vector());
			overflow	  = _mm_andnot_ps(overflow, _mm_castsi128_ps(result));
			overflow	  = _mm_or_ps(overflow, ret);
			return overflow;
		}

		[[nodiscard]] constexpr vector vector_ui2f(const vector& v, std::uint32_t div_exponent) noexcept
		{
			gal::toolbox::utils::gal_assert(div_exponent < 32);

			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			auto mask			 = _mm_and_ps(v, constants::g_vector_neg_zero.operator vector());
			// Force all values positive
			auto ret			 = _mm_xor_ps(v, mask);
			// Convert to floats
			ret					 = _mm_cvtepi32_ps(_mm_castps_si128(ret));
			// Convert 0x80000000 -> 0xFFFFFFFF
			auto i				 = _mm_srai_epi32(_mm_castps_si128(mask), 31);
			// For only the ones that are too big, add the fixup
			mask				 = _mm_and_ps(_mm_castsi128_ps(i), constants::g_vector_fix_unsigned.operator vector());
			ret					 = _mm_add_ps(ret, mask);
			// Convert div_exponent into 1.0f / (1 << div_exponent)
			std::uint32_t scalar = 0x3F800000U - (div_exponent << 23);
			// Splat
			i					 = _mm_set1_epi32(static_cast<int>(scalar));
			ret					 = _mm_mul_ps(ret, _mm_castsi128_ps(i));
			return ret;
		}

		[[nodiscard]] constexpr vector vector_f2ui(const vector& v, std::uint32_t mul_exponent) noexcept
		{
			gal::toolbox::utils::gal_assert(mul_exponent < 32);

			auto ret										   = _mm_set_ps1(static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent));
			ret												   = _mm_mul_ps(ret, v);
			// Clamp to >=0
			ret												   = _mm_max_ps(ret, constants::g_vector_zero.operator vector());
			// Any numbers that are too big, set to 0xFFFFFFFFU
			auto overflow									   = _mm_cmpgt_ps(ret, constants::g_vector_max_uint.operator vector());
			auto value										   = constants::g_vector_unsigned_fix.operator vector();
			// Too large for a signed integer?
			auto										  mask = _mm_cmpge_ps(ret, value);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			value											   = _mm_and_ps(value, mask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			ret												   = _mm_sub_ps(ret, value);
			auto result										   = _mm_cvttps_epi32(ret);
			// Convert from signed to unsigned if greater than 0x80000000
			mask											   = _mm_and_ps(mask, constants::g_vector_neg_zero.operator vector());
			ret												   = _mm_xor_ps(_mm_castsi128_ps(result), mask);
			// On those that are too large, set to 0xFFFFFFFF
			ret												   = _mm_or_ps(ret, overflow);
			return ret;
		}

		template<one_tier_container_t T>
		constexpr vector vector_load(std::span<const float, T::size> source) noexcept
		{
			static_assert(T::size >= 2 && T::size <= 4);

			gal::toolbox::utils::gal_assert(source.data(), "invalid source pointer");

			if constexpr (T::size == 2)
			{
				return _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data())));
			}
			else if constexpr (T::size == 3)
			{
				auto xy = _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data())));
				auto z	= _mm_load_ss(static_cast<const float*>(source.data() + T::index_of_z));
				return _mm_movelh_ps(xy, z);
			}
			else
			{
				return _mm_castsi128_ps(_mm_loadu_si128(reinterpret_cast<const __m128i*>(source.data())));
			}
		}

		template<one_tier_container_t T>
		constexpr vector vector_load(const T& source) noexcept
		{
			static_assert(T::size >= 2 && T::size <= 4);

			gal::toolbox::utils::gal_assert(source.data(), "invalid source pointer");

			if constexpr (is_aligned_one_tier_container_v<T>)
			{
				gal::toolbox::utils::gal_assert(
						(reinterpret_cast<std::uintptr_t>(source.data) & 0xF) == 0,
						"invalid source data");
			}

			using value_type = typename T::value_type;

			vector v{};

			if constexpr (T::size == 2)
			{
				v = _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data)));
				if constexpr (std::is_floating_point_v<value_type>)
				{
					return v;
				}
				else if constexpr (std::is_signed_v<value_type>)
				{
					return _mm_cvtepi32_ps(_mm_castps_si128(v));
				}
			}
			else if constexpr (T::size == 3)
			{
				if constexpr (std::is_floating_point_v<value_type> && is_aligned_one_tier_container_v<T>)
				{
					// Reads an extra float which is zeroed
					return _mm_and_ps(_mm_load_ps(static_cast<const float*>(source.data)), g_vector_mask_xyz.operator vector());
				}
				else
				{
					auto xy = _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data)));
					auto z	= _mm_load_ss(static_cast<const float*>(source.data + T::index_of_z));
					if constexpr (std::is_floating_point_v<value_type>)
					{
						return _mm_movelh_ps(xy, z);
					}
					else
					{
						v = _mm_movelh_ps(xy, z);
						if constexpr (std::is_signed_v<value_type>)
						{
							return _mm_cvtepi32_ps(_mm_castps_si128(v));
						}
					}
				}
			}
			else
			{
				if constexpr (std::is_floating_point_v<value_type>)
				{
					return _mm_loadu_ps(static_cast<const float*>(source.data));
				}
				else
				{
					auto m128i = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source.data));
					if constexpr (std::is_signed_v<value_type>)
					{
						return _mm_cvtepi32_ps(m128i);
					}
					else
					{
						v = _mm_castsi128_ps(m128i);
					}
				}
			}

			// If value_type is unsigned

			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			auto mask = _mm_and_ps(v, constants::g_vector_neg_zero.operator vector());
			// Force all values positive
			auto ret  = _mm_xor_ps(v, mask);
			// Convert to floats
			ret		  = _mm_cvtepi32_ps(_mm_castps_si128(ret));
			// Convert 0x80000000 -> 0xFFFFFFFF
			auto i	  = _mm_srai_epi32(_mm_castps_si128(mask), 31);
			// For only the ones that are too big, add the fixup
			mask	  = _mm_and_ps(_mm_castsi128_ps(i), constants::g_vector_fix_unsigned.operator vector());
			ret		  = _mm_add_ps(ret, mask);
			return ret;
		}
	}// namespace utils
}// namespace gal::toolbox::math
