#pragma once

#include <type_traits>
#include <tuple>
#include <array>

namespace gal::test::math
{
	//===============================
	template <typename T>
	concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;
	template <arithmetic T, std::size_t N>
	class vector;

	template <arithmetic T>
	using vector1 = vector<T, 1>;
	template <arithmetic T>
	using vector2 = vector<T, 2>;
	template <arithmetic T>
	using vector3 = vector<T, 3>;
	template <arithmetic T>
	using vector4 = vector<T, 4>;

	/**
	 * @brief vector construction assistance: can be used when trying to assign all values of the entire vector with one parameter
	 * @tparam T arg type
	 * @param arg arg
	 * @return tuple containing repeated parameters
	*/
	template <std::size_t N, typename T>
	constexpr auto duplicate(const T& arg) noexcept;
	//===============================

	template <typename T>
	struct vector_trait
	{
		using value_type = T;
		constexpr static bool        value = false;
		constexpr static std::size_t size  = 1;
	};

	template <arithmetic T, std::size_t N>
	struct vector_trait<vector<T, N>>
	{
		using value_type = T;
		constexpr static bool        value = true;
		constexpr static std::size_t size  = N;
	};

	template <typename T>
	constexpr auto to_rref_tuple(T&& t) noexcept
	{
		using trait = vector_trait<std::remove_cvref_t<T>>;
		if constexpr (trait::value)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return std::forward_as_tuple(std::forward<T>(t)[I]...);
			}(std::make_index_sequence<trait::size>{});
		}
		else
		{
			return std::forward_as_tuple(std::forward<T>(t));
		}
	}

	template <typename T>
	constexpr auto to_tuple(const T& t) noexcept
	{
		using trait = vector_trait<std::remove_cvref_t<T>>;
		if constexpr (trait::value)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return std::make_tuple(t[I]...);
			}(std::make_index_sequence<trait::size>{});
		}
		else
		{
			return std::make_tuple(t);
		}
	}

	template <std::size_t N, typename T>
	constexpr auto duplicate(const T& arg) noexcept
	{
		if constexpr (N == 0)
		{
			return std::make_tuple();
		}
		else
		{
			using trait = vector_trait<std::remove_cvref_t<T>>;
			if constexpr (trait::value)
			{
				if constexpr (trait::size >= N)
				{
					return to_tuple(arg);
				}
				else
				{
					return std::tuple_cat(to_tuple(arg), duplicate<N - trait::size>(arg));
				}
			}
			else
			{
				return std::tuple_cat(to_tuple(arg), duplicate<N - 1>(arg));
			}
		}
	}

	struct vector_invoker
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

		template <typename Vec, typename Func, std::size_t...I, typename T>
		constexpr static void binary_invoke_impl(Vec& vec, Func&& func, std::index_sequence<I...>, T arg)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>(), std::declval<T>())), ...))
		{
			(func(vec[I], arg), ...);
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

		template <typename Vec, typename Func, std::size_t...I, typename T>
		constexpr static bool binary_invoke_impl_r(Vec& vec, Func&& func, std::index_sequence<I...>, T arg)
		noexcept((noexcept(func(std::declval<decltype(vec[I])>(), std::declval<T>())), ...))
		{
			return (func(vec[I], arg) && ...);
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
		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func>
		constexpr static void invoke(Vec& vec, Func&& func)
		noexcept(
			noexcept(
				vector_invoker::unary_invoke_impl(
												std::declval<decltype(vec)>(),
												std::declval<decltype(func)>(),
												std::declval<Indices>())
			)
		)
		{
			vector_invoker::unary_invoke_impl(vec, std::forward<Func>(func), Indices{});
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				... Args>
			requires (sizeof...(Args) == N)
		constexpr static void invoke(Vec& vec, Func&& func, Args&&...args)
		noexcept(
			noexcept(
				vector_invoker::binary_invoke_impl(
													std::declval<decltype(vec)>(),
													std::declval<decltype(func)>(),
													std::declval<Indices>(),
													std::declval<Args>()...)
			)
		)
		{
			vector_invoker::binary_invoke_impl(vec, std::forward<Func>(func), Indices{}, std::forward<Args>(args)...);
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec1, typename Vec2, typename
				Func>
		constexpr static void invoke_vec(Vec1& vec1, Func&& func, Vec2& vec2)
		noexcept(
			noexcept(
				vector_invoker::binary_invoke_impl(
													std::declval<decltype(vec1)>(),
													std::declval<decltype(vec2)>(),
													std::declval<decltype(func)>(),
													std::declval<Indices>())
			)
		)
		{
			vector_invoker::binary_invoke_impl(vec1, vec2, std::forward<Func>(func), Indices{});
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				T>
		constexpr static void invoke_dup(Vec& vec, Func& func, T dup_value)
		{
			vector_invoker::binary_invoke_impl(vec, func, Indices{}, dup_value);
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func>
		constexpr static bool invoke_r(Vec& vec, Func&& func)
		noexcept(
			noexcept(
				vector_invoker::unary_invoke_impl_r(
													std::declval<decltype(vec)>(),
													std::declval<decltype(func)>(),
													std::declval<Indices>())
			)
		)
		{
			return vector_invoker::unary_invoke_impl_r(vec, std::forward<Func>(func), Indices{});
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				... Args>
			requires (sizeof...(Args) == N)
		constexpr static bool invoke_r(Vec& vec, Func&& func, Args&&...args)
		noexcept(
			noexcept(
				vector_invoker::binary_invoke_impl_r(
													std::declval<decltype(vec)>(),
													std::declval<decltype(func)>(),
													std::declval<Indices>(),
													std::declval<Args>()...)
			)
		)
		{
			return vector_invoker::binary_invoke_impl_r(vec,
														std::forward<Func>(func),
														Indices{},
														std::forward<Args>(args)...);
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec1, typename Vec2, typename
				Func>
		constexpr static bool invoke_vec_r(Vec1& vec1, Func&& func, Vec2& vec2)
		noexcept(
			noexcept(
				vector_invoker::binary_invoke_impl_r(
													std::declval<decltype(vec1)>(),
													std::declval<decltype(vec2)>(),
													std::declval<decltype(func)>(),
													std::declval<Indices>())
			)
		)
		{
			return vector_invoker::binary_invoke_impl_r(vec1, vec2, std::forward<Func>(func), Indices{});
		}

		template <std::size_t N, typename Indices = std::make_index_sequence<N>, typename Vec, typename Func, typename
				T>
		constexpr static bool invoke_dup_r(Vec& vec, Func& func, T dup_value)
		{
			return vector_invoker::binary_invoke_impl_r(vec, func, Indices{}, dup_value);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_add(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value += static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_subtract(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value -= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_multi(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value *= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_div(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			value /= static_cast<ValueType>(scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_model(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
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
		constexpr static void operator_and(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
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
		constexpr static void operator_or(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
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
		constexpr static void operator_xor(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
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
		constexpr static void operator_left_shift(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			vector_invoker::shift_impl<true>(value, scalar);
		}

		template <typename ValueType, typename T>
			requires std::is_convertible_v<T, ValueType>
		constexpr static void operator_right_shift(
			ValueType& value,
			T          scalar
			) noexcept(std::is_nothrow_convertible_v<T, ValueType>)
		{
			vector_invoker::shift_impl<false>(value, scalar);
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
				return false;
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

	template <arithmetic T, std::size_t N>
	class vector
	{
	public:
		using container_type = std::array<T, N>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;

		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		constexpr static size_type data_size = N;

		using self_type = vector<value_type, data_size>;
		using self_reference = vector<value_type, data_size>&;
		using self_const_reference = const vector<value_type, data_size>&;
		using self_rreference = vector<value_type, data_size>&&;

		/**
		 * @brief default construction
		*/
		constexpr vector() = default;

		/**
		 * @brief tuple composition parameter construction
		 * @tparam Tuple Tuple type
		 * @tparam I index sequence
		 * @param tuple args tuple
		*/
		template <typename Tuple, size_type...I>
			requires (sizeof...(I) <= data_size)
		constexpr vector(Tuple&& tuple, std::index_sequence<I...>)
			: data_({static_cast<value_type>(std::get<I>(std::forward<Tuple>(tuple)))...}) {}

		/**
		 * @brief arbitrary parameter construction
		 * It is allowed to use arithmetic data and vector to construct a vector at the same time.
		 * If the provided data cannot satisfy all the data initialization, the remaining data will be initialized by default.
		 * If the provided data is more than the required data, the extra part will be discarded.
		 * @tparam Args args type
		 * @param args args
		*/
		template <typename... Args>
		// requires((vector_trait<std::remove_cvref_t<Args>>::size + ...) >= data_size)
		constexpr explicit vector(Args&&...args)
			:
			// vector(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...), std::make_index_sequence<data_size>{}) {}
			vector(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...),
					std::make_index_sequence<std::min(data_size,
													std::tuple_size_v<decltype(
														std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...))>)>
					{}) {}

		[[nodiscard]] constexpr static size_type size() noexcept
		{
			return data_size;
		}

		[[nodiscard]] constexpr reference operator[](size_type index) noexcept
		{
			return data_[index];
		}

		[[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept
		{
			return data_[index];
		}

		[[nodiscard]] constexpr iterator begin() noexcept
		{
			return data_.begin();
		}

		[[nodiscard]] constexpr const_iterator begin() const noexcept
		{
			return data_.begin();
		}

		[[nodiscard]] constexpr iterator end() noexcept
		{
			return data_.end();
		}

		[[nodiscard]] constexpr const_iterator end() const noexcept
		{
			return data_.end();
		}

		[[nodiscard]] constexpr reverse_iterator rbegin() noexcept
		{
			return data_.rbegin();
		}

		[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
		{
			return data_.rbegin();
		}

		[[nodiscard]] constexpr reverse_iterator rend() noexcept
		{
			return data_.rend();
		}

		[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
		{
			return data_.rend();
		}

		[[nodiscard]] constexpr const_iterator cbegin() const noexcept
		{
			return data_.cbegin();
		}

		[[nodiscard]] constexpr const_iterator cend() const noexcept
		{
			return data_.cend();
		}

		/**
		 * @brief Add a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `add` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `add` the first element of another vector
		 * else:
		 *		each value of the current vector will `add` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator+=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_add<value_type, vector_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename vector_trait<
																						U>::value_type>()
																					)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_add<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_add<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_add<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Subtract a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `subtract` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `subtract` the first element of another vector
		 * else:
		 *		each value of the current vector will `subtract` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator-=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_subtract<value_type, vector_trait<U>::value_type>(
																							std::declval<value_type&>(),
																							std::declval<typename
																								vector_trait<
																									U>::value_type>()
																						)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_subtract<
															value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_subtract<
															value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_subtract<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Multiply a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `multiply` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `multiply` the first element of another vector
		 * else:
		 *		each value of the current vector will `multiply` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator*=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_multi<value_type, vector_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename
																							vector_trait<
																								U>::value_type>()
																						)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_multi<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_multi<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_multi<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Divide a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `divide` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `divide` the first element of another vector
		 * else:
		 *		each value of the current vector will `divide` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator/=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_div<value_type, vector_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename vector_trait<
																						U>::value_type>()
																					)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_div<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_div<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_div<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Model a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `model` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `model` the first element of another vector
		 * else:
		 *		each value of the current vector will `model` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator%=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_model<value_type, vector_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename
																							vector_trait<
																								U>::value_type>()
																						)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_model<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_model<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_model<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Bit and a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `bit and` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `bit and` the first element of another vector
		 * else:
		 *		each value of the current vector will `bit and` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator&=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_and<value_type, vector_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename vector_trait<
																						U>::value_type>()
																					)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_and<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_and<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_and<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Bit or a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `bit or` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `bit or` the first element of another vector
		 * else:
		 *		each value of the current vector will `bit or` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator|=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_or<value_type, vector_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename vector_trait<
																						U>::value_type>()
																					)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_or<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_or<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_or<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Bit xor a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `bit xor` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `bit xor` the first element of another vector
		 * else:
		 *		each value of the current vector will `bit xor` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator^=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_xor<value_type, vector_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename vector_trait<
																						U>::value_type>()
																					)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_xor<value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_xor<value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_xor<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Left shift a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `left shift` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `left shift` the first element of another vector
		 * else:
		 *		each value of the current vector will `left shift` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator<<=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_left_shift<value_type, vector_trait<U>::value_type>(
																							std::declval<value_type&>(),
																							std::declval<typename
																								vector_trait<
																									U>::value_type>()
																							)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_left_shift<
															value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_left_shift<
															value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_left_shift<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		/**
		 * @brief Right shift a value to the current vector
		 * If the value is also a vector:
		 *		two vectors have the same size: `right shift` the current vector to the corresponding value of another vector
		 *		otherwise: each value of the current vector will `right shift` the first element of another vector
		 * else:
		 *		each value of the current vector will `right shift` the value
		 * @tparam U value type
		 * @param scalar value
		 * @return this
		*/
		template <typename U>
		constexpr self_reference operator>>=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_right_shift<value_type, vector_trait<U>::value_type>(
																							std::declval<value_type&>(),
																							std::declval<typename
																								vector_trait<
																									U>::value_type>()
																							)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					vector_invoker::invoke_vec<data_size>(
														data_,
														vector_invoker::operator_right_shift<
															value_type, trait::value_type>,
														scalar
														);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<data_size>(
														data_,
														vector_invoker::operator_right_shift<
															value_type, trait::value_type>,
														scalar[0]
														);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<data_size>(
													data_,
													vector_invoker::operator_right_shift<value_type, trait::value_type>,
													scalar
													);
			}

			return *this;
		}

		template <typename U>
		[[nodiscard]] constexpr bool operator==(const U& value) const
		noexcept(
			noexcept(
				vector_invoker::operator_equal_to<value_type, vector_trait<U>::value_type>(
																							std::declval<const
																								value_type&>(),
																							std::declval<const typename
																								vector_trait<
																									U>::value_type&>()
																						)
			)
		)
		{
			using trait = vector_trait<U>;
			if constexpr (trait::value)
			{
				// value is a vector
				if constexpr (trait::size == data_size)
				{
					// have the same size
					return vector_invoker::invoke_vec_r<data_size>(
																	data_,
																	vector_invoker::operator_equal_to<
																		value_type, trait::value_type>,
																	value
																);
				}
				else
				{
					// duplicate value 's first element it
					return vector_invoker::invoke_dup_r<data_size>(
																	data_,
																	vector_invoker::operator_equal_to<
																		value_type, trait::value_type>,
																	value[0]
																);
				}
			}
			else
			{
				// duplicate value
				return vector_invoker::invoke_dup_r<data_size>(
																data_,
																vector_invoker::operator_equal_to<
																	value_type, trait::value_type>,
																value
															);
			}
		}

		template <typename U>
		[[nodiscard]] constexpr bool operator!=(const U& value) const
		noexcept(noexcept(std::declval<self_type>().operator==(std::declval<const U&>())))
		{
			return !this->operator==(value);
		}

		constexpr self_reference operator--()
		noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<value_type>())))
		{
			this->operator-=(1);
			return *this;
		}

		[[nodiscard]] constexpr self_type operator--(int)
		noexcept(noexcept(std::declval<self_type>().operator--()))
		{
			auto copy{*this};
			this->operator--();
			return copy;
		}

		constexpr self_reference operator++()
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<value_type>())))
		{
			this->operator+=(1);
			return *this;
		}

		[[nodiscard]] constexpr self_type operator++(int)
		noexcept(noexcept(std::declval<self_type>().operator++()))
		{
			auto copy{*this};
			this->operator++();
			return copy;
		}

		[[nodiscard]] constexpr auto operator-() const
		noexcept(
			noexcept(
				vector_invoker::operator_unary_minus<value_type>(
																std::declval<value_type&>()
																)
			)
		)
		{
			if constexpr (std::is_unsigned_v<value_type>)
			{
				using signed_type = std::make_signed_t<value_type>;
				vector<signed_type, data_size> copy{*this};
				vector_invoker::invoke<data_size>(
												copy,
												vector_invoker::operator_unary_minus<value_type>
												);
				return copy;
			}
			else
			{
				self_type copy{*this};
				vector_invoker::invoke<data_size>(
												copy,
												vector_invoker::operator_unary_minus<value_type>
												);
				return copy;
			}
		}

		[[nodiscard]] constexpr self_type operator~() const
		noexcept(
			noexcept(
				vector_invoker::operator_unary_tilde<value_type>(
																std::declval<value_type&>()
																)
			)
		)
		{
			self_type copy{*this};
			vector_invoker::invoke<data_size>(
											copy,
											vector_invoker::operator_unary_tilde<value_type>
											);
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator+(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy += scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator+(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator+=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy += self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator-(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy -= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator-(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator-=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy -= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator*(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy *= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator*(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator*=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy *= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator/(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy /= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator/(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator/=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy /= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator%(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy %= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator%(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator%=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy %= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator&(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy &= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator&(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator&=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy &= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator|(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy |= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator|(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator|=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy |= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator^(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy ^= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator^(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator^=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy ^= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator<<(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy <<= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator<<(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator<<=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy <<= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr self_type operator>>(const U& scalar) const
		noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<const U&>())))
		{
			auto copy{*this};
			copy >>= scalar;
			return copy;
		}

		template <arithmetic U>
		[[nodiscard]] friend constexpr vector<U, data_size> operator>>(const U& scalar, self_const_reference self)
		noexcept(noexcept(std::declval<vector<U, data_size>>().operator>>=(std::declval<self_const_reference>())))
		{
			vector<U, data_size> copy{duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}};
			copy >>= self;
			return copy;
		}

	private:
		container_type data_;
	};
}
