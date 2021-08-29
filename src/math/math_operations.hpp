#pragma once

#include "new_vector.hpp"
#include "new_matrix.hpp"
#include "math_apply.hpp"
#include "../utils/tuple_maker.hpp"

namespace gal::test::new_math
{
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

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static bool operator_equal_to(value_type v1, U v2)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return v1 == v2;
		}

		// unchecked
		template <typename U>
			requires (std::is_convertible_v<U, value_type> && std::is_floating_point_v<value_type>)
		constexpr static bool operator_equal_to(value_type v1, U v2, value_type epsilon = 0.000001)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return static_cast<double>(v1) - static_cast<double>(v2) <= epsilon &&
				static_cast<double>(v1) - static_cast<double>(v2) >= -epsilon;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_less_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 < scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_less_equal_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 < scalar2 || scalar1 == scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_greater_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 > scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_greater_equal_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 > scalar2 || scalar1 == scalar2;
		}
	};

	/**
	 * @brief `Add` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
	 * @tparam T1 matrix/vector1 type
	 * @tparam T2 matrix/vector2 type
	 * @param t1 matrix/vector1
	 * @param t2 matrix/vector2
	 * @return added matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(
			(is_vector_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)
		) &&
		is_math_same_size_v<T1, T2>
	constexpr T1& operator+=(T1& t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Add` a vector/vector_view to another vector_view, requires the same size of the two vectors
	* @tparam T1 vector_view type
	* @tparam T2 vector/vector_view type
	* @param t1 vector_view
	* @param t2 vector/vector_view
	* @return added matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr T1 operator+=(T1 t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Add` another matrix/vector/vector_view to it, and finally return it
	* @tparam T1 matrix/vector/vector_view1 type
	* @tparam T2 matrix/vector/vector_view2 type
	* @param t1 matrix/vector/vector_view1
	* @param t2 matrix/vector/vector_view2
	* @return added matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires
		((is_vector_type_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator+(const T1& t1, const T2& t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() += std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret += t2;
		return ret;
	}

	/**
	 * @brief `Add` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
	 * @tparam T1 matrix/vector type
	 * @tparam T2 value type
	 * @param t1 matrix/vector
	 * @param t2 value
	 * @return added matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr T1& operator+=(T1& t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					);
		return t1;
	}

	/**
	 * @brief `Add` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
	 * @tparam T1 vector_view type
	 * @tparam T2 value type
	 * @param t1 vector_view
	 * @param t2 value
	 * @return added vector_view self
	*/
	template <typename T1, typename T2>
		requires is_vector_view_v<T1> && (!is_math_type_v<T2>)
	constexpr T1 operator+=(T1 t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Add` a value to a vector_view,
	* repeat this value vector_view::data_size times to `Add` to each value of the matrix/vector/vector_view, and finally return it
	* @tparam T1 matrix/vector/vector_view type
	* @tparam T2 value type
	* @param t1 matrix/vector/vector_view
	* @param t2 value
	* @return added matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires is_math_type_v<T1> && (!is_math_type_v<T2>)
	constexpr auto operator+(const T1& t1, T2 t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() += std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret += t2;
		return ret;
	}

	/**
	 * @brief A value (type T) `Add` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
	 * that is initialized with this value repeated matrix/vector/vector_view::data_size times,
	 * and then the corresponding value of matrix/vector/vector_view `Add` this copied matrix/vector/vector_view::copy_type<T>
	 * @tparam T1 value type
	 * @tparam T2 matrix/vector/vector_view type
	 * @param t1 value
	 * @param t2 matrix/vector/vector_view
	 * @return matrix/vector/vector_view::copy_type<T1>
	*/
	template <typename T1, typename T2>
		requires
		(!is_math_type_v<T1>) &&
		(is_matrix_v<T2> || is_vector_type_v<T2>)
	constexpr auto operator+(T1 t1, const T2& t2)
	noexcept(noexcept(
		std::declval<typename T2::template copy_type<math_value_type<T1>>>() += std::declval<const T2&>()
	))
	{
		typename T2::template copy_type<math_value_type<T1>> ret{
			utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
		};
		ret += t2;
		return ret;
	}

	/**
	* @brief `Subtract` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
	* @tparam T1 matrix/vector1 type
	* @tparam T2 matrix/vector2 type
	* @param t1 matrix/vector1
	* @param t2 matrix/vector2
	* @return subtracted matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(
			(is_vector_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)
		) &&
		is_math_same_size_v<T1, T2>
	constexpr T1& operator-=(T1& t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Subtract` a vector/vector_view to another vector_view, requires the same size of the two vectors
	* @tparam T1 vector_view type
	* @tparam T2 vector/vector_view type
	* @param t1 vector_view
	* @param t2 vector/vector_view
	* @return subtracted matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr T1 operator-=(T1 t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Subtract` another matrix/vector/vector_view to it, and finally return it
	* @tparam T1 matrix/vector/vector_view1 type
	* @tparam T2 matrix/vector/vector_view2 type
	* @param t1 matrix/vector/vector_view1
	* @param t2 matrix/vector/vector_view2
	* @return subtracted matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires
		((is_vector_type_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator-(const T1& t1, const T2& t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() -= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret -= t2;
		return ret;
	}

	/**
	* @brief `Subtract` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
	* @tparam T1 matrix/vector type
	* @tparam T2 value type
	* @param t1 matrix/vector
	* @param t2 value
	* @return subtracted matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr T1& operator-=(T1& t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Subtract` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
	* @tparam T1 vector_view type
	* @tparam T2 value type
	* @param t1 vector_view
	* @param t2 value
	* @return subtracted vector_view self
	*/
	template <typename T1, typename T2>
		requires is_vector_view_v<T1> && (!is_math_type_v<T2>)
	constexpr T1 operator-=(T1 t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_subtract<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Subtract` a value to a vector_view,
	* repeat this value vector_view::data_size times to `Subtract` to each value of the matrix/vector/vector_view, and finally return it
	* @tparam T1 matrix/vector/vector_view type
	* @tparam T2 value type
	* @param t1 matrix/vector/vector_view
	* @param t2 value
	* @return subtracted matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires is_math_type_v<T1> && (!is_math_type_v<T2>)
	constexpr auto operator-(const T1& t1, T2 t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() -= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret -= t2;
		return ret;
	}

	/**
	* @brief A value (type T) `Subtract` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
	* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
	* and then the corresponding value of matrix/vector/vector_view `Subtract` this copied matrix/vector/vector_view::copy_type<T>
	* @tparam T1 value type
	* @tparam T2 matrix/vector/vector_view type
	* @param t1 value
	* @param t2 matrix/vector/vector_view
	* @return matrix/vector/vector_view::copy_type<T1>
	*/
	template <typename T1, typename T2>
		requires
		(!is_math_type_v<T1>) &&
		(is_matrix_v<T2> || is_vector_type_v<T2>)
	constexpr auto operator-(T1 t1, const T2& t2)
	noexcept(noexcept(
		std::declval<typename T2::template copy_type<math_value_type<T1>>>() -= std::declval<const T2&>()
	))
	{
		typename T2::template copy_type<math_value_type<T1>> ret{
			utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
		};
		ret -= t2;
		return ret;
	}

	/**
	* @brief `Multiply` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
	* @tparam T1 matrix/vector1 type
	* @tparam T2 matrix/vector2 type
	* @param t1 matrix/vector1
	* @param t2 matrix/vector2
	* @return multiplied matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(
			(is_vector_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)
		) &&
		is_math_same_size_v<T1, T2>
	constexpr T1& operator*=(T1& t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Multiply` a vector/vector_view to another vector_view, requires the same size of the two vectors
	* @tparam T1 vector_view type
	* @tparam T2 vector/vector_view type
	* @param t1 vector_view
	* @param t2 vector/vector_view
	* @return multiplied matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr T1 operator*=(T1 t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Multiply` another matrix/vector/vector_view to it, and finally return it
	* @tparam T1 matrix/vector/vector_view1 type
	* @tparam T2 matrix/vector/vector_view2 type
	* @param t1 matrix/vector/vector_view1
	* @param t2 matrix/vector/vector_view2
	* @return multiplied matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires
		((is_vector_type_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator*(const T1& t1, const T2& t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() *= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret *= t2;
		return ret;
	}

	/**
	* @brief `Multiply` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
	* @tparam T1 matrix/vector type
	* @tparam T2 value type
	* @param t1 matrix/vector
	* @param t2 value
	* @return multiplied matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr T1& operator*=(T1& t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Multiply` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
	* @tparam T1 vector_view type
	* @tparam T2 value type
	* @param t1 vector_view
	* @param t2 value
	* @return multiplied vector_view self
	*/
	template <typename T1, typename T2>
		requires is_vector_view_v<T1> && (!is_math_type_v<T2>)
	constexpr T1 operator*=(T1 t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_multiply<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Multiply` a value to a vector_view,
	* repeat this value vector_view::data_size times to `Multiply` to each value of the matrix/vector/vector_view, and finally return it
	* @tparam T1 matrix/vector/vector_view type
	* @tparam T2 value type
	* @param t1 matrix/vector/vector_view
	* @param t2 value
	* @return multiplied matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires is_math_type_v<T1> && (!is_math_type_v<T2>)
	constexpr auto operator*(const T1& t1, T2 t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() *= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret *= t2;
		return ret;
	}

	/**
	* @brief A value (type T) `Multiply` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
	* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
	* and then the corresponding value of matrix/vector/vector_view `Multiply` this copied matrix/vector/vector_view::copy_type<T>
	* @tparam T1 value type
	* @tparam T2 matrix/vector/vector_view type
	* @param t1 value
	* @param t2 matrix/vector/vector_view
	* @return matrix/vector/vector_view::copy_type<T1>
	*/
	template <typename T1, typename T2>
		requires
		(!is_math_type_v<T1>) &&
		(is_matrix_v<T2> || is_vector_type_v<T2>)
	constexpr auto operator*(T1 t1, const T2& t2)
	noexcept(noexcept(
		std::declval<typename T2::template copy_type<math_value_type<T1>>>() *= std::declval<const T2&>()
	))
	{
		typename T2::template copy_type<math_value_type<T1>> ret{
			utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
		};
		ret *= t2;
		return ret;
	}

	/**
	* @brief `Divide` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
	* @tparam T1 matrix/vector1 type
	* @tparam T2 matrix/vector2 type
	* @param t1 matrix/vector1
	* @param t2 matrix/vector2
	* @return divided matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(
			(is_vector_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)
		) &&
		is_math_same_size_v<T1, T2>
	constexpr T1& operator/=(T1& t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Divide` a vector/vector_view to another vector_view, requires the same size of the two vectors
	* @tparam T1 vector_view type
	* @tparam T2 vector/vector_view type
	* @param t1 vector_view
	* @param t2 vector/vector_view
	* @return divided matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr T1 operator/=(T1 t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Divide` another matrix/vector/vector_view to it, and finally return it
	* @tparam T1 matrix/vector/vector_view1 type
	* @tparam T2 matrix/vector/vector_view2 type
	* @param t1 matrix/vector/vector_view1
	* @param t2 matrix/vector/vector_view2
	* @return divided matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires
		((is_vector_type_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator/(const T1& t1, const T2& t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() /= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret /= t2;
		return ret;
	}

	/**
	* @brief `Divide` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
	* @tparam T1 matrix/vector type
	* @tparam T2 value type
	* @param t1 matrix/vector
	* @param t2 value
	* @return divided matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr T1& operator/=(T1& t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Divide` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
	* @tparam T1 vector_view type
	* @tparam T2 value type
	* @param t1 vector_view
	* @param t2 value
	* @return divided vector_view self
	*/
	template <typename T1, typename T2>
		requires is_vector_view_v<T1> && (!is_math_type_v<T2>)
	constexpr T1 operator/=(T1 t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_divide<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Divide` a value to a vector_view,
	* repeat this value vector_view::data_size times to `Divide` to each value of the matrix/vector/vector_view, and finally return it
	* @tparam T1 matrix/vector/vector_view type
	* @tparam T2 value type
	* @param t1 matrix/vector/vector_view
	* @param t2 value
	* @return divided matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires is_math_type_v<T1> && (!is_math_type_v<T2>)
	constexpr auto operator/(const T1& t1, T2 t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() /= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret /= t2;
		return ret;
	}

	/**
	* @brief A value (type T) `Divide` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
	* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
	* and then the corresponding value of matrix/vector/vector_view `Divide` this copied matrix/vector/vector_view::copy_type<T>
	* @tparam T1 value type
	* @tparam T2 matrix/vector/vector_view type
	* @param t1 value
	* @param t2 matrix/vector/vector_view
	* @return matrix/vector/vector_view::copy_type<T1>
	*/
	template <typename T1, typename T2>
		requires
		(!is_math_type_v<T1>) &&
		(is_matrix_v<T2> || is_vector_type_v<T2>)
	constexpr auto operator/(T1 t1, const T2& t2)
	noexcept(noexcept(
		std::declval<typename T2::template copy_type<math_value_type<T1>>>() /= std::declval<const T2&>()
	))
	{
		typename T2::template copy_type<math_value_type<T1>> ret{
			utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
		};
		ret /= t2;
		return ret;
	}

	/**
	* @brief `Model` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
	* @tparam T1 matrix/vector1 type
	* @tparam T2 matrix/vector2 type
	* @param t1 matrix/vector1
	* @param t2 matrix/vector2
	* @return modeled matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(
			(is_vector_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)
		) &&
		is_math_same_size_v<T1, T2>
	constexpr T1& operator%=(T1& t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Model` a vector/vector_view to another vector_view, requires the same size of the two vectors
	* @tparam T1 vector_view type
	* @tparam T2 vector/vector_view type
	* @param t1 vector_view
	* @param t2 vector/vector_view
	* @return modeled matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr T1 operator%=(T1 t1, const T2& t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Model` another matrix/vector/vector_view to it, and finally return it
	* @tparam T1 matrix/vector/vector_view1 type
	* @tparam T2 matrix/vector/vector_view2 type
	* @param t1 matrix/vector/vector_view1
	* @param t2 matrix/vector/vector_view2
	* @return modeled matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires
		((is_vector_type_v<T1> && is_vector_type_v<T2>) ||
			(is_matrix_v<T1> && is_matrix_v<T2>)) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator%(const T1& t1, const T2& t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() %= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret %= t2;
		return ret;
	}

	/**
	* @brief `Model` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
	* @tparam T1 matrix/vector type
	* @tparam T2 value type
	* @param t1 matrix/vector
	* @param t2 value
	* @return modeled matrix/vector self
	*/
	template <typename T1, typename T2>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr T1& operator%=(T1& t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief `Model` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
	* @tparam T1 vector_view type
	* @tparam T2 value type
	* @param t1 vector_view
	* @param t2 value
	* @return modeled vector_view self
	*/
	template <typename T1, typename T2>
		requires is_vector_view_v<T1> && (!is_math_type_v<T2>)
	constexpr T1 operator%=(T1 t1, T2 t2)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					)
	))
	{
		operator_base(
					t1,
					t2,
					math_invoker<math_value_type<T1>>::template operator_model<math_value_type<T2>>
					);
		return t1;
	}

	/**
	* @brief get a copy of matrix/vector/vector_view, then `Model` a value to a vector_view,
	* repeat this value vector_view::data_size times to `Model` to each value of the matrix/vector/vector_view, and finally return it
	* @tparam T1 matrix/vector/vector_view type
	* @tparam T2 value type
	* @param t1 matrix/vector/vector_view
	* @param t2 value
	* @return modeled matrix/vector copy
	*/
	template <typename T1, typename T2>
		requires is_math_type_v<T1> && (!is_math_type_v<T2>)
	constexpr auto operator%(const T1& t1, T2 t2)
	noexcept(noexcept(
		std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() %= std::declval<T2>()
	))
	{
		auto ret = t1.copy();
		ret %= t2;
		return ret;
	}

	/**
	* @brief A value (type T) `Model` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
	* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
	* and then the corresponding value of matrix/vector/vector_view `Model` this copied matrix/vector/vector_view::copy_type<T>
	* @tparam T1 value type
	* @tparam T2 matrix/vector/vector_view type
	* @param t1 value
	* @param t2 matrix/vector/vector_view
	* @return matrix/vector/vector_view::copy_type<T1>
	*/
	template <typename T1, typename T2>
		requires
		(!is_math_type_v<T1>) &&
		(is_matrix_v<T2> || is_vector_type_v<T2>)
	constexpr auto operator%(T1 t1, const T2& t2)
	noexcept(noexcept(
		std::declval<typename T2::template copy_type<math_value_type<T1>>>() %= std::declval<const T2&>()
	))
	{
		typename T2::template copy_type<math_value_type<T1>> ret{
			utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
		};
		ret %= t2;
		return ret;
	}
}
