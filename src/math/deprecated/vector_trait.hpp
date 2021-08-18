#pragma once

#include <type_traits>
#include <cstdint>

namespace gal::test
{
	struct vector_trait
	{
	private:
		template <typename Vec, typename Func, std::size_t...I, typename... Args>
		constexpr static void binary_invoke_impl(Vec& vec, Func&& func, std::index_sequence<I...>, Args&&...args)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>(), std::declval<Args>())), ...))
		{
			(func(vec[I], args), ...);
		}

		template <typename Vec1, typename Vec2, typename Func, std::size_t...I>
		constexpr static void binary_invoke_impl(Vec1& vec1, const Vec2& vec2, Func&& func, std::index_sequence<I...>)
		noexcept((noexcept(func(std::declval<decltype(vec1[I])>(), std::declval<decltype(vec2[I])>())), ...))
		{
			(func(vec1[I], vec2[I]), ...);
		}

		template <typename Vec, typename Func, std::size_t...I, typename... Args>
		constexpr static bool binary_invoke_impl_r(Vec& vec, Func&& func, std::index_sequence<I...>, Args&&...args)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>(), std::declval<Args>())), ...))
		{
			return (func(vec[I], args) && ...);
		}

		template <typename Vec1, typename Vec2, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_impl_r(Vec1& vec1, const Vec2& vec2, Func&& func, std::index_sequence<I...>)
		noexcept((noexcept(func(std::declval<decltype(vec1[I])>(), std::declval<decltype(vec2[I])>())), ...))
		{
			return (func(vec1[I], vec2[I]) && ...);
		}

		template <typename Vec, typename Func, std::size_t...I>
		constexpr static void unary_invoke_impl(Vec& vec, Func&& func, std::index_sequence<I...>)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>())), ...))
		{
			(func(vec[I]), ...);
		}

		template <typename Vec, typename Func, std::size_t...I>
		constexpr static bool unary_invoke_impl_r(Vec& vec, Func&& func, std::index_sequence<I...>)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>())), ...))
		{
			return (func(vec[I]) && ...);
		}

		template <bool Left, typename ValueType, typename T>
		constexpr static void shift_impl(ValueType& value, T scalar) noexcept
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				if constexpr (Left)
				{
					value <<= static_cast<ValueType>(scalar);
				}
				else
				{
					value >>= static_cast<ValueType>(scalar);
				}
			}
			else
			{
				bool s_lt_0 = false;
				if constexpr (std::is_signed_v<T>)
				{
					if (scalar < 0)
					{
						s_lt_0 = true;
						scalar = -scalar;
					}
				}

				if constexpr (
					constexpr auto shift = [](ValueType& v, const bool left, auto s) constexpr noexcept -> void
					{
						if (
							constexpr auto pow = [](auto base, auto p) constexpr noexcept -> auto
							{
								auto ret = base;
								while (--p)
								{
									ret *= base;
								}
								return ret;
							};
							left)
						{
							v *= pow(2,
									static_cast<std::conditional_t<
										(sizeof(T) > sizeof(std::conditional_t<
											std::is_signed_v<T>, std::int32_t, std::uint32_t>)),
										std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>,
										std::conditional_t<std::is_signed_v<T>, std::int32_t, std::uint32_t>>>(
										s));
						}
						else
						{
							v /= pow(2,
									static_cast<std::conditional_t<
										(sizeof(T) > sizeof(std::conditional_t<
											std::is_signed_v<T>, std::int32_t, std::uint32_t>)),
										std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>,
										std::conditional_t<std::is_signed_v<T>, std::int32_t, std::uint32_t>>>(
										s));
						}
					}; Left)
				{
					shift(value, !s_lt_0, scalar);
				}
				else
				{
					shift(value, s_lt_0, scalar);
				}
			}
		}

	public:
		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				... Args>
			requires (sizeof...(Args) == 0 || sizeof...(Args) == N)
		constexpr static void invoke(Vec& vec, Func&& func, Args&&...args)
		// todo: noexcept
		// noexcept(sizeof...(Args) == 0 ?
		// 	noexcept(
		// 		vector_trait::unary_invoke_impl(
		// 										std::declval<decltype(vec)>(),
		// 										std::declval<decltype(func)>(),
		// 										std::declval<Indices>()
		// 										)
		// 	) :
		// 	noexcept(
		// 		vector_trait::binary_invoke_impl(
		// 										std::declval<decltype(vec)>(),
		// 										std::declval<decltype(func)>(),
		// 										std::declval<Indices>(),
		// 										std::declval<Args>()...
		// 										)
		// 	)
		// )
		{
			if constexpr (sizeof...(Args) == 0)
			{
				vector_trait::unary_invoke_impl(vec, std::forward<Func>(func), Indices{});
			}
			else
			{
				vector_trait::binary_invoke_impl(vec, std::forward<Func>(func), Indices{}, std::forward<Args>(args)...);
			}
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec1, typename Vec2, typename
				Func>
		constexpr static void invoke_vec(Vec1& vec1, const Vec2& vec2, Func&& func)
		// todo: noexcept
		{
			vector_trait::binary_invoke_impl(vec1, vec2, std::forward<Func>(func), Indices{});
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				... Args>
			requires (sizeof...(Args) == 0 || sizeof...(Args) == N)
		constexpr static bool invoke_r(Vec& vec, Func&& func, Args&&...args)
		// todo: noexcept
		{
			if constexpr (sizeof...(Args) == 0)
			{
				return vector_trait::unary_invoke_impl_r(vec, std::forward<Func>(func), Indices{});
			}
			else
			{
				return vector_trait::binary_invoke_impl_r(vec,
														std::forward<Func>(func),
														Indices{},
														std::forward<Args>(args)...);
			}
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec1, typename Vec2, typename
				Func>
		constexpr static bool invoke_vec_r(Vec1& vec1, const Vec2& vec2, Func&& func)
		// todo: noexcept
		{
			return vector_trait::binary_invoke_impl_r(vec1, vec2, std::forward<Func>(func), Indices{});
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_add(ValueType& value,
											T         scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value += static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_minus(ValueType& value,
											T           scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value -= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_multi(ValueType& value,
											T           scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value *= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_div(ValueType& value,
											T         scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value /= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_percent(ValueType& value,
												T         scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				value %= static_cast<ValueType>(scalar);
			}
			else
			{
				if constexpr (std::is_signed_v<T>)
				{
					if (scalar < 0)
					{
						scalar = -scalar;
					}
				}

				if (value < 0)
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(ValueType) > sizeof(std::int32_t)), std::int64_t, std::int32_t>>(-value / static_cast<
						ValueType>(scalar));
					value += static_cast<ValueType>(n) * static_cast<decltype(n)>(scalar);
				}
				else
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(ValueType) > sizeof(std::uint32_t)), std::uint64_t, std::uint32_t>>(value / static_cast
						<ValueType>(scalar));
					value -= static_cast<ValueType>(n) * static_cast<decltype(n)>(scalar);
				}
			}
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_and(ValueType& value,
											T         scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				value &= static_cast<ValueType>(scalar);
			}
			else
			{
				// do nothing here
			}
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_or(ValueType& value,
										T            scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				value |= static_cast<ValueType>(scalar);
			}
			else
			{
				// do nothing here
			}
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_xor(ValueType& value,
											T         scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				value ^= static_cast<ValueType>(scalar);
			}
			else
			{
				// do nothing here
			}
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_left_shift(ValueType& value,
												T scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			vector_trait::shift_impl<true>(value, scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_right_shift(ValueType& value,
													T scalar) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			vector_trait::shift_impl<false>(value, scalar);
		}

		template <typename ValueType>
		constexpr static void operator_unary_minus(ValueType& value) noexcept
		{
			value = -value;
		}

		template <typename ValueType>
		constexpr static void operator_unary_tilde(ValueType& value) noexcept
		{
			if constexpr (std::is_integral_v<ValueType>)
			{
				value = ~value;
			}
			else
			{
				// do nothing here
			}
		}

		template <typename ValueType, typename T>
		constexpr static bool operator_equal_to(const ValueType& v1, const T& v2) noexcept
		{
			// todo: floating_point
			if constexpr (std::is_floating_point_v<ValueType>)
			{
				if (static_cast<double>(v1) - static_cast<double>(v2) <= 0.000001 && static_cast<double>(v1) -
					static_cast<double>(v2) >= -0.000001)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return v1 == static_cast<ValueType>(v2);
			}
		}

		template <typename ValueType>
		constexpr static bool operator_to_bool(const ValueType& value) noexcept
		{
			return static_cast<bool>(value);
		}
	};
}
