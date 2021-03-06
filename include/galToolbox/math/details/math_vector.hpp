namespace gal::toolbox::math
{
	inline namespace utils
	{
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_zero() noexcept
		{
			if (std::is_constant_evaluated()) { return g_vector_zero.operator vector(); }

			return _mm_setzero_ps();
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_set(T x, std::type_identity_t<T> y, std::type_identity_t<T> z, std::type_identity_t<T> w) noexcept
		{
			if (std::is_constant_evaluated()) { return make<generic_vector<T>>(x, y, z, w).operator vector(); }

			if constexpr (std::is_same_v<float, T>) { return _mm_set_ps(w, z, y, x); }
			else { return _mm_castsi128_ps(_mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x))); }
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_replicate(T value) noexcept
		{
			if (std::is_constant_evaluated()) { return make<generic_vector<T>>(value, value, value, value).operator vector(); }

			if constexpr (std::is_same_v<float, T>) { return _mm_set_ps1(value); }
			else { return _mm_castsi128_ps(_mm_set1_epi32(static_cast<int>(value))); }
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_replicate(const T* value) noexcept
		{
			if (std::is_constant_evaluated()) { return make<generic_vector<T>>(*value, *value, *value, *value).operator vector(); }

			return _mm_load_ps1(reinterpret_cast<const float*>(value));
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_true() noexcept
		{
			if (std::is_constant_evaluated()) { return g_vector_mask.operator vector(); }

			return _mm_castsi128_ps(_mm_set1_epi32(-1));
		}

		constexpr vector GAL_MATH_CALLCONV vector_false() noexcept { return vector_zero(); }

		template<std::size_t Index, basic_math_type_t ActiveType>
			requires(Index >= 0 && Index <= 3)
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat(vector_f v) noexcept
		{
			if (std::is_constant_evaluated())
			{
				ActiveType value{};
				if constexpr (std::is_same_v<float, ActiveType>) { value = v.m128_f32[Index]; }
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>) { value = v.m128_i32[Index]; }
				else { value = v.m128_u32[Index]; }
				return make<generic_vector<ActiveType>>(value, value, value, value).operator vector();
			}

			return _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index));
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_one() noexcept { return g_vector_one.operator vector(); }

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_infinity() noexcept { return g_vector_infinity.operator vector(); }

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_qnan() noexcept { return g_vector_nan.operator vector(); }

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_epsilon() noexcept { return g_vector_epsilon.operator vector(); }

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_sign_mask() noexcept { return g_vector_neg_zero.operator vector(); }

		template<std::size_t Index, basic_math_type_t ActiveType>
			requires(Index >= 0 && Index <= 3)
		[[nodiscard]] constexpr ActiveType GAL_MATH_CALLCONV vector_get(vector_f v) noexcept
		{
			if (std::is_constant_evaluated())
			{
				if constexpr (std::is_same_v<float, ActiveType>) { return v.m128_f32[Index]; }
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>) { return v.m128_i32[Index]; }
				else { return v.m128_u32[Index]; }
			}

			// todo: https://godbolt.org/z/csxezd5c5
			// about my cpu :(
			// WSL gcc9 -> not support _mm_cvtss_i32 and _mm_cvtss_u32
			// but MSVC seem support _mm_cvtss_i32 (not support _mm_cvtss_u32 also)

			// auto ret = _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index));
			if constexpr (std::is_same_v<float, ActiveType>)
			{
				auto ret = _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index));
				return _mm_cvtss_f32(ret);
			}
			else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
			{
				// return _mm_cvtss_i32(ret);
				return static_cast<ActiveType>(_mm_extract_ps(v, Index));
			}
			else
			{
				// return _mm_cvtss_u32(ret);
				return static_cast<ActiveType>(_mm_extract_ps(v, Index));
			}
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
			requires(Index >= 0 && Index <= 3)
		GAL_ASSERT_CONSTEXPR void GAL_MATH_CALLCONV vector_get(ActiveType* dest, vector_f v) noexcept
		{
			gal_assert(dest, "invalid dest pointer");

			// todo: use proprietary instructions
			*dest = vector_get<Index, ActiveType>(v);

			//			if (std::is_constant_evaluated())
			//			{
			//				if constexpr (std::is_same_v<float, ActiveType>)
			//				{
			//					*dest = v.m128_f32[Index];
			//				}
			//				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
			//				{
			//					*dest = v.m128_i32[Index];
			//				}
			//				else
			//				{
			//					*dest = v.m128_u32[Index];
			//				}
			//				return;
			//			}
			//
			//			_mm_store_ss(reinterpret_cast<float*>(dest), _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index)));
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
			requires(Index >= 0 && Index <= 3)
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_set(vector_f v, ActiveType value) noexcept
		{
			if (std::is_constant_evaluated())
			{
				auto ret = v;
				if constexpr (std::is_same_v<float, ActiveType>) { ret.m128_f32[Index] = value; }
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>) { ret.m128_i32[Index] = value; }
				else { ret.m128_u32[Index] = value; }
				return ret;
			}

			// Convert input to vector
			vector temp{};
			if constexpr (std::is_same_v<float, ActiveType>) { temp = _mm_set_ss(value); }
			else { temp = _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int>(value))); }

			vector ret{};

			if constexpr (Index == 0) { ret = v; }
			else if constexpr (Index == 1)
			{
				// Swap y and x
				ret = _mm_permute_ps(v, _MM_SHUFFLE(3, 2, 0, 1));
			}
			else if constexpr (Index == 2)
			{
				// Swap z and x
				ret = _mm_permute_ps(v, _MM_SHUFFLE(3, 0, 1, 2));
			}
			else
			{
				// Swap w and x
				ret = _mm_permute_ps(v, _MM_SHUFFLE(0, 2, 1, 3));
			}

			// Replace the component
			ret = _mm_move_ss(ret, temp);

			if constexpr (Index == 0)
			{
				// do nothing
			}
			else if constexpr (Index == 1)
			{
				// Swap y and x again
				ret = _mm_permute_ps(ret, _MM_SHUFFLE(3, 2, 0, 1));
			}
			else if constexpr (Index == 2)
			{
				// Swap z and x again
				ret = _mm_permute_ps(ret, _MM_SHUFFLE(3, 0, 1, 2));
			}
			else
			{
				// Swap w and x again
				ret = _mm_permute_ps(ret, _MM_SHUFFLE(0, 2, 1, 3));
			}

			return ret;
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
			requires(Index >= 0 && Index <= 3)
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_set(vector_f v, const ActiveType* value) noexcept
		{
			gal_assert(value, "invalid value pointer");

			// todo: use proprietary instructions
			return vector_set<Index, ActiveType>(v, *value);

			//			if (std::is_constant_evaluated())
			//			{
			//				auto ret = v;
			//				if constexpr (std::is_same_v<float, ActiveType>)
			//				{
			//					ret.m128_f32[Index] = *value;
			//				}
			//				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
			//				{
			//					ret.m128_i32[Index] = *value;
			//				}
			//				else
			//				{
			//					ret.m128_u32[Index] = *value;
			//				}
			//				return ret;
			//			}
			//
			//			// Convert input to vector
			//			vector temp = _mm_load_ss(reinterpret_cast<const float*>(value));
			//
			//			vector ret{};
			//
			//			if constexpr(Index == 0)
			//			{
			//				ret = v;
			//			}
			//			else if constexpr(Index == 1)
			//			{
			//				// Swap y and x
			//				ret = _mm_permute_ps(v, _MM_SHUFFLE(3, 2, 0, 1));
			//			}
			//			else if constexpr(Index == 2)
			//			{
			//				// Swap z and x
			//				ret = _mm_permute_ps(v, _MM_SHUFFLE(3, 0, 1, 2));
			//			}
			//			else
			//			{
			//				// Swap w and x
			//				ret = _mm_permute_ps(v, _MM_SHUFFLE(0, 2, 1, 3));
			//			}
			//
			//			// Replace the component
			//			ret = _mm_move_ss(ret, temp);
			//
			//			if constexpr (Index == 0)
			//			{
			//				// do nothing
			//			}
			//			else if constexpr (Index == 1)
			//			{
			//				// Swap y and x again
			//				ret = _mm_permute_ps(ret, _MM_SHUFFLE(3, 2, 0, 1));
			//			}
			//			else if constexpr(Index == 2)
			//			{
			//				// Swap z and x again
			//				ret = _mm_permute_ps(ret, _MM_SHUFFLE(3, 0, 1, 2));
			//			}
			//			else
			//			{
			//				// Swap w and x again
			//				ret = _mm_permute_ps(ret, _MM_SHUFFLE(0, 2, 1, 3));
			//			}
			//
			//			return ret;
		}

		template<basic_math_type_t ActiveType>
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_swizzle(vector_f v, std::uint32_t index0, std::uint32_t index1, std::uint32_t index2, std::uint32_t index3) noexcept
		{
			gal_assert((index0 < 4) && (index1 < 4) && (index2 < 4) && (index3 < 4));

			if (std::is_constant_evaluated())
			{
				const ActiveType* value;

				if constexpr (std::is_same_v<float, ActiveType>) { value = v.m128_f32; }
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>) { value = v.m128_i32; }
				else { value = v.m128_u32; }

				return make<generic_vector<ActiveType>>(value[index0], value[index1], value[index2], value[index3]).operator vector();
			}

			std::uint32_t control_elements[4]{index0, index1, index2, index3};
			return _mm_permutevar_ps(v, _mm_loadu_si128(reinterpret_cast<const __m128i*>(control_elements)));
		}

		template<basic_math_type_t ActiveType>
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_permute(vector_f v1, vector_f v2, std::uint32_t index0, std::uint32_t index1, std::uint32_t index2, std::uint32_t index3) noexcept
		{
			gal_assert((index0 < 8) && (index1 < 8) && (index2 < 8) && (index3 < 8));

			if (std::is_constant_evaluated())
			{
				vector ret{};

				const ActiveType* p[2];
				ActiveType* pr;

				if constexpr (std::is_same_v<float, ActiveType>)
				{
					p[0] = v1.m128_f32;
					p[1] = v2.m128_f32;
					pr = ret.m128_f32;
				}
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
				{
					p[0] = v1.m128_i32;
					p[1] = v2.m128_i32;
					pr = ret.m128_i32;
				}
				else
				{
					p[0] = v1.m128_u32;
					p[1] = v2.m128_u32;
					pr = ret.m128_u32;
				}

				pr[0] = p[index0 >> 2][index0 & 3];
				pr[1] = p[index1 >> 2][index1 & 3];
				pr[2] = p[index2 >> 2][index2 & 3];
				pr[3] = p[index3 >> 2][index3 & 3];

				return ret;
			}

			constexpr u32_vector three{3, 3, 3, 3};
			alignas(math_type_alignment) std::uint32_t control_elements[4]{index0, index1, index2, index3};

			auto control = _mm_load_si128(reinterpret_cast<const __m128i*>(control_elements));
			auto select = _mm_castsi128_ps(_mm_cmpgt_epi32(control, three.operator __m128i()));
			control = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(control), three.operator vector()));

			auto shuffled1 = _mm_permutevar_ps(v1, control);
			auto shuffled2 = _mm_permutevar_ps(v2, control);

			auto masked1 = _mm_andnot_ps(select, shuffled1);
			auto masked2 = _mm_and_ps(select, shuffled2);

			return _mm_or_ps(masked1, masked2);
		}

		template<basic_math_type_t ActiveType>
		[[nodiscard]] GAL_ASSERT_CONSTEXPR vector GAL_MATH_CALLCONV vector_select_control(std::uint32_t index0, std::uint32_t index1, std::uint32_t index2, std::uint32_t index3) noexcept
		{
			gal_assert((index0 < 2) && (index1 < 2) && (index2 < 2) && (index3 < 2));

			if (std::is_constant_evaluated())
			{
				constexpr std::uint32_t control_elements[]{select_0, select_1};

				vector ret{};
				ActiveType* p;

				if constexpr (std::is_same_v<float, ActiveType>) { p = ret.m128_f32; }
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>) { p = ret.m128_i32; }
				else { p = ret.m128_u32; }

				p[0] = static_cast<ActiveType>(control_elements[index0]);
				p[1] = static_cast<ActiveType>(control_elements[index1]);
				p[2] = static_cast<ActiveType>(control_elements[index2]);
				p[3] = static_cast<ActiveType>(control_elements[index3]);

				return ret;
			}

			// x=index0,y=index1,z=index2,w=index3
			auto ret = _mm_set_epi32(static_cast<int>(index3), static_cast<int>(index2), static_cast<int>(index1), static_cast<int>(index0));
			// Any non-zero entries become 0xFFFFFFFF else 0
			ret = _mm_cmpgt_epi32(ret, g_vector_zero.operator __m128i());
			return _mm_castsi128_ps(ret);
		}

		template<basic_math_type_t ActiveType>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_select(vector_f v1, vector_f v2, vector_f control) noexcept
		{
			if (std::is_constant_evaluated())
			{
				vector ret{};

				const ActiveType* p[2];
				const ActiveType* c;
				ActiveType* pr;

				if constexpr (std::is_same_v<float, ActiveType>)
				{
					p[0] = v1.m128_f32;
					p[1] = v2.m128_f32;
					c = control.m128_f32;
					pr = ret.m128_f32;
				}
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
				{
					p[0] = v1.m128_i32;
					p[1] = v2.m128_i32;
					c = control.m128_i32;
					pr = ret.m128_i32;
				}
				else
				{
					p[0] = v1.m128_u32;
					p[1] = v2.m128_u32;
					c = control.m128_u32;
					pr = ret.m128_u32;
				}

				pr[0] = c[0] ? p[1][0] : p[0][0];
				pr[1] = c[1] ? p[1][1] : p[0][1];
				pr[2] = c[2] ? p[1][2] : p[0][2];
				pr[3] = c[3] ? p[1][3] : p[0][3];

				return ret;
			}

			return _mm_or_ps(_mm_andnot_ps(control, v1), _mm_and_ps(v2, control));
		}
	}// namespace utils
}    // namespace gal::toolbox::math
