namespace gal::toolbox::math
{
	inline namespace utils
	{
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_zero() noexcept
		{
			if (std::is_constant_evaluated())
			{
				return g_vector_zero.operator vector();
			}

			return _mm_setzero_ps();
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_set(T x, std::type_identity_t<T> y, std::type_identity_t<T> z, std::type_identity_t<T> w) noexcept
		{
			if (std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(x, y, z, w).operator vector();
			}

			if constexpr (std::is_same_v<float, T>)
			{
				return _mm_set_ps(w, z, y, x);
			}
			else
			{
				return _mm_castsi128_ps(_mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x)));
			}
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_replicate(T value) noexcept
		{
			if (std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(value, value, value, value).operator vector();
			}

			if constexpr (std::is_same_v<float, T>)
			{
				return _mm_set_ps1(value);
			}
			else
			{
				return _mm_castsi128_ps(_mm_set1_epi32(static_cast<int>(value)));
			}
		}

		template<basic_math_type_t T>
		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_replicate(const T* value) noexcept
		{
			if (std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(*value, *value, *value, *value).operator vector();
			}

			return _mm_load_ps1(reinterpret_cast<const float*>(value));
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_true() noexcept
		{
			if (std::is_constant_evaluated())
			{
				return g_vector_mask.operator vector();
			}

			return _mm_castsi128_ps(_mm_set1_epi32(-1));
		}

		constexpr vector GAL_MATH_CALLCONV vector_false() noexcept
		{
			return vector_zero();
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
		requires(Index >= 0 && Index <= 3) [[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat(vector_f v) noexcept
		{
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			if (std::is_constant_evaluated())
			{
				ActiveType value{};
				if constexpr (std::is_same_v<float, ActiveType>)
				{
					value = v.m128_f32[Index];
				}
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
				{
					value = v.m128_i32[Index];
				}
				else
				{
					value = v.m128_u32[Index];
				}
				return make<generic_vector<ActiveType>>(value, value, value, value).operator vector();
			}

			return _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index));
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_one() noexcept
		{
			return g_vector_one.operator vector();
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_infinity() noexcept
		{
			return g_vector_infinity.operator vector();
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_qnan() noexcept
		{
			return g_vector_nan.operator vector();
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_epsilon() noexcept
		{
			return g_vector_epsilon.operator vector();
		}

		[[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_splat_sign_mask() noexcept
		{
			return g_vector_neg_zero.operator vector();
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
		requires(Index >= 0 && Index <= 3) [[nodiscard]] constexpr ActiveType GAL_MATH_CALLCONV vector_get(vector_f v) noexcept
		{
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			if (std::is_constant_evaluated())
			{
				if constexpr (std::is_same_v<float, ActiveType>)
				{
					return v.m128_f32[Index];
				}
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
				{
					return v.m128_i32[Index];
				}
				else
				{
					return v.m128_u32[Index];
				}
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
		requires(Index >= 0 && Index <= 3) constexpr void GAL_MATH_CALLCONV vector_get(ActiveType* dest, vector_f v) noexcept
		{
			gal::toolbox::utils::gal_assert(dest, "invalid dest pointer");

			// todo: use proprietary instructions
			*dest = vector_get<Index, ActiveType>(v);

			// _mm_store_ss(reinterpret_cast<float*>(dest), _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index)));
		}

		template<std::size_t Index, basic_math_type_t ActiveType>
		requires(Index >= 0 && Index <= 3) [[nodiscard]] constexpr vector GAL_MATH_CALLCONV vector_set(vector_f v, ActiveType value) noexcept
		{
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			if (std::is_constant_evaluated())
			{
				auto ret = v;
				if constexpr (std::is_same_v<float, ActiveType>)
				{
					ret.m128_f32[Index] = value;
				}
				else if constexpr (std::is_same_v<std::int32_t, ActiveType>)
				{
					ret.m128_i32[Index] = value;
				}
				else
				{
					ret.m128_u32[Index] = value;
				}
				return ret;
			}

			// Convert input to vector
			vector temp{};
			if constexpr (std::is_same_v<float, ActiveType>)
			{
				temp = _mm_set_ss(value);
			}
			else
			{
				temp = _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int>(value)));
			}

			vector ret{};

			if constexpr(Index == 0)
			{
				ret = v;
			}
			else if constexpr(Index == 1)
			{
				// Swap y and x
				ret = _mm_permute_ps(v, _MM_SHUFFLE(3, 2, 0, 1));
			}
			else if constexpr(Index == 2)
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
			else if constexpr(Index == 2)
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
	}// namespace utils
}// namespace gal::toolbox::math
