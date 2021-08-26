// ReSharper disable CppInconsistentNaming
#pragma once

#include <type_traits>

namespace gal::test::math
{
	template <typename T, std::size_t N>
	class vector;
	template <typename T, std::size_t Row, std::size_t Column>
	class matrix;

	template <typename T>
	using vector1 = vector<T, 1>;
	template <typename T>
	using vector2 = vector<T, 2>;
	template <typename T>
	using vector3 = vector<T, 3>;
	template <typename T>
	using vector4 = vector<T, 4>;

	template <typename T>
	using matrix2x2 = matrix<T, 2, 2>;
	template <typename T>
	using matrix2x3 = matrix<T, 2, 3>;
	template <typename T>
	using matrix2x4 = matrix<T, 2, 4>;
	template <typename T>
	using matrix3x2 = matrix<T, 3, 2>;
	template <typename T>
	using matrix3x3 = matrix<T, 3, 3>;
	template <typename T>
	using matrix3x4 = matrix<T, 3, 4>;
	template <typename T>
	using matrix4x2 = matrix<T, 4, 2>;
	template <typename T>
	using matrix4x3 = matrix<T, 4, 3>;
	template <typename T>
	using matrix4x4 = matrix<T, 4, 4>;

	/**
	 * @brief specialize math_invoker_trait for operator support
	 * @note if the operation corresponding to the target type is false, the corresponding operation is not allowed
	 * @tparam T specialized type
	*/
	template <typename T>
	struct math_invoker_trait : std::false_type {};

	/**
	 * @brief specialize math_invoker for the actual implementer of the operation
	 * @note if the operation corresponding to the target type is not implemented, the corresponding operation is not allowed
	 * @tparam T specialized type
	*/
	template <typename T>
	struct math_invoker : std::false_type {};

	/**
	 * @brief give some basic operations for the arithmetic type
	 * @tparam T arithmetic
	*/
	template <typename T>
		requires std::is_arithmetic_v<T>
	struct math_invoker<T> : std::true_type
	{
		using value_type = T;
		static_assert(std::is_arithmetic_v<value_type>);
	private:
		template <bool Left, typename U>
		constexpr static void shift_impl(value_type& value, U scalar) noexcept
		{
			if constexpr (std::is_floating_point_v<value_type>)
			{
				bool s_lt_0 = scalar < 0;
				if (s_lt_0)
				{
					scalar = -scalar;
				}

				if constexpr (
					constexpr auto shift = [](value_type& v, const bool left, auto s) constexpr noexcept -> void
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
			else
			{
				if constexpr (Left)
				{
					value <<= static_cast<value_type>(scalar);
				}
				else
				{
					value >>= static_cast<value_type>(scalar);
				}
			}
		}

	public:
		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_add(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value += static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_subtract(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value -= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_multiply(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value *= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_divide(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value /= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_model(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			if constexpr (std::is_floating_point_v<value_type>)
			{
				if (scalar < 0)
				{
					scalar = -scalar;
				}

				if (value < 0)
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(value_type) > sizeof(std::int32_t)), std::int64_t, std::int32_t>>(-value / static_cast<
						value_type>(scalar));
					value += static_cast<value_type>(n) * static_cast<decltype(n)>(scalar);
				}
				else
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(value_type) > sizeof(std::uint32_t)), std::uint64_t, std::uint32_t>>(value / static_cast
						<value_type>(scalar));
					value -= static_cast<value_type>(n) * static_cast<decltype(n)>(scalar);
				}
			}
			else
			{
				value %= static_cast<value_type>(scalar);
			}
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_and(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value &= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_or(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value |= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_xor(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value ^= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_left_shift(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			math_invoker::template shift_impl<true>(value, scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_right_shift(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			math_invoker::template shift_impl<false>(value, scalar);
		}

		// unchecked
		constexpr static void operator_unary_minus(value_type& value)
		noexcept
		{
			value = -value;
		}

		// unchecked
		constexpr static void operator_unary_tilde(value_type& value)
		noexcept
		{
			value = ~value;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static bool operator_equal_to(value_type v1, U v2)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			// todo: floating_point
			if constexpr (std::is_floating_point_v<value_type>)
			{
				return static_cast<double>(v1) - static_cast<double>(v2) <= 0.000001 &&
					static_cast<double>(v1) - static_cast<double>(v2) >= -0.000001;
			}
			else
			{
				return v1 == v2;
			}
		}
	};
}
