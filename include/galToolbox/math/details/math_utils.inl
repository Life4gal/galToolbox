namespace gal::toolbox::math
{
	inline namespace utils
	{
		[[nodiscard]] constexpr float degrees2radians(float degrees) noexcept { return degrees * (pi / 180.f); }

		[[nodiscard]] constexpr float radians2degrees(float radians) noexcept { return radians * (180.f / pi); }

		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_i2f(vector_f v, const std::uint32_t div_exponent) noexcept
		{
			gal_assert(div_exponent < 32);

			if (std::is_constant_evaluated())
			{
				const auto scalar = 1.f / static_cast<float>(static_cast<std::uint32_t>(1) << div_exponent);

				vector ret{};
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					auto i = static_cast<std::int32_t>(v.m128_u32[index]);
					ret.m128_f32[index] = static_cast<float>(i) * scalar;
				}
				return ret;
			}

			// Convert to floats
			auto ret = _mm_cvtepi32_ps(_mm_castps_si128(v));
			// Convert div_exponent into 1.0f / (1 << div_exponent)
			std::uint32_t scalar1 = 0x3F800000U - (div_exponent << 23);
			// Splat the scalar value
			auto scalar2 = _mm_set1_epi32(static_cast<int>(scalar1));
			ret = _mm_mul_ps(ret, _mm_castsi128_ps(scalar2));
			return ret;
		}

		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_f2i(vector_f v, const std::uint32_t mul_exponent) noexcept
		{
			gal_assert(mul_exponent < 32);

			if (std::is_constant_evaluated())
			{
				const auto scalar = static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent);

				vector ret{};
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					std::int32_t result;
					if (const auto f = v.m128_f32[index] * scalar; f <= -(65536.0f * 32768.0f)) { result = (-0x7FFFFFFF) - 1; }
					else if (f > (65536.0f * 32768.0f) - 128.0f) { result = 0x7FFFFFFF; }
					else { result = static_cast<std::int32_t>(f); }
					ret.m128_u32[index] = static_cast<std::uint32_t>(result);
				}
				return ret;
			}

			auto ret = _mm_set_ps1(static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent));
			ret = _mm_mul_ps(ret, v);
			// In case of positive overflow, detect it
			auto overflow = _mm_cmpgt_ps(ret, g_vector_max_int.operator vector());
			// Float to int conversion
			auto result = _mm_cvttps_epi32(ret);
			// If there was positive overflow, set to 0x7FFFFFFF
			ret = _mm_and_ps(overflow, g_vector_abs_mask.operator vector());
			overflow = _mm_andnot_ps(overflow, _mm_castsi128_ps(result));
			overflow = _mm_or_ps(overflow, ret);
			return overflow;
		}

		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_ui2f(vector_f v, std::uint32_t div_exponent) noexcept
		{
			gal_assert(div_exponent < 32);

			if (std::is_constant_evaluated())
			{
				auto scalar = 1.f / static_cast<float>(static_cast<std::uint32_t>(1) << div_exponent);

				vector ret;
				for (std::uint32_t index = 0; index < 4; ++index) { ret.m128_f32[index] = static_cast<float>(v.m128_u32[index]) * scalar; }
				return ret;
			}

			// For the values that are higher than 0x7FFFFFFF, a fixup is needed
			// Determine which ones need the fix.
			auto mask = _mm_and_ps(v, g_vector_neg_zero.operator vector());
			// Force all values positive
			auto ret = _mm_xor_ps(v, mask);
			// Convert to floats
			ret = _mm_cvtepi32_ps(_mm_castps_si128(ret));
			// Convert 0x80000000 -> 0xFFFFFFFF
			auto i = _mm_srai_epi32(_mm_castps_si128(mask), 31);
			// For only the ones that are too big, add the fixup
			mask = _mm_and_ps(_mm_castsi128_ps(i), g_vector_fix_unsigned.operator vector());
			ret = _mm_add_ps(ret, mask);
			// Convert div_exponent into 1.0f / (1 << div_exponent)
			std::uint32_t scalar = 0x3F800000U - (div_exponent << 23);
			// Splat
			i = _mm_set1_epi32(static_cast<int>(scalar));
			ret = _mm_mul_ps(ret, _mm_castsi128_ps(i));
			return ret;
		}

		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_f2ui(vector_f v, std::uint32_t mul_exponent) noexcept
		{
			gal_assert(mul_exponent < 32);

			if (std::is_constant_evaluated())
			{
				auto scalar = static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent);

				vector ret;
				for (std::uint32_t index = 0; index < 4; ++index)
				{
					std::uint32_t result;
					if (auto f = v.m128_f32[index] * scalar; f <= 0.0f) { result = 0; }
					else if (f >= (65536.0f * 65536.0f)) { result = 0xFFFFFFFFU; }
					else { result = static_cast<std::uint32_t>(f); }
					ret.m128_u32[index] = result;
				}
				return ret;
			}

			auto ret = _mm_set_ps1(static_cast<float>(static_cast<std::uint32_t>(1) << mul_exponent));
			ret = _mm_mul_ps(ret, v);
			// Clamp to >=0
			ret = _mm_max_ps(ret, g_vector_zero.operator vector());
			// Any numbers that are too big, set to 0xFFFFFFFFU
			auto overflow = _mm_cmpgt_ps(ret, g_vector_max_uint.operator vector());
			auto value = g_vector_unsigned_fix.operator vector();
			// Too large for a signed integer?
			auto mask = _mm_cmpge_ps(ret, value);
			// Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
			value = _mm_and_ps(value, mask);
			// Perform fixup only on numbers too large (Keeps low bit precision)
			ret = _mm_sub_ps(ret, value);
			auto result = _mm_cvttps_epi32(ret);
			// Convert from signed to unsigned if greater than 0x80000000
			mask = _mm_and_ps(mask, g_vector_neg_zero.operator vector());
			ret = _mm_xor_ps(_mm_castsi128_ps(result), mask);
			// On those that are too large, set to 0xFFFFFFFF
			ret = _mm_or_ps(ret, overflow);
			return ret;
		}

		template<std::size_t Size, basic_math_type_t ValueType, typename T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV consteval_vector_load(T source) noexcept
		{
			static_assert(Size >= 1 && Size <= 4);

			vector ret{};

			ValueType* data;

			if constexpr (std::is_same_v<float, ValueType>) { data = ret.m128_f32; }
			else if constexpr (std::is_same_v<std::int32_t, ValueType>) { data = ret.m128_i32; }
			else { data = ret.m128_u32; }

			data[0] = source[0];

			if constexpr (Size >= 2) { data[1] = source[1]; }
			else { data[1] = ValueType{0}; }
			if constexpr (Size >= 3) { data[2] = source[2]; }
			else { data[2] = ValueType{0}; }
			if constexpr (Size >= 4) { data[3] = source[3]; }
			else { data[3] = ValueType{0}; }
			return ret;
		}

		template<std::size_t Size, basic_math_type_t T>
			requires(Size >= 1 && Size <= 4)
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_load(const T* source) noexcept
		{
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			gal_assert(source, "invalid source pointer");

			if (std::is_constant_evaluated()) { return consteval_vector_load<Size, T, const T*>(source); }

			if constexpr (Size == 1) { return _mm_load_ss(reinterpret_cast<const float*>(source)); }
			else if constexpr (Size == 2) { return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source))); }
			else if constexpr (Size == 3)
			{
				auto xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source)));
				auto z = _mm_load_ss(reinterpret_cast<const float*>(source + uint3::index_of_z));
				return _mm_movelh_ps(xy, z);
			}
			else { return _mm_castsi128_ps(_mm_loadu_si128(reinterpret_cast<const __m128i*>(source))); }
		}

		template<one_tier_container_ignore_aligned_t T>
			requires(T::size >= 2 && T::size <= 4)
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_load(const T& source) noexcept
		{
			constexpr auto size = T::size;
			using value_type = typename T::value_type;

			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			if constexpr (is_aligned_one_tier_container_v<T>)
			{
				if (!std::is_constant_evaluated())
				{
					// we cannot use reinterpret_cast in consteval
					gal_assert(
							(reinterpret_cast<std::uintptr_t>(&source) & 0xF) == 0,
							"invalid source data");
				}
			}

			if (std::is_constant_evaluated()) { return consteval_vector_load<size, value_type, const T&>(source); }

			vector v{};

			if constexpr (size == 2)
			{
				v = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source.data)));
				if constexpr (std::is_floating_point_v<value_type>) { return v; }
				else if constexpr (std::is_signed_v<value_type>) { return _mm_cvtepi32_ps(_mm_castps_si128(v)); }
			}
			else if constexpr (size == 3)
			{
				if constexpr (std::is_floating_point_v<value_type> && is_aligned_one_tier_container_v<T>)
				{
					// Reads an extra float which is zeroed
					return _mm_and_ps(_mm_load_ps(reinterpret_cast<const float*>(source.data)), g_vector_mask_xyz.operator vector());
				}
				else
				{
					auto xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source.data)));
					auto z = _mm_load_ss(reinterpret_cast<const float*>(source.data + T::index_of_z));
					if constexpr (std::is_floating_point_v<value_type>) { return _mm_movelh_ps(xy, z); }
					else
					{
						v = _mm_movelh_ps(xy, z);
						if constexpr (std::is_signed_v<value_type>) { return _mm_cvtepi32_ps(_mm_castps_si128(v)); }
					}
				}
			}
			else
			{
				if constexpr (std::is_floating_point_v<value_type>) { return _mm_loadu_ps(reinterpret_cast<const float*>(source.data)); }
				else
				{
					auto m128i = _mm_loadu_si128(reinterpret_cast<const __m128i*>(source.data));
					if constexpr (std::is_signed_v<value_type>) { return _mm_cvtepi32_ps(m128i); }
					else { v = _mm_castsi128_ps(m128i); }
				}
			}

			// If value_type is unsigned
			if constexpr (std::is_unsigned_v<value_type>)
			{
				/**
				 * note:
				 *  The `if constexpr` here is necessary, otherwise if the value_type is not unsigned (even if they have been returned in the above branch),
				 *  the following code still exists (even if they are not reachable), and the warning generated by this will be regarded as an error.
				 */

				// For the values that are higher than 0x7FFFFFFF, a fixup is needed
				// Determine which ones need the fix.
				auto mask = _mm_and_ps(v, g_vector_neg_zero.operator vector());
				// Force all values positive
				auto ret = _mm_xor_ps(v, mask);
				// Convert to floats
				ret = _mm_cvtepi32_ps(_mm_castps_si128(ret));
				// Convert 0x80000000 -> 0xFFFFFFFF
				auto i = _mm_srai_epi32(_mm_castps_si128(mask), 31);
				// For only the ones that are too big, add the fixup
				mask = _mm_and_ps(_mm_castsi128_ps(i), g_vector_fix_unsigned.operator vector());
				ret = _mm_add_ps(ret, mask);
				return ret;
			}
		}

		template<std::size_t FirstSize, std::size_t SecondSize, basic_math_type_t ValueType, typename T>
		[[nodiscard]] constexpr matrix GAL_MATH_CALLCONV consteval_matrix_load(T source) noexcept
		{
			static_assert(FirstSize == 3 || FirstSize == 4);
			static_assert(SecondSize == 3 || SecondSize == 4);

			matrix ret{};

			ValueType* data0;
			ValueType* data1;
			ValueType* data2;
			ValueType* data3;

			if constexpr (std::is_same_v<float, ValueType>)
			{
				data0 = ret.data[0].m128_f32;
				data1 = ret.data[1].m128_f32;
				data2 = ret.data[2].m128_f32;
				data3 = ret.data[3].m128_f32;
			}
			else if constexpr (std::is_same_v<std::int32_t, ValueType>)
			{
				data0 = ret.data[0].m128_i32;
				data1 = ret.data[1].m128_i32;
				data2 = ret.data[2].m128_i32;
				data3 = ret.data[3].m128_i32;
			}
			else
			{
				data0 = ret.data[0].m128_u32;
				data1 = ret.data[1].m128_u32;
				data2 = ret.data[2].m128_u32;
				data3 = ret.data[3].m128_u32;
			}

			data0[0] = source(0, 0);
			data0[1] = source(0, 1);
			data0[2] = source(0, 2);

			data1[0] = source(1, 0);
			data1[1] = source(1, 1);
			data1[2] = source(1, 2);

			data2[0] = source(2, 0);
			data2[1] = source(2, 1);
			data2[2] = source(2, 2);

			if constexpr (SecondSize == 4)
			{
				data0[3] = source(0, 3);
				data1[3] = source(1, 3);
				data2[3] = source(2, 3);
			}
			else
			{
				data0[3] = ValueType{0};
				data1[3] = ValueType{0};
				data2[3] = ValueType{0};
			}

			if constexpr (FirstSize == 4)
			{
				data3[0] = source(3, 0);
				data3[1] = source(3, 1);
				data3[2] = source(3, 2);
			}
			else
			{
				data3[0] = ValueType{0};
				data3[1] = ValueType{0};
				data3[2] = ValueType{0};
			}

			if constexpr (FirstSize == 4 && SecondSize == 4) { data3[3] = source(3, 3); }
			else { data3[3] = ValueType{1}; }

			return ret;
		}

		template<two_tier_container_ignore_aligned_t T>
			requires(T::first_size == 3 || T::first_size == 4) && (T::second_size == 3 || T::second_size == 4)
		[[nodiscard]] GAL_ASSERT_CONSTEXPR matrix GAL_MATH_CALLCONV matrix_load(const T& source) noexcept
		{
			constexpr auto first_size = T::first_size;
			constexpr auto second_size = T::second_size;
			constexpr auto size = T::size;

			using value_type = typename T::value_type;

			if constexpr (is_aligned_two_tier_container_v<T>)
			{
				if (!std::is_constant_evaluated())
				{
					// we cannot use reinterpret_cast in consteval
					gal_assert(
							(reinterpret_cast<std::uintptr_t>(&source) & 0xF) == 0,
							"invalid source data");
				}
			}

			if (std::is_constant_evaluated()) { return consteval_matrix_load<first_size, second_size, value_type, const T&>(source); }

			if constexpr (size == 9)
			{
				// 3 * 3 matrix
				auto z = _mm_setzero_ps();

				auto v1 = _mm_loadu_ps(static_cast<const float*>(&source.data[0][0]));
				auto v2 = _mm_loadu_ps(static_cast<const float*>(&source.data[1][1]));
				auto v3 = _mm_load_ss(static_cast<const float*>(&source.data[2][2]));

				auto t1 = _mm_unpackhi_ps(v1, z);
				auto t2 = _mm_unpacklo_ps(v2, z);
				auto t3 = _mm_shuffle_ps(v3, t2, _MM_SHUFFLE(0, 1, 0, 0));
				auto t4 = _mm_movehl_ps(t2, t3);
				auto t5 = _mm_movehl_ps(z, t1);

				matrix ret{};
				ret.data[0] = _mm_movelh_ps(v1, t1);
				ret.data[1] = _mm_add_ps(t4, t5);
				ret.data[2] = _mm_shuffle_ps(v2, v3, _MM_SHUFFLE(1, 0, 3, 2));
				ret.data[3] = g_vector_identity_r3.operator vector();
				return ret;
			}
			else if constexpr (size == 12)
			{
				// 3 * 4 matrix / 4 * 3 matrix

				if constexpr (first_size == 4)
				{
					// 4 * 3 matrix
					vector v1{};
					vector v2{};
					vector v3{};
					vector v4{};

					if constexpr (is_aligned_one_tier_container_v<T>)
					{
						// Use aligned load instructions to
						// load the 12 floats
						// v1 = x1,y1,z1,x2
						v1 = _mm_load_ps(static_cast<const float*>(&source.data[0][0]));
						// v2 = y2,z2,x3,y3
						v2 = _mm_load_ps(static_cast<const float*>(&source.data[1][1]));
						// v4 = z3,x4,y4,z4
						v4 = _mm_load_ps(static_cast<const float*>(&source.data[2][2]));
					}
					else
					{
						// Use unaligned load instructions to
						// load the 12 floats
						// v1 = x1,y1,z1,x2
						v1 = _mm_loadu_ps(static_cast<const float*>(&source.data[0][0]));
						// v2 = y2,z2,x3,y3
						v2 = _mm_loadu_ps(static_cast<const float*>(&source.data[1][1]));
						// v4 = z3,x4,y4,z4
						v4 = _mm_loadu_ps(static_cast<const float*>(&source.data[2][2]));
					}
					// v3 = x3,y3,z3,z3
					v3 = _mm_shuffle_ps(v2, v4, _MM_SHUFFLE(0, 0, 3, 2));

					// v2 = y2,z2,x2,x2
					v2 = _mm_shuffle_ps(v2, v1, _MM_SHUFFLE(3, 3, 1, 0));
					// v2 = x2,y2,z2,z2
					v2 = _mm_permute_ps(v2, _MM_SHUFFLE(1, 1, 0, 2));
					// v1 = x1,y1,z1,0
					v1 = _mm_and_ps(v1, g_vector_mask_xyz.operator vector());
					// v2 = x2,y2,z2,0
					v2 = _mm_and_ps(v2, g_vector_mask_xyz.operator vector());
					// v3 = x3,y3,z3,0
					v3 = _mm_and_ps(v3, g_vector_mask_xyz.operator vector());

					// v4i = x4,y4,z4,0
					auto v4i = _mm_srli_si128(_mm_castps_si128(v4), 32 / 8);
					// v4i = x4,y4,z4,1.0f
					v4i = _mm_or_si128(v4i, g_vector_identity_r3.operator __m128i());
					return {v1, v2, v3, _mm_castsi128_ps(v4i)};
				}
				else
				{
					// 3 * 4 matrix
					matrix m{};
					auto& [row0, row1, row2, row3] = m.data;

					row0 = _mm_loadu_ps(static_cast<const float*>(&source.data[0][0]));
					row1 = _mm_loadu_ps(static_cast<const float*>(&source.data[1][0]));
					row2 = _mm_loadu_ps(static_cast<const float*>(&source.data[2][0]));
					row3 = g_vector_identity_r3.operator vector();

					// x.x,x.y,y.x,y.y
					auto v1 = _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(1, 0, 1, 0));
					// x.z,x.w,y.z,y.w
					auto v3 = _mm_shuffle_ps(row0, row1, _MM_SHUFFLE(3, 2, 3, 2));
					// z.x,z.y,w.x,w.y
					auto v2 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(1, 0, 1, 0));
					// z.z,z.w,w.z,w.w
					auto v4 = _mm_shuffle_ps(row2, row3, _MM_SHUFFLE(3, 2, 3, 2));

					matrix ret{};
					auto& [r0, r1, r2, r3] = ret.data;

					// x.x,y.x,z.x,w.x
					r0 = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(2, 0, 2, 0));
					// x.y,y.y,z.y,w.y
					r1 = _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(3, 1, 3, 1));
					// x.z,y.z,z.z,w.z
					r2 = _mm_shuffle_ps(v3, v4, _MM_SHUFFLE(2, 0, 2, 0));
					// x.w,y.w,z.w,w.w
					r3 = _mm_shuffle_ps(v3, v4, _MM_SHUFFLE(3, 1, 3, 1));

					return ret;
				}
			}
			else
			{
				// 4 * 4 matrix
				matrix ret{};
				auto& [r0, r1, r2, r3] = ret.data;

				if constexpr (is_aligned_one_tier_container_v<T>)
				{
					r0 = _mm_load_ps(static_cast<const float*>(&source.data[0][0]));
					r1 = _mm_load_ps(static_cast<const float*>(&source.data[1][0]));
					r2 = _mm_load_ps(static_cast<const float*>(&source.data[2][0]));
					r3 = _mm_load_ps(static_cast<const float*>(&source.data[3][0]));
				}
				else
				{
					r0 = _mm_loadu_ps(static_cast<const float*>(&source.data[0][0]));
					r1 = _mm_loadu_ps(static_cast<const float*>(&source.data[1][0]));
					r2 = _mm_loadu_ps(static_cast<const float*>(&source.data[2][0]));
					r3 = _mm_loadu_ps(static_cast<const float*>(&source.data[3][0]));
				}

				return ret;
			}
		}

		template<std::size_t Size, basic_math_type_t T>
			requires(Size >= 1 && Size <= 4)
		constexpr void GAL_MATH_CALLCONV vector_store(T* dest, vector_f source) noexcept
		{
			// todo
		}

		template<one_tier_container_ignore_aligned_t T>
			requires(T::size >= 2 && T::size <= 4)
		constexpr void GAL_MATH_CALLCONV vector_store(T& dest, vector_f source) noexcept
		{
			// todo
		}

		template<two_tier_container_ignore_aligned_t T>
			requires(T::first_size == 3 || T::first_size == 4) && (T::second_size == 3 || T::secondt_size == 4)
		constexpr void GAL_MATH_CALLCONV matrix_store(T& dest, matrix_f source) noexcept
		{
			// todo
		}
	}// namespace utils
}    // namespace gal::toolbox::math
