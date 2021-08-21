#pragma once

#include <array>

#include "utils.hpp"
#include "../iterator/stride_iterator.hpp"

namespace gal::test::math
{
	//===============================
	template <typename T, std::size_t N>
	class vector;

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	class vector_view;

	/**
	 * @brief specialize vector_invoker_trait for operator support
	 * @note if the operation corresponding to the target type is false, the corresponding operation is not allowed
	 * @tparam T specialized type
	*/
	template <typename T>
	struct vector_invoker_trait
	{
		constexpr static bool is_arithmetic = std::is_arithmetic_v<T>;

		constexpr static bool add          = false;
		constexpr static bool subtract     = false;
		constexpr static bool multiply     = false;
		constexpr static bool divide       = false;
		constexpr static bool model        = false;
		constexpr static bool bit_and      = false;
		constexpr static bool bit_or       = false;
		constexpr static bool bit_xor      = false;
		constexpr static bool left_shift   = false;
		constexpr static bool right_shift  = false;
		constexpr static bool unary_minus  = false;
		constexpr static bool unary_tilde  = false;
		constexpr static bool equal_to     = false;
		constexpr static bool not_equal_to = false;
	};
	//===============================

	template <typename T, std::size_t N>
	struct math_trait<vector<T, N>>
	{
		using value_type = T;
		constexpr static bool        value = true;
		constexpr static std::size_t size  = N;
	};

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	struct math_trait<vector_view<Stride, Size, Iterator>>
	{
		using value_type = std::iter_value_t<Iterator>;
		constexpr static bool        value = true;
		constexpr static std::size_t size  = Size;
	};

	template <std::integral T>
	struct vector_invoker_trait<T>
	{
		static_assert(std::is_arithmetic_v<T>);
		constexpr static bool is_arithmetic = true;

		constexpr static bool add          = true;
		constexpr static bool subtract     = true;
		constexpr static bool multiply     = true;
		constexpr static bool divide       = true;
		constexpr static bool model        = true;
		constexpr static bool bit_and      = true;
		constexpr static bool bit_or       = true;
		constexpr static bool bit_xor      = true;
		constexpr static bool left_shift   = true;
		constexpr static bool right_shift  = true;
		constexpr static bool unary_minus  = std::is_signed_v<T>;
		constexpr static bool unary_tilde  = true;
		constexpr static bool equal_to     = true;
		constexpr static bool not_equal_to = true;
	};

	template <std::floating_point T>
	struct vector_invoker_trait<T>
	{
		static_assert(std::is_arithmetic_v<T>);
		constexpr static bool is_arithmetic = true;

