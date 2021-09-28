#pragma once

namespace gal::toolbox::math
{
	inline namespace utils
	{
		[[nodiscard]] constexpr vector vector_i2f(const vector& v, std::uint32_t div_exponent) noexcept
		{
			gal::toolbox::utils::gal_assert(div_exponent < 32);

			if (std::is_constant_evaluated())
			{
				auto   scalar = 1.f / static_cast<float>(static_cast<std::uint32_t>(1) << div_exponent);

				vector ret{};
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					auto i				= static_cast<std::int32_t>(v.m128_u32[index]);
					ret.m128_f32[index] = static_cast<float>(i) * scalar;
				}
				return ret;
			}

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

			if (std::is_constant_evaluated())
			{
				auto   scalar = static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent);

				vector ret{};
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					std::int32_t result;
					if (auto f = v.m128_f32[index] * scalar; f <= -(65536.0f * 32768.0f))
					{
						result = (-0x7FFFFFFF) - 1;
					}
					else if (f > (65536.0f * 32768.0f) - 128.0f)
					{
						result = 0x7FFFFFFF;
					}
					else
					{
						result = static_cast<std::int32_t>(f);
					}
					ret.m128_u32[index] = static_cast<std::uint32_t>(result);
				}
				return ret;
			}

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

			if (std::is_constant_evaluated())
			{
				auto   scalar = 1.f / static_cast<float>(static_cast<std::uint32_t>(1) << div_exponent);

				vector ret;
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					ret.m128_f32[index] = static_cast<float>(v.m128_u32[index]) * scalar;
				}
				return ret;
			}

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

			if (std::is_constant_evaluated())
			{
				auto   scalar = static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent);

				vector ret;
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					std::uint32_t result;
					if (auto f = v.m128_f32[index] * scalar; f <= 0.0f)
					{
						result = 0;
					}
					else if (f >= (65536.0f * 65536.0f))
					{
						result = 0xFFFFFFFFU;
					}
					else
					{
						result = static_cast<std::uint32_t>(f);
					}
					ret.m128_u32[index] = result;
				}
				return ret;
			}

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

		constexpr vector vector_load(const float* source) noexcept
		{
			if (std::is_constant_evaluated())
			{
				vector ret{};
				auto& [a, b, c, d] = ret.m128_f32;
				a				   = *source;
				b				   = 0.f;
				c				   = 0.f;
				d				   = 0.f;
				return ret;
			}

			return _mm_load_ss(source);
		}

		constexpr vector vector_load(const std::uint32_t* source) noexcept
		{
			if (std::is_constant_evaluated())
			{
				vector ret{};
				auto& [a, b, c, d] = ret.m128_u32;
				a				   = *source;
				b				   = 0;
				c				   = 0;
				d				   = 0;
				return ret;
			}

			return _mm_load_ss(reinterpret_cast<const float*>(source));
		}

		template<std::size_t Size>
		constexpr vector vector_load(std::span<const std::uint32_t, Size> source) noexcept
		{
			static_assert(Size >= 2 && Size <= 4);

			static_assert(uint4::index_of_x == 0);
			static_assert(uint4::index_of_y == 1);
			static_assert(uint4::index_of_z == 2);
			static_assert(uint4::index_of_w == 3);
			static_assert(uint4a::index_of_x == 0);
			static_assert(uint4a::index_of_y == 1);
			static_assert(uint4a::index_of_z == 2);
			static_assert(uint4a::index_of_w == 3);

			gal::toolbox::utils::gal_assert(source.data(), "invalid source pointer");

			if (std::is_constant_evaluated())
			{
				vector ret{};
				auto& [a, b, c, d] = ret.m128_u32;

				if constexpr (Size >= 2)
				{
					a = source[0];
					b = source[1];
				}
				if constexpr (Size >= 3)
				{
					c = source[2];
				}
				else
				{
					c = 0;
				}
				if constexpr (Size >= 4)
				{
					d = source[3];
				}
				else
				{
					d = 0;
				}
				return ret;
			}

			if constexpr (Size == 2)
			{
				return _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data())));
			}
			else if constexpr (Size == 3)
			{
				auto xy = _mm_castpd_ps(_mm_load_sd(static_cast<const double*>(source.data())));
				auto z	= _mm_load_ss(static_cast<const float*>(source.data() + uint3::index_of_z));
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

			static_assert(uint4::index_of_x == 0);
			static_assert(uint4::index_of_y == 1);
			static_assert(uint4::index_of_z == 2);
			static_assert(uint4::index_of_w == 3);
			static_assert(uint4a::index_of_x == 0);
			static_assert(uint4a::index_of_y == 1);
			static_assert(uint4a::index_of_z == 2);
			static_assert(uint4a::index_of_w == 3);

			gal::toolbox::utils::gal_assert(source.data(), "invalid source pointer");

			if constexpr (is_aligned_one_tier_container_v<T>)
			{
				gal::toolbox::utils::gal_assert(
						(reinterpret_cast<std::uintptr_t>(source.data) & 0xF) == 0,
						"invalid source data");
			}

			using value_type = typename T::value_type;

			if (std::is_constant_evaluated())
			{
				vector ret{};
				auto& [a, b, c, d] = ret.m128_u32;

				if constexpr (T::size == 2 >= 2)
				{
					a = source[0];
					b = source[1];
				}
				if constexpr (T::size == 2 >= 3)
				{
					c = source[2];
				}
				else
				{
					c = 0;
				}
				if constexpr (T::size == 2 >= 4)
				{
					d = source[3];
				}
				else
				{
					d = 0;
				}
				return ret;
			}

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
