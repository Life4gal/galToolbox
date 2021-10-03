namespace gal::toolbox::math
{
	inline namespace utils
	{
		constexpr vector vector_zero() noexcept
		{
			if(std::is_constant_evaluated())
			{
				return g_vector_zero.operator vector();
			}

			return _mm_setzero_ps();
		}

		template<basic_math_type_t T>
		constexpr vector vector_set(T x, std::type_identity_t<T> y, std::type_identity_t<T> z, std::type_identity_t<T> w) noexcept
		{
			if(std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(x, y, z, w).operator vector();
			}

			if constexpr(std::is_same_v<float, T>)
			{
				return _mm_set_ps(w, z, y, x);
			}
			else
			{
				return _mm_castsi128_ps(_mm_set_epi32(static_cast<int>(w), static_cast<int>(z), static_cast<int>(y), static_cast<int>(x)));
			}
		}

		template<basic_math_type_t T>
		constexpr vector vector_replicate(T value) noexcept
		{
			if(std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(value, value, value, value).operator vector();
			}

			if constexpr(std::is_same_v<float, T>)
			{
				return _mm_set_ps1(value);
			}
			else
			{
				return _mm_castsi128_ps(_mm_set1_epi32(static_cast<int>(value)));
			}
		}

		template<basic_math_type_t T>
		constexpr vector vector_replicate(const T* value) noexcept
		{
			if(std::is_constant_evaluated())
			{
				return make<generic_vector<T>>(*value, *value, *value, *value).operator vector();
			}

			return _mm_load_ps1(reinterpret_cast<const float*>(value));
		}

		constexpr vector vector_true() noexcept
		{
			if(std::is_constant_evaluated())
			{
				return g_vector_mask.operator vector();
			}

			return _mm_castsi128_ps(_mm_set1_epi32(-1));
		}

		constexpr vector vector_false() noexcept
		{
			return vector_zero();
		}

		template<std::size_t Index, typename Active>
		requires (Index >= 0 && Index <= 3) && basic_math_type_t<Active>
		constexpr vector vector_splat(const vector& v) noexcept
		{
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_x == 0);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_y == 1);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_z == 2);
			static_assert(generic_one_tier_container<std::uint32_t, 4>::index_of_w == 3);

			if(std::is_constant_evaluated())
			{
				Active value{};
				if constexpr (std::is_same_v<float, Active>)
				{
					value = v.m128_f32[Index];
				}
				else if constexpr (std::is_same_v<std::int32_t, Active>)
				{
					value = v.m128_i32[Index];
				}
				else
				{
					value = v.m128_u32[Index];
				}
				return make<generic_vector<Active>>(value, value, value, value).operator vector();
			}

			return _mm_permute_ps(v, _MM_SHUFFLE(Index, Index, Index, Index));
		}

		constexpr vector vector_splat_one() noexcept
		{
			return g_vector_one.operator vector();
		}

		constexpr vector vector_splat_infinity() noexcept
		{
			return g_vector_infinity.operator vector();
		}

		constexpr vector vector_splat_qnan() noexcept
		{
			return g_vector_nan.operator vector();
		}

		constexpr vector vector_splat_epsilon() noexcept
		{
			return g_vector_epsilon.operator vector();
		}

		constexpr vector vector_splat_sign_mask() noexcept
		{
			return g_vector_neg_zero.operator vector();
		}
	}
}