		constexpr static bool add          = true;
		constexpr static bool subtract     = true;
		constexpr static bool multiply     = true;
		constexpr static bool divide       = true;
		constexpr static bool model        = true;
		constexpr static bool bit_and      = false;
		constexpr static bool bit_or       = false;
		constexpr static bool bit_xor      = false;
		constexpr static bool left_shift   = true;
		constexpr static bool right_shift  = true;
		constexpr static bool unary_minus  = true;
		constexpr static bool unary_tilde  = false;
		constexpr static bool equal_to     = true;
		constexpr static bool not_equal_to = true;
	};

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
			if constexpr (std::is_floating_point_v<ValueType>)
			{
				bool s_lt_0 = scalar < 0;
				if (s_lt_0)
				{
					scalar = -scalar;
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
			else
			{
				if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
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
					if constexpr (Left)
					{
						value <<= scalar;
					}
					else
					{
						value >>= scalar;
					}
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

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_add(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value += static_cast<ValueType>(scalar);
			}
			else
			{
				value += scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_subtract(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value -= static_cast<ValueType>(scalar);
			}
			else
			{
				value -= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_multi(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value *= static_cast<ValueType>(scalar);
			}
			else
			{
				value *= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_div(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value /= static_cast<ValueType>(scalar);
			}
			else
			{
				value /= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_model(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (std::is_floating_point_v<ValueType>)
			{
				if (scalar < 0)
				{
					scalar = -scalar;
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
			else
			{
				if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
				{
					value %= static_cast<ValueType>(scalar);
				}
				else
				{
					value %= scalar;
				}
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_and(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value &= static_cast<ValueType>(scalar);
			}
			else
			{
				value &= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_or(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value |= static_cast<ValueType>(scalar);
			}
			else
			{
				value |= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_xor(
			ValueType& value,
			T          scalar
			) noexcept
		{
			if constexpr (vector_invoker_trait<ValueType>::is_arithmetic)
			{
				value ^= static_cast<ValueType>(scalar);
			}
			else
			{
				value ^= scalar;
			}
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_left_shift(
			ValueType& value,
			T          scalar
			) noexcept
		{
			vector_invoker::shift_impl<true>(value, scalar);
		}

		// unchecked
		template <typename ValueType, typename T>
		constexpr static void operator_right_shift(
			ValueType& value,
			T          scalar
			) noexcept
		{
			vector_invoker::shift_impl<false>(value, scalar);
		}

		// unchecked
		template <typename ValueType>
		constexpr static void operator_unary_minus(ValueType& value)
		noexcept
		{
			value = -value;
		}

		// unchecked
		template <typename ValueType>
		constexpr static void operator_unary_tilde(ValueType& value)
		noexcept
		{
			value = ~value;
		}

		template <typename ValueType, typename T>
		constexpr static bool operator_equal_to(const ValueType& v1, const T& v2)
		noexcept
		{
			// todo: floating_point
			if constexpr (std::is_floating_point_v<ValueType>)
			{
				if (static_cast<double>(v1) - static_cast<double>(v2) <= 0.000001 &&
					static_cast<double>(v1) - static_cast<double>(v2) >= -0.000001
				)
				{
					return true;
				}
				return false;
			}
			else
			{
				return v1 == v2;
			}
		}
	};

	template <typename T, std::size_t N>
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

		using view_type = vector_view<1, data_size, iterator>;
		using const_view_type = vector_view<1, data_size, const_iterator>;

		/**
		 * @brief default construction
		*/
		constexpr vector() noexcept(std::is_nothrow_default_constructible_v<container_type>) = default;

		/**
		 * @brief tuple composition parameter construction
		 * @tparam Tuple Tuple type
		 * @tparam I index sequence
		 * @param tuple args tuple
		*/
		template <typename Tuple, size_type...I>
			requires (sizeof...(I) <= data_size)
		constexpr vector(Tuple&& tuple, std::index_sequence<I...>)
		noexcept((noexcept(static_cast<value_type>(std::get<I>(std::forward<Tuple>(tuple)))) && ...))
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
		// requires((math_trait<std::remove_cvref_t<Args>>::size + ...) >= data_size)
		constexpr explicit vector(Args&&...args)
		noexcept(noexcept(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...)))
			:
			// vector(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...), std::make_index_sequence<data_size>{}) {}
			vector(
					std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...),
					std::make_index_sequence<std::min(
													data_size,
													std::tuple_size_v<decltype(
														std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...))>
													)>{}
				) {}

		constexpr view_type to_view() noexcept
		{
			return view_type{data_.begin()};
		}

		[[nodiscard]] constexpr const_view_type to_view() const noexcept
		{
			return const_view_type{data_.cbegin()};
		}

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

		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::equal_to
		[[nodiscard]] constexpr friend bool operator==(const self_type& lhs, const vector<U, data_size>& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs[I])) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::equal_to
		[[nodiscard]] constexpr friend bool operator==(const self_type& lhs, const U& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs)) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::equal_to
		[[nodiscard]] constexpr friend bool operator==(const U& lhs, const self_type& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return rhs == lhs;
		}


		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const self_type& lhs, const vector<U, data_size>& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const self_type& lhs, const U& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && vector_invoker_trait<value_type>::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const U& lhs, const self_type& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

	private:
		container_type data_;
	};

	template <typename T>
	struct is_vector_view : std::false_type {};

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	struct is_vector_view<vector_view<Stride, Size, Iterator>> : std::true_type {};

	template <typename T>
	constexpr static bool is_vector_view_v = is_vector_view<T>::value;

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	class vector_view
	{
	public:
		constexpr static auto stride      = Stride;
		constexpr static auto vector_size = Size;

		using view_iterator = iterator::stride_iterator<Stride, Iterator>;
		using iterator_type = typename view_iterator::iterator_type;
		using value_type = typename view_iterator::value_type;
		using difference_type = typename view_iterator::difference_type;
		using reference = typename view_iterator::reference;
		using rreference = typename view_iterator::rreference;

		using self_type = vector_view<stride, vector_size, iterator_type>;
		using self_reference = vector_view<stride, vector_size, iterator_type>&;
		using const_self_reference = const vector_view<stride, vector_size, iterator_type>&;
		using self_rreference = vector_view<stride, vector_size, iterator_type>&&;

		using vector_type = vector<value_type, vector_size>;
		using size_type = typename vector_type::size_type;

		constexpr explicit vector_view(iterator_type iterator) : iterator_(std::move(iterator), vector_size) {}

		[[nodiscard]] constexpr vector_type copy_vector() const noexcept
		{
			return {to_tuple<vector_size>(iterator_), std::make_index_sequence<vector_size>{}};
		}

		[[nodiscard]] constexpr decltype(auto) operator[](size_type index) noexcept
		{
			return iterator_[index];
		}

		[[nodiscard]] constexpr decltype(auto) operator[](size_type index) const noexcept
		{
			return iterator_[index];
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
		*/
		template <typename U>
		constexpr void operator+=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_add<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::add
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_add<value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_add<value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_add<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator-=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_subtract<value_type, math_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename math_trait
																							<
																								U>::value_type>()
																						)
			)
		)
			requires vector_invoker_trait<value_type>::subtract
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_subtract<
																value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_subtract<
																value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_subtract<
															value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator*=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_multi<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::multiply
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_multi<
																value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_multi<
																value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_multi<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator/=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_div<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::divide
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_div<value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_div<value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_div<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator%=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_model<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::model
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_model<
																value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_model<
																value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_model<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator&=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_and<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::bit_and
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_and<value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_and<value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_and<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator|=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_or<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																				)
			)
		)
			requires vector_invoker_trait<value_type>::bit_or
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_or<value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_or<value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_or<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator^=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_xor<value_type, math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait<
																						U>::value_type>()
																					)
			)
		)
			requires vector_invoker_trait<value_type>::bit_xor
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_xor<value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_xor<value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_xor<value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator<<=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_left_shift<value_type, math_trait<U>::value_type>(
																							std::declval<value_type&>(),
																							std::declval<typename
																								math_trait<
																									U>::value_type>()
																						)
			)
		)
			requires vector_invoker_trait<value_type>::left_shift
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_left_shift<
																value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_left_shift<
																value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_left_shift<
															value_type, trait::value_type>,
														scalar
														);
			}
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
		*/
		template <typename U>
		constexpr void operator>>=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_right_shift<value_type, math_trait<U>::value_type>(
																							std::declval<value_type&>(),
																							std::declval<typename
																								math_trait<
																									U>::value_type>()
																							)
			)
		)
			requires vector_invoker_trait<value_type>::right_shift
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					vector_invoker::invoke_vec<vector_size>(
															iterator_,
															vector_invoker::operator_right_shift<
																value_type, trait::value_type>,
															scalar
															);
				}
				else
				{
					// duplicate scalar 's first element it
					vector_invoker::invoke_dup<vector_size>(
															iterator_,
															vector_invoker::operator_right_shift<
																value_type, trait::value_type>,
															scalar[0]
															);
				}
			}
			else
			{
				// duplicate scalar
				vector_invoker::invoke_dup<vector_size>(
														iterator_,
														vector_invoker::operator_right_shift<
															value_type, trait::value_type>,
														scalar
														);
			}
		}

		template <typename U>
		constexpr bool operator==(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_equal_to<value_type, math_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename math_trait
																							<
																								U>::value_type>()
																						)
			)
		)
			requires vector_invoker_trait<value_type>::equal_to
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					return vector_invoker::invoke_vec_r<vector_size>(
																	iterator_,
																	vector_invoker::operator_equal_to<
																		value_type, trait::value_type>,
																	scalar
																	);
				}
				else
				{
					// duplicate scalar 's first element it
					return vector_invoker::invoke_dup_r<vector_size>(
																	iterator_,
																	vector_invoker::operator_equal_to<
																		value_type, trait::value_type>,
																	scalar[0]
																	);
				}
			}
			else
			{
				// duplicate scalar
				return vector_invoker::invoke_dup_r<vector_size>(
																iterator_,
																vector_invoker::operator_equal_to<
																	value_type, trait::value_type>,
																scalar
																);
			}
		}

		template <typename U>
		constexpr void operator!=(const U& scalar)
		noexcept(
			noexcept(
				vector_invoker::operator_equal_to<value_type, math_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename math_trait
																							<
																								U>::value_type>()
																						)
			)
		)
			requires vector_invoker_trait<value_type>::not_equal_to
		{
			return !(*this == scalar);
		}

		constexpr void operator-() const
		noexcept(
			noexcept(
				vector_invoker::operator_unary_minus<value_type>(
																std::declval<value_type&>()
																)
			)
		)
			requires vector_invoker_trait<value_type>::unary_minus
		{
			vector_invoker::invoke<vector_size>(
												iterator_,
												vector_invoker::operator_unary_minus<value_type>
												);
		}

		constexpr void operator~() const
		noexcept(
			noexcept(
				vector_invoker::operator_unary_tilde<value_type>(
																std::declval<value_type&>()
																)
			)
		)
			requires vector_invoker_trait<value_type>::unary_tilde
		{
			vector_invoker::invoke<vector_size>(
												iterator_,
												vector_invoker::operator_unary_tilde<value_type>
												);
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator+(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator+=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() += scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator+(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator+=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() += self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator-(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator-=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() -= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator-(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator-=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() -= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator*(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator*=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() *= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator*(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator*=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() *= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator/(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator/=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() /= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator/(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator/=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() /= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator%(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator%=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() %= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator%(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator%=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() %= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator&(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator&=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() &= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator&(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator&=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() &= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator|(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator|=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() |= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator|(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator|=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() |= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator^(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator^=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() ^= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator^(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator^=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() ^= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator<<(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator<<=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() <<= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator<<(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator<<=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() <<= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr vector_type operator>>(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator>>=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_vector();
			copy.to_view() >>= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_vector_view_v<U>)
		[[nodiscard]] constexpr friend auto operator>>(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_vector()
			) &&
			noexcept(
				std::declval<self_type>().operator>>=(std::declval<const_self_reference>())
			)
		)
		{
			vector<typename math_trait<U>::value_type, vector_size> copy{
				duplicate<vector_size>(scalar), std::make_index_sequence<vector_size>{}
			};
			copy.to_view() >>= self;
			return copy;
		}

	private:
		view_iterator iterator_;
	};
}
