export module gal.toolbox.math.basic_operations;

import <type_traits>;
import <concepts>;
import <utility>;

import gal.toolbox.utils.tuple_maker;
import gal.toolbox.math.math_apply;
export import gal.toolbox.math.vector;
export import gal.toolbox.math.matrix;

namespace gal::toolbox::math
{
	export
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
			template <std::convertible_to<value_type> U>
			constexpr static void operator_add(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value += static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_subtract(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value -= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_multiply(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value *= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_divide(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value /= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
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
			template <std::convertible_to<value_type> U>
			constexpr static void operator_bit_and(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value &= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_bit_or(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value |= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_bit_xor(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				value ^= static_cast<value_type>(scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_left_shift(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				math_invoker::template shift_impl<true>(value, scalar);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static void operator_right_shift(
				value_type& value,
				U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				math_invoker::template shift_impl<false>(value, scalar);
			}

			// unchecked
			constexpr static void operator_abs(value_type& value) noexcept
			{
				if constexpr (std::is_signed_v<value_type>)
				{
					if (value < value_type{ 0 })
					{
						value = -value;
					}
				}
			}

			// unchecked
			constexpr static void operator_negative(value_type& value) noexcept
			{
				value = -value;
			}

			// unchecked
			constexpr static void operator_bit_not(value_type& value) noexcept
			{
				value = ~value;
			}

			constexpr static void operator_not(value_type& value) noexcept
			{
				value = !value;
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static bool operator_equal_to(value_type v1, U v2)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return std::cmp_equal(v1, v2);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			requires std::is_floating_point_v<value_type>
				constexpr static bool operator_equal_to(value_type v1, U v2, value_type epsilon = 0.000001)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return static_cast<double>(v1) - static_cast<double>(v2) <= epsilon &&
					static_cast<double>(v1) - static_cast<double>(v2) >= -epsilon;
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static bool operator_less_than(value_type v1, U v2)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				if constexpr (std::is_floating_point_v<value_type>)
				{
					return v1 < v2;
				}
				else
				{
					return std::cmp_less(v1, v2);
				}
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static bool operator_less_equal_than(value_type v1, U v2)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return math_invoker::operator_less_than(v1, v2) || math_invoker::operator_equal_to(v1, v2);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			requires std::is_floating_point_v<value_type>
				constexpr static bool operator_less_equal_than(value_type v1, U v2, value_type epsilon = 0.000001)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return math_invoker::operator_less_than(v1, v2) || math_invoker::operator_equal_to(v1, v2, epsilon);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static bool operator_greater_than(value_type v1, U v2)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				if constexpr (std::is_floating_point_v<value_type>)
				{
					return v1 > v2;
				}
				else
				{
					return std::cmp_greater(v1, v2);
				}
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			constexpr static bool operator_greater_equal_than(value_type v1, U v2)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return math_invoker::operator_greater_than(v1, v2) || math_invoker::operator_equal_to(v1, v2);
			}

			// unchecked
			template <std::convertible_to<value_type> U>
			requires std::is_floating_point_v<value_type>
				constexpr static bool operator_greater_equal_than(value_type v1, U v2, value_type epsilon = 0.000001)
				noexcept(std::is_nothrow_convertible_v<U, value_type>)
			{
				return math_invoker::operator_greater_than(v1, v2) || math_invoker::operator_equal_to(v1, v2, epsilon);
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
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator+=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <matrix_or_vector_t T1, not_math_type_t T2>
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
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator+=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <math_type_t T1, not_math_type_t T2>
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
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator+(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() +=
				std::declval<const T2&>()
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
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator-=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <matrix_or_vector_t T1, not_math_type_t T2>
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
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator-=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <math_type_t T1, not_math_type_t T2>
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
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator-(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() -=
				std::declval<const T2&>()
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
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator*=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <matrix_or_vector_t T1, not_math_type_t T2>
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
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator*=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <math_type_t T1, not_math_type_t T2>
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
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator*(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() *=
				std::declval<const T2&>()
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
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator/=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <matrix_or_vector_t T1, not_math_type_t T2>
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
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator/=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <math_type_t T1, not_math_type_t T2>
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
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator/(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() /=
				std::declval<const T2&>()
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
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator%=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
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
		template <matrix_or_vector_t T1, not_math_type_t T2>
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
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator%=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
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
		template <math_type_t T1, not_math_type_t T2>
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
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator%(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() %=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret %= t2;
			return ret;
		}

		/**
		* @brief `Bit and` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector1
		* @param t2 matrix/vector2
		* @return matrix/vector after bit and
		*/
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr T1& operator&=(T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit and` a vector/vector_view to another vector_view, requires the same size of the two vectors
		* @tparam T1 vector_view type
		* @tparam T2 vector/vector_view type
		* @param t1 vector_view
		* @param t2 vector/vector_view
		* @return matrix/vector after bit and
		*/
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator&=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit and` another matrix/vector/vector_view to it, and finally return it
		* @tparam T1 matrix/vector/vector_view1 type
		* @tparam T2 matrix/vector/vector_view2 type
		* @param t1 matrix/vector/vector_view1
		* @param t2 matrix/vector/vector_view2
		* @return matrix/vector copy after bit and
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator&(const T1& t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() &= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret &= t2;
			return ret;
		}

		/**
		* @brief `Bit and` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector after bit and
		*/
		template <matrix_or_vector_t T1, not_math_type_t T2>
		constexpr T1& operator&=(T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit and` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
		* @tparam T1 vector_view type
		* @tparam T2 value type
		* @param t1 vector_view
		* @param t2 value
		* @return vector_view after bit and
		*/
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator&=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_and<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit and` a value to a vector_view,
		* repeat this value vector_view::data_size times to `Bit and` to each value of the matrix/vector/vector_view, and finally return it
		* @tparam T1 matrix/vector/vector_view type
		* @tparam T2 value type
		* @param t1 matrix/vector/vector_view
		* @param t2 value
		* @return matrix/vector copy after bit and
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator&(const T1& t1, T2 t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() &= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret &= t2;
			return ret;
		}

		/**
		* @brief A value (type T) `Bit and` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
		* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
		* and then the corresponding value of matrix/vector/vector_view `Bit and` this copied matrix/vector/vector_view::copy_type<T>
		* @tparam T1 value type
		* @tparam T2 matrix/vector/vector_view type
		* @param t1 value
		* @param t2 matrix/vector/vector_view
		* @return matrix/vector/vector_view::copy_type<T1>
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator&(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() &=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret &= t2;
			return ret;
		}

		/**
		* @brief `Bit or` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector1
		* @param t2 matrix/vector2
		* @return matrix/vector after bit or
		*/
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr T1& operator|=(T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit or` a vector/vector_view to another vector_view, requires the same size of the two vectors
		* @tparam T1 vector_view type
		* @tparam T2 vector/vector_view type
		* @param t1 vector_view
		* @param t2 vector/vector_view
		* @return matrix/vector after bit or
		*/
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator|=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit or` another matrix/vector/vector_view to it, and finally return it
		* @tparam T1 matrix/vector/vector_view1 type
		* @tparam T2 matrix/vector/vector_view2 type
		* @param t1 matrix/vector/vector_view1
		* @param t2 matrix/vector/vector_view2
		* @return matrix/vector copy after bit or
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator|(const T1& t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() |= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret |= t2;
			return ret;
		}

		/**
		* @brief `Bit or` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector after bit or
		*/
		template <matrix_or_vector_t T1, not_math_type_t T2>
		constexpr T1& operator|=(T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit or` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
		* @tparam T1 vector_view type
		* @tparam T2 value type
		* @param t1 vector_view
		* @param t2 value
		* @return vector_view after bit or
		*/
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator|=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_or<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit or` a value to a vector_view,
		* repeat this value vector_view::data_size times to `Bit or` to each value of the matrix/vector/vector_view, and finally return it
		* @tparam T1 matrix/vector/vector_view type
		* @tparam T2 value type
		* @param t1 matrix/vector/vector_view
		* @param t2 value
		* @return matrix/vector copy after bit or
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator|(const T1& t1, T2 t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() |= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret |= t2;
			return ret;
		}

		/**
		* @brief A value (type T) `Bit or` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
		* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
		* and then the corresponding value of matrix/vector/vector_view `Bit or` this copied matrix/vector/vector_view::copy_type<T>
		* @tparam T1 value type
		* @tparam T2 matrix/vector/vector_view type
		* @param t1 value
		* @param t2 matrix/vector/vector_view
		* @return matrix/vector/vector_view::copy_type<T1>
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator|(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() |=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret |= t2;
			return ret;
		}

		/**
		* @brief `Bit xor` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector1
		* @param t2 matrix/vector2
		* @return matrix/vector after bit xor
		*/
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr T1& operator^=(T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit xor` a vector/vector_view to another vector_view, requires the same size of the two vectors
		* @tparam T1 vector_view type
		* @tparam T2 vector/vector_view type
		* @param t1 vector_view
		* @param t2 vector/vector_view
		* @return matrix/vector after bit xor
		*/
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator^=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit xor` another matrix/vector/vector_view to it, and finally return it
		* @tparam T1 matrix/vector/vector_view1 type
		* @tparam T2 matrix/vector/vector_view2 type
		* @param t1 matrix/vector/vector_view1
		* @param t2 matrix/vector/vector_view2
		* @return matrix/vector copy after bit xor
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator^(const T1& t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() ^= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret ^= t2;
			return ret;
		}

		/**
		* @brief `Bit xor` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector after bit xor
		*/
		template <matrix_or_vector_t T1, not_math_type_t T2>
		constexpr T1& operator^=(T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Bit xor` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
		* @tparam T1 vector_view type
		* @tparam T2 value type
		* @param t1 vector_view
		* @param t2 value
		* @return vector_view after bit or
		*/
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator^=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_bit_xor<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Bit xor` a value to a vector_view,
		* repeat this value vector_view::data_size times to `Bit xor` to each value of the matrix/vector/vector_view, and finally return it
		* @tparam T1 matrix/vector/vector_view type
		* @tparam T2 value type
		* @param t1 matrix/vector/vector_view
		* @param t2 value
		* @return matrix/vector copy after bit xor
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator^(const T1& t1, T2 t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() ^= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret ^= t2;
			return ret;
		}

		/**
		* @brief A value (type T) `Bit xor` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
		* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
		* and then the corresponding value of matrix/vector/vector_view `Bit xor` this copied matrix/vector/vector_view::copy_type<T>
		* @tparam T1 value type
		* @tparam T2 matrix/vector/vector_view type
		* @param t1 value
		* @param t2 matrix/vector/vector_view
		* @return matrix/vector/vector_view::copy_type<T1>
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator^(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() ^=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret ^= t2;
			return ret;
		}

		/**
		* @brief `Left shift` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector1
		* @param t2 matrix/vector2
		* @return matrix/vector after left shift
		*/
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr T1& operator<<=(T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Left shift` a vector/vector_view to another vector_view, requires the same size of the two vectors
		* @tparam T1 vector_view type
		* @tparam T2 vector/vector_view type
		* @param t1 vector_view
		* @param t2 vector/vector_view
		* @return matrix/vector after left shift
		*/
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator<<=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Left shift` another matrix/vector/vector_view to it, and finally return it
		* @tparam T1 matrix/vector/vector_view1 type
		* @tparam T2 matrix/vector/vector_view2 type
		* @param t1 matrix/vector/vector_view1
		* @param t2 matrix/vector/vector_view2
		* @return matrix/vector copy after left shift
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator<<(const T1& t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() <<= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret <<= t2;
			return ret;
		}

		/**
		* @brief `Left shift` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector after left shift
		*/
		template <matrix_or_vector_t T1, not_math_type_t T2>
		constexpr T1& operator<<=(T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Left shift` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
		* @tparam T1 vector_view type
		* @tparam T2 value type
		* @param t1 vector_view
		* @param t2 value
		* @return vector_view after left shift
		*/
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator<<=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_left_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Left shift` a value to a vector_view,
		* repeat this value vector_view::data_size times to `Left shift` to each value of the matrix/vector/vector_view, and finally return it
		* @tparam T1 matrix/vector/vector_view type
		* @tparam T2 value type
		* @param t1 matrix/vector/vector_view
		* @param t2 value
		* @return matrix/vector copy after left shift
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator<<(const T1& t1, T2 t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() <<= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret <<= t2;
			return ret;
		}

		/**
		* @brief A value (type T) `Left shift` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
		* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
		* and then the corresponding value of matrix/vector/vector_view `Left shift` this copied matrix/vector/vector_view::copy_type<T>
		* @tparam T1 value type
		* @tparam T2 matrix/vector/vector_view type
		* @param t1 value
		* @param t2 matrix/vector/vector_view
		* @return matrix/vector/vector_view::copy_type<T1>
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator<<(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() <<=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret <<= t2;
			return ret;
		}

		/**
		* @brief `Right shift` two matrices/vectors, requires the same size of the matrix/vector (if it is a matrix, both row_size and column_size are required)
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector1
		* @param t2 matrix/vector2
		* @return matrix/vector after right shift
		*/
		template <not_vector_view_t T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr T1& operator>>=(T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Right shift` a vector/vector_view to another vector_view, requires the same size of the two vectors
		* @tparam T1 vector_view type
		* @tparam T2 vector/vector_view type
		* @param t1 vector_view
		* @param t2 vector/vector_view
		* @return matrix/vector after right shift
		*/
		template <vector_view_t T1, math_vector_same_size_t<T1> T2>
		constexpr T1 operator>>=(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Right shift` another matrix/vector/vector_view to it, and finally return it
		* @tparam T1 matrix/vector/vector_view1 type
		* @tparam T2 matrix/vector/vector_view2 type
		* @param t1 matrix/vector/vector_view1
		* @param t2 matrix/vector/vector_view2
		* @return matrix/vector copy after right shift
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator>>(const T1& t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() >>= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret >>= t2;
			return ret;
		}

		/**
		* @brief `Right shift` a value to a matrix/vector, repeat this value matrix/vector::data_size times to add to each value of the matrix/vector
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector after right shift
		*/
		template <matrix_or_vector_t T1, not_math_type_t T2>
		constexpr T1& operator>>=(T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief `Right shift` a value to a vector_view, repeat this value vector_view::data_size times to add to each value of the vector_view
		* @tparam T1 vector_view type
		* @tparam T2 value type
		* @param t1 vector_view
		* @param t2 value
		* @return vector_view after right shift
		*/
		template <vector_view_t T1, not_math_type_t T2>
		constexpr T1 operator>>=(T1 t1, T2 t2)
			noexcept(noexcept(
				operator_base(
					std::declval<T1>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
				)
				))
		{
			operator_base(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_right_shift<math_value_type<T2>>
			);
			return t1;
		}

		/**
		* @brief get a copy of matrix/vector/vector_view, then `Right shift` a value to a vector_view,
		* repeat this value vector_view::data_size times to `Right shift` to each value of the matrix/vector/vector_view, and finally return it
		* @tparam T1 matrix/vector/vector_view type
		* @tparam T2 value type
		* @param t1 matrix/vector/vector_view
		* @param t2 value
		* @return matrix/vector copy after right shift
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator>>(const T1& t1, T2 t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T1&>().copy())>>() >>= std::declval<T2>()
				))
		{
			auto ret = t1.copy();
			ret >>= t2;
			return ret;
		}

		/**
		* @brief A value (type T) `Right shift` a matrix/vector/vector_view will get a matrix/vector/vector_view::copy_type<T>
		* that is initialized with this value repeated matrix/vector/vector_view::data_size times,
		* and then the corresponding value of matrix/vector/vector_view `Right shift` this copied matrix/vector/vector_view::copy_type<T>
		* @tparam T1 value type
		* @tparam T2 matrix/vector/vector_view type
		* @param t1 value
		* @param t2 matrix/vector/vector_view
		* @return matrix/vector/vector_view::copy_type<T1>
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator>>(T1 t1, const T2& t2)
			noexcept(noexcept(
				std::declval<std::add_lvalue_reference_t<typename T2::template copy_type<math_value_type<T1>>>>() >>=
				std::declval<const T2&>()
				))
		{
			typename T2::template copy_type<math_value_type<T1>> ret{
				utils::tuple_maker::duplicate<T2::data_size>(t1), std::make_index_sequence<T2::data_size>{}
			};
			ret >>= t2;
			return ret;
		}

		/**
		* @brief `Abs` the matrix/vector itself
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return inverted matrix/vector
		*/
		template <matrix_or_vector_t T>
		constexpr T& make_abs(T& t)
			noexcept(noexcept(
				operator_base(
					std::declval<T&>(),
					math_invoker<math_value_type<T>>::template operator_abs
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_abs
			);
			return t;
		}

		/**
		* @brief `Abs` the vector_view itself
		* @tparam T vector_view type
		* @param t vector_view
		* @return inverted vector_view
		*/
		template <vector_view_t T>
		constexpr T make_abs(T t)
			noexcept(noexcept(
				operator_base(
					std::declval<T>(),
					math_invoker<math_value_type<T>>::template operator_abs
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_abs
			);
			return t;
		}

		/**
			* @brief get a copy of the `Abs` of matrix/vector
			* @tparam T matrix/vector type
			* @param t matrix/vector
			* @return inverted matrix/vector
		*/
		template <matrix_or_vector_t T>
		constexpr auto operator+(const T& t)
			noexcept(noexcept(
				make_abs(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_abs(ret);
			return ret;
		}

		/**
		* @brief get a copy of the `Abs` of vector_view
		* @tparam T vector_view type
		* @param t vector_view
		* @return inverted vector_view
		*/
		template <vector_view_t T>
		constexpr auto operator+(T t)
			noexcept(noexcept(
				make_abs(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_abs(ret);
			return ret;
		}

		/**
		* @brief `Invert` the matrix/vector itself
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return inverted matrix/vector
		*/
		template <matrix_or_vector_t T>
		constexpr T& make_inverse(T& t)
			noexcept(noexcept(
				operator_base(
					std::declval<T&>(),
					math_invoker<math_value_type<T>>::template operator_negative
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_negative
			);
			return t;
		}

		/**
		* @brief `Invert` the vector_view itself
		* @tparam T vector_view type
		* @param t vector_view
		* @return inverted vector_view
		*/
		template <vector_view_t T>
		constexpr T make_inverse(T t)
			noexcept(noexcept(
				operator_base(
					std::declval<T>(),
					math_invoker<math_value_type<T>>::template operator_negative
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_negative
			);
			return t;
		}

		/**
			* @brief get a copy of the `Inverse` of matrix/vector
			* @tparam T matrix/vector type
			* @param t matrix/vector
			* @return inverted matrix/vector
		*/
		template <matrix_or_vector_t T>
		constexpr auto operator-(const T& t)
			noexcept(noexcept(
				make_inverse(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_inverse(ret);
			return ret;
		}

		/**
		* @brief get a copy of the `Inverse` of vector_view
		* @tparam T vector_view type
		* @param t vector_view
		* @return inverted vector_view
		*/
		template <vector_view_t T>
		constexpr auto operator-(T t)
			noexcept(noexcept(
				make_inverse(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_inverse(ret);
			return ret;
		}

		/**
		* @brief `Bitwise not` the matrix/vector itself
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return matrix/vector after bitwise not
		*/
		template <matrix_or_vector_t T>
		constexpr T& make_bitwise_not(T& t)
			noexcept(noexcept(
				operator_base(
					std::declval<T&>(),
					math_invoker<math_value_type<T>>::template operator_bit_not
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_bit_not
			);
			return t;
		}

		/**
		* @brief `Bitwise not` the vector_view itself
		* @tparam T vector_view type
		* @param t vector_view
		* @return vector_view after bitwise not
		*/
		template <vector_view_t T>
		constexpr T make_bitwise_not(T t)
			noexcept(noexcept(
				operator_base(
					std::declval<T>(),
					math_invoker<math_value_type<T>>::template operator_bit_not
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_bit_not
			);
			return t;
		}

		/**
		* @brief get a copy of the `Bitwise not` of matrix/vector
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return matrix/vector after bitwise not
		*/
		template <matrix_or_vector_t T>
		constexpr auto operator~(const T& t)
			noexcept(noexcept(
				make_bitwise_not(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_bitwise_not(ret);
			return ret;
		}

		/**
		* @brief get a copy of the `Bitwise not` of vector_view
		* @tparam T vector_view type
		* @param t vector_view
		* @return vector_view after bitwise not
		*/
		template <vector_view_t T>
		constexpr auto operator~(T t)
			noexcept(noexcept(
				make_bitwise_not(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_bitwise_not(ret);
			return ret;
		}

		/**
		* @brief `Not` the matrix/vector itself
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return matrix/vector after bitwise not
		*/
		template <matrix_or_vector_t T>
		constexpr T& make_not(T& t)
			noexcept(noexcept(
				operator_base(
					std::declval<T&>(),
					math_invoker<math_value_type<T>>::template operator_not
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_not
			);
			return t;
		}

		/**
		* @brief `Not` the vector_view itself
		* @tparam T vector_view type
		* @param t vector_view
		* @return vector_view after bitwise not
		*/
		template <vector_view_t T>
		constexpr T make_not(T t)
			noexcept(noexcept(
				operator_base(
					std::declval<T>(),
					math_invoker<math_value_type<T>>::template operator_not
				)
				))
		{
			operator_base(
				t,
				math_invoker<math_value_type<T>>::template operator_not
			);
			return t;
		}

		/**
		* @brief get a copy of the `Not` of matrix/vector
		* @tparam T matrix/vector type
		* @param t matrix/vector
		* @return matrix/vector after bitwise not
		*/
		template <matrix_or_vector_t T>
		constexpr auto operator!(const T& t)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_not(ret);
			return ret;
		}

		/**
		* @brief get a copy of the `Not` of vector_view
		* @tparam T vector_view type
		* @param t vector_view
		* @return vector_view after bitwise not
		*/
		template <vector_view_t T>
		constexpr auto operator!(T t)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(std::declval<const T&>().copy())>>()
				)
				))
		{
			auto ret = t.copy();
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto equal(const T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_equal_to<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_equal_to<math_value_type<T2>>
				);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto equal(const T1& t1, const T2& t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					[epsilon](math_value_type<T1> a, math_value_type<T2> b)
					{
						return math_invoker<math_value_type<T1>>::template operator_equal_to<
							math_value_type<T2>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_equal_to<
						math_value_type<T2>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto equal(const T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_equal_to<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_equal_to<math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto equal(const T1& t1, T2 t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					[epsilon](math_value_type<T1> a, math_value_type<T2> b)
					{
						return math_invoker<math_value_type<T1>>::template operator_equal_to<
							math_value_type<T2>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_equal_to<
						math_value_type<T2>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto equal(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					math_invoker<math_value_type<T2>>::template operator_equal_to<math_value_type<T1>>
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				math_invoker<math_value_type<T2>>::template operator_equal_to<math_value_type<T1>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto equal(T1 t1, const T2& t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					[epsilon](math_value_type<T2> a, math_value_type<T1> b)
					{
						return math_invoker<math_value_type<T2>>::template operator_equal_to<
							math_value_type<T1>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				[epsilon](math_value_type<T2> a, math_value_type<T1> b)
				{
					return math_invoker<math_value_type<T2>>::template operator_equal_to<
						math_value_type<T1>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator==(const T1& t1, const T2& t2)
			noexcept(noexcept(
				equal(
					std::declval<const T1&>(),
					std::declval<const T2&>()
				)
				))
		{
			return equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator==(const T1& t1, T2 t2)
			noexcept(noexcept(
				equal(
					std::declval<const T1&>(),
					std::declval<T2>()
				)
				))
		{
			return equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator==(T1 t1, const T2& t2)
			noexcept(noexcept(
				equal(
					std::declval<T1>(),
					std::declval<const T2&>()
				)
				))
		{
			return equal(t1, t2);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Not equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto not_equal(const T1& t1, const T2& t2)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<const T1&>(),
							std::declval<const T2&>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Not equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto not_equal(const T1& t1, const T2& t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<const T1&>(),
							std::declval<const T2&>(),
							std::declval<math_value_type<T1>>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2, epsilon);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto not_equal(const T1& t1, T2 t2)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<const T1&>(),
							std::declval<T2>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto not_equal(const T1& t1, T2 t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<const T1&>(),
							std::declval<T2>(),
							std::declval<math_value_type<T1>>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2, epsilon);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto not_equal(T1 t1, const T2& t2)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<T1>(),
							std::declval<const T2&>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto not_equal(T1 t1, const T2& t2, std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001)
			noexcept(noexcept(
				make_not(
					std::declval<std::add_lvalue_reference_t<decltype(
						equal(
							std::declval<T1>(),
							std::declval<const T2&>(),
							std::declval<math_value_type<T1>>()
						))>>()
				)
				))
		{
			auto ret = equal(t1, t2, epsilon);
			make_not(ret);
			return ret;
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Not equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator!=(const T1& t1, const T2& t2)
			noexcept(noexcept(
				not_equal(
					std::declval<const T1&>(),
					std::declval<const T2&>()
				)
				))
		{
			return not_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator!=(const T1& t1, T2 t2)
			noexcept(noexcept(
				not_equal(
					std::declval<const T1&>(),
					std::declval<T2>()
				)
				))
		{
			return not_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Not equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator!=(T1 t1, const T2& t2)
			noexcept(noexcept(
				not_equal(
					std::declval<T1>(),
					std::declval<const T2&>()
				)
				))
		{
			return not_equal(t1, t2);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Less` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto less(const T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_less_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_less_than<math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto less(const T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_less_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_less_than<math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		// we cannot assume that the target type exists operator>
		requires requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 > t1 } -> std::same_as<bool>;
		}
		constexpr auto less(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					math_invoker<math_value_type<T2>>::template operator_greater_than<math_value_type<T1>>
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				math_invoker<math_value_type<T2>>::template operator_greater_than<
				math_value_type<T1>>
				);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Less` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator<(const T1& t1, const T2& t2)
			noexcept(noexcept(
				less(
					std::declval<const T1&>(),
					std::declval<const T2&>()
				)
				))
		{
			return less(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator<(const T1& t1, T2 t2)
			noexcept(noexcept(
				less(
					std::declval<const T1&>(),
					std::declval<T2>()
				)
				))
		{
			return less(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator<(T1 t1, const T2& t2)
			noexcept(noexcept(
				less(
					std::declval<T1>(),
					std::declval<const T2&>()
				)
				))
		{
			return less(t1, t2);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Less equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto less_equal(const T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_less_equal_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_less_equal_than<
				math_value_type<T2>>
				);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Less equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto less_equal(
				const T1& t1, const T2& t2,
				std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
			)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					[epsilon](math_value_type<T1> a, math_value_type<T2> b)
					{
						return math_invoker<math_value_type<T1>>::template operator_less_equal_than<
							math_value_type<T2>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_less_equal_than<
						math_value_type<T2>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto less_equal(const T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_less_equal_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_less_equal_than<math_value_type
				<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto less_equal(
				const T1& t1, T2 t2,
				std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
			)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					[epsilon](math_value_type<T1> a, math_value_type<T2> b)
					{
						return math_invoker<math_value_type<T1>>::template operator_less_equal_than<
							math_value_type<T2>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_less_equal_than<
						math_value_type<T2>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		// we cannot assume that the target type exists operator> and operator==
		requires requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 > t1 } -> std::same_as<bool>;
			{ t2 == t1 } -> std::same_as<bool>;
		}
		constexpr auto less_equal(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					math_invoker<math_value_type<T2>>::template operator_greater_equal_than<math_value_type<T1>>
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				math_invoker<math_value_type<T2>>::template operator_greater_equal_than<
				math_value_type<T1>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>&&
			// we cannot assume that the target type exists operator> and operator==
			requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 > t1 } -> std::same_as<bool>;
			{ t2 == t1 } -> std::same_as<bool>;
		}
		constexpr auto less_equal(
			T1                                        t1, const T2& t2,
			std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
		)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					[epsilon](math_value_type<T2> a, math_value_type<T1> b)
					{
						return math_invoker<math_value_type<T2>>::template operator_greater_equal_than<
							math_value_type<T1>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				[epsilon](math_value_type<T2> a, math_value_type<T1> b)
				{
					return math_invoker<math_value_type<T2>>::template operator_greater_equal_than<
						math_value_type<T1>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Less equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator<=(const T1& t1, const T2& t2)
			noexcept(noexcept(
				less_equal(
					std::declval<const T1&>(),
					std::declval<const T2&>()
				)
				))
		{
			return less_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator<=(const T1& t1, T2 t2)
			noexcept(noexcept(
				less_equal(
					std::declval<const T1&>(),
					std::declval<T2>()
				)
				))
		{
			return less_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Less equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator<=(T1 t1, const T2& t2)
			noexcept(noexcept(
				less_equal(
					std::declval<T1>(),
					std::declval<const T2&>()
				)
				))
		{
			return less_equal(t1, t2);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Greater` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto greater(const T1& t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<const T2&>(),
					math_invoker<math_value_type<T1>>::template operator_greater_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_greater_than<
				math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto greater(const T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_greater_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_greater_than<
				math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		// we cannot assume that the target type exists operator<
		requires requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 < t1 } -> std::same_as<bool>;
		}
		constexpr auto greater(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					math_invoker<math_value_type<T1>>::template operator_less_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				math_invoker<math_value_type<T1>>::template operator_less_than<
				math_value_type<T2>>
				);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Greater` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator>(const T1& t1, const T2& t2)
		noexcept(noexcept(
			greater(
				std::declval<const T1&>(),
				std::declval<const T2&>()
			)
			))
		{
		return greater(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator>(const T1& t1, T2 t2)
		noexcept(noexcept(
			greater(
				std::declval<const T1&>(),
				std::declval<T2>()
			)
			))
		{
		return greater(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator>(T1 t1, const T2& t2)
		noexcept(noexcept(
			greater(
				std::declval<T1>(),
				std::declval<const T2&>()
			)
			))
		{
		return greater(t1, t2);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Greater equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto greater_equal(const T1& t1, const T2& t2)
		noexcept(noexcept(
			operator_base<bool>(
				std::declval<const T1&>(),
				std::declval<const T2&>(),
				math_invoker<math_value_type<T1>>::template operator_greater_equal_than<math_value_type<T2>>
				)
			))
		{
		return operator_base<bool>(
			t1,
			t2,
			math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
			math_value_type<T2>>
			);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Greater equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		requires std::is_floating_point_v<math_value_type<T1>>
		constexpr auto greater_equal(
			const T1& t1, const T2& t2,
			std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
		)
		noexcept(noexcept(
			operator_base<bool>(
				std::declval<const T1&>(),
				std::declval<const T2&>(),
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
						math_value_type<T2>>(a, b, epsilon);
				}
				)
			))
		{
		return operator_base<bool>(
			t1,
			t2,
			[epsilon](math_value_type<T1> a, math_value_type<T2> b)
			{
				return math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
					math_value_type<T2>>(a, b, epsilon);
			}
		);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto greater_equal(const T1& t1, T2 t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					math_invoker<math_value_type<T1>>::template operator_greater_equal_than<math_value_type<T2>>
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
				math_value_type<T2>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>
			constexpr auto greater_equal(
				const T1& t1, T2 t2,
				std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
			)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T1&>(),
					std::declval<T2>(),
					[epsilon](math_value_type<T1> a, math_value_type<T2> b)
					{
						return math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
							math_value_type<T2>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t1,
				t2,
				[epsilon](math_value_type<T1> a, math_value_type<T2> b)
				{
					return math_invoker<math_value_type<T1>>::template operator_greater_equal_than<
						math_value_type<T2>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		// we cannot assume that the target type exists operator< and operator==
		requires requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 < t1 } -> std::same_as<bool>;
			{ t2 == t1 } -> std::same_as<bool>;
		}
		constexpr auto greater_equal(T1 t1, const T2& t2)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					math_invoker<math_value_type<T2>>::template operator_less_equal_than<math_value_type<T1>>
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				math_invoker<math_value_type<T2>>::template operator_less_equal_than<
				math_value_type<T1>>
				);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @param epsilon allowable error of floating point comparison
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		requires std::is_floating_point_v<math_value_type<T1>>&&
			// we cannot assume that the target type exists operator< and operator==
			requires(math_value_type<T1> t1, math_value_type<T2> t2)
		{
			{ t2 < t1 } -> std::same_as<bool>;
			{ t2 == t1 } -> std::same_as<bool>;
		}
		constexpr auto greater_equal(
			T1                                        t1, const T2& t2,
			std::type_identity_t<math_value_type<T1>> epsilon = 0.0000001
		)
			noexcept(noexcept(
				operator_base<bool>(
					std::declval<const T2&>(),
					std::declval<T1>(),
					[epsilon](math_value_type<T2> a, math_value_type<T1> b)
					{
						return math_invoker<math_value_type<T2>>::template operator_less_equal_than<
							math_value_type<T1>>(a, b, epsilon);
					}
					)
				))
		{
			return operator_base<bool>(
				t2,
				t1,
				[epsilon](math_value_type<T2> a, math_value_type<T1> b)
				{
					return math_invoker<math_value_type<T2>>::template
						operator_less_equal_than<
						math_value_type<T1>>(a, b, epsilon);
				}
			);
		}

		/**
		* @brief compare two matrices/vectors and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the two matrices/vectors are `Greater equal` at that point
		* @tparam T1 matrix/vector1 type
		* @tparam T2 matrix/vector2 type
		* @param t1 matrix/vector
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <typename T1, math_same_type_and_size_ignore_view_t<T1> T2>
		constexpr auto operator>=(const T1& t1, const T2& t2)
			noexcept(noexcept(
				greater_equal(
					std::declval<const T1&>(),
					std::declval<const T2&>()
				)
				))
		{
			return greater_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 matrix/vector type
		* @tparam T2 value type
		* @param t1 matrix/vector
		* @param t2 value
		* @return matrix/vector
		*/
		template <math_type_t T1, not_math_type_t T2>
		constexpr auto operator>=(const T1& t1, T2 t2)
			noexcept(noexcept(
				greater_equal(
					std::declval<const T1&>(),
					std::declval<T2>()
				)
				))
		{
			return greater_equal(t1, t2);
		}

		/**
		* @brief compare a matrix/vector with a value and return a matrix/vector whose value_type is bool,
		* and the value of the corresponding position is whether the values of the matrix/vector and the value  are `Greater equal` at that point
		* @tparam T1 value type
		* @tparam T2 matrix/vector type
		* @param t1 value
		* @param t2 matrix/vector
		* @return matrix/vector
		*/
		template <not_math_type_t T1, math_type_t T2>
		constexpr auto operator>=(T1 t1, const T2& t2)
			noexcept(noexcept(
				greater_equal(
					std::declval<T1>(),
					std::declval<const T2&>()
				)
				))
		{
			return greater_equal(t1, t2);
		}
	}
}
