#pragma once

#include <array>

#include "utils.hpp"
#include "../iterator/stride_iterator.hpp"

namespace gal::test::math
{
	//===============================
	template <typename T, std::size_t N>
	class vector;

	template <typename T>
	struct is_vector : std::false_type {};

	template <typename T, std::size_t N>
	struct is_vector<vector<T, N>> : std::true_type {};

	template <typename T>
	constexpr static bool is_vector_v = is_vector<T>::value;

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	class vector_view;

	template <typename T>
	struct is_vector_view : std::false_type {};

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	struct is_vector_view<vector_view<Stride, Size, Iterator>> : std::true_type {};

	template <typename T>
	constexpr static bool is_vector_view_v = is_vector_view<T>::value;

	/**
	 * @brief specialize integral and floating_point to support corresponding operations
	 * @tparam T vector/vector_view
	 * @note if you want to include non-arithmetic vectors to support corresponding operations, you need to specialize math_invoker_trait<vector<my_type>> and give all corresponding values(see below)
	*/
	template <typename T>
	requires
		(is_vector_v<T> || is_vector_view_v<T>) &&
		(
			std::is_integral_v<typename math_trait<T>::value_type> ||
			std::is_floating_point_v<typename math_trait<T>::value_type>
			)
		struct math_invoker_trait<T>;
	//===============================

	/**
	 * @brief specialize the vector to support the construction of other vectors
	 * @tparam T vector's value_type
	 * @tparam N vector's size
	*/
	template <typename T, std::size_t N>
	struct math_trait<vector<T, N>>
	{
		using value_type = T;
		constexpr static bool        value = true;
		constexpr static std::size_t size  = N;
	};

	/**
	 * @brief specialize the vector to support the construction of other vectors
	 * @tparam Stride vector_view's stride
	 * @tparam Size vector_view's size
	 * @tparam Iterator type of vector_view's holding iterator
	*/
	template <std::size_t Stride, std::size_t Size, typename Iterator>
	struct math_trait<vector_view<Stride, Size, Iterator>>
	{
		using value_type = std::iter_value_t<Iterator>;
		constexpr static bool        value = true;
		constexpr static std::size_t size  = Size;
	};

	template <typename T>
		requires
		(is_vector_v<T> || is_vector_view_v<T>) &&
		(
			std::is_integral_v<typename math_trait<T>::value_type> ||
			std::is_floating_point_v<typename math_trait<T>::value_type>
		)
	struct math_invoker_trait<T> : std::true_type
	{
		using value_type = typename math_trait<T>::value_type;

		static_assert(std::is_arithmetic_v<value_type>);

		constexpr static bool add          = true;
		constexpr static bool subtract     = true;
		constexpr static bool multiply     = true;
		constexpr static bool divide       = true;
		constexpr static bool model        = true;
		constexpr static bool bit_and      = std::is_integral_v<value_type>;
		constexpr static bool bit_or       = std::is_integral_v<value_type>;
		constexpr static bool bit_xor      = std::is_integral_v<value_type>;
		constexpr static bool left_shift   = true;
		constexpr static bool right_shift  = true;
		constexpr static bool unary_minus  = std::is_integral_v<value_type> && std::is_signed_v<T>;
		constexpr static bool unary_tilde  = true;
		constexpr static bool equal_to     = true;
		constexpr static bool not_equal_to = true;
	};

	/**
	 * @brief the actual implementer of the vector operation
	*/
	template <typename VectorType>
		requires (is_vector_v<VectorType> || is_vector_view_v<VectorType>)
	struct vector_invoker
	{
		using vector_type = VectorType;
		using value_type = typename vector_type::value_type;
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

		template <bool Left, typename T>
		constexpr static void shift_impl(value_type& value, T scalar) noexcept
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
				if constexpr (std::is_arithmetic_v<value_type>)
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
		template <typename T>
		constexpr static void operator_add(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value += static_cast<value_type>(scalar);
			}
			else
			{
				value += scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_subtract(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value -= static_cast<value_type>(scalar);
			}
			else
			{
				value -= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_multi(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value *= static_cast<value_type>(scalar);
			}
			else
			{
				value *= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_div(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value /= static_cast<value_type>(scalar);
			}
			else
			{
				value /= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_model(
			value_type& value,
			T           scalar
			) noexcept
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
				if constexpr (std::is_arithmetic_v<value_type>)
				{
					value %= static_cast<value_type>(scalar);
				}
				else
				{
					value %= scalar;
				}
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_and(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value &= static_cast<value_type>(scalar);
			}
			else
			{
				value &= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_or(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value |= static_cast<value_type>(scalar);
			}
			else
			{
				value |= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_xor(
			value_type& value,
			T           scalar
			) noexcept
		{
			if constexpr (std::is_arithmetic_v<value_type>)
			{
				value ^= static_cast<value_type>(scalar);
			}
			else
			{
				value ^= scalar;
			}
		}

		// unchecked
		template <typename T>
		constexpr static void operator_left_shift(
			value_type& value,
			T           scalar
			) noexcept
		{
			vector_invoker::template shift_impl<true>(value, scalar);
		}

		// unchecked
		template <typename T>
		constexpr static void operator_right_shift(
			value_type& value,
			T           scalar
			) noexcept
		{
			vector_invoker::template shift_impl<false>(value, scalar);
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

		template <typename T>
		constexpr static bool operator_equal_to(const value_type& v1, const T& v2)
		noexcept
		{
			// todo: floating_point
			if constexpr (std::is_floating_point_v<value_type>)
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

		// Stride may not be 1, and even Iterator is not std::array<value_type, data_size>::iterator but std::array<value_type, N>::iterator (N != data_size)
		template<std::size_t Stride, typename Iterator = iterator>
		using view_type = vector_view<Stride, data_size, Iterator>;
		// Stride may not be 1, and even Iterator is not std::array<value_type, data_size>::const_iterator but std::array<value_type, N>::const_iterator (N != data_size)
		template<std::size_t Stride, typename Iterator = const_iterator>
		using const_view_type = vector_view<Stride, data_size, Iterator>;

		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value, "there is no specialization of vector<value_type> to math_invoker_trait, all operations will be unavailable");
		
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

		template<std::size_t Stride = 1>
		[[nodiscard]] constexpr view_type<Stride> to_view() noexcept
		{
			return view_type<Stride>{data_.begin()};
		}
		
		template<std::size_t Stride = 1>
		[[nodiscard]] constexpr const_view_type<Stride> to_view() const noexcept
		{
			return const_view_type<Stride>{data_.cbegin()};
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
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::equal_to
		[[nodiscard]] constexpr friend bool operator==(const self_type& lhs, const vector<U, data_size>& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs[I])) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::equal_to
		[[nodiscard]] constexpr friend bool operator==(const self_type& lhs, const U& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs)) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::equal_to
		[[nodiscard]] constexpr friend bool operator==(const U& lhs, const self_type& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return rhs == lhs;
		}


		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const self_type& lhs, const vector<U, data_size>& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const self_type& lhs, const U& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::not_equal_to
		[[nodiscard]] constexpr friend bool operator!=(const U& lhs, const self_type& rhs)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return !(lhs == rhs);
		}

	private:
		container_type data_;
	};

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

		using invoker_type = vector_invoker<self_type>;
		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value, "there is no specialization of vector<value_type> to math_invoker_trait, all operations will be unavailable");

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

		// stl compatible, can also be used for for-loop
		[[nodiscard]] constexpr view_iterator begin() const noexcept
		{
			return iterator_;
		}

		// ReSharper disable once CppMemberFunctionMayBeStatic
		// stl compatible, can also be used for for-loop
		[[nodiscard]] constexpr std::default_sentinel_t end() const noexcept
		{
			return std::default_sentinel;
		}

		// /**
		//  * @brief get the underlying iterator
		//  */
		// [[nodiscard]] constexpr const iterator_type& base() const& noexcept
		// {
		// 	return iterator_;
		// }
		//
		// /**
		//  * @brief get the underlying iterator
		//  */
		// [[nodiscard]] constexpr iterator_type base() && noexcept(std::is_nothrow_move_constructible_v<iterator_type>)
		// {
		// 	return std::move(iterator_);
		// }
		//
		// template <std::common_with<iterator_type> I>
		// [[nodiscard]] constexpr friend bool operator==(
		// 	const vector_view& lhs,
		// 	const vector_view<stride, vector_size, I>& rhs
		// 	) noexcept
		// {
		// 	return lhs == rhs;
		// }
		//
		// [[nodiscard]] constexpr friend bool operator==(
		// 	const vector_view& lhs,
		// 	std::default_sentinel_t) noexcept
		// {
		// 	return lhs == std::default_sentinel;
		// }

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
				invoker_type::template operator_add<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																			)
			)
		)
			requires invoker_trait_type::add
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_add<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_add<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_add<trait::value_type>,
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
				invoker_type::template operator_subtract<math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait
																						<
																							U>::value_type>()
																					)
			)
		)
			requires invoker_trait_type::subtract
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_subtract<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_subtract<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_subtract<
																	trait::value_type>,
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
				invoker_type::template operator_multi<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																				)
			)
		)
			requires invoker_trait_type::multiply
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_multi<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_multi<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_multi<
																	trait::value_type>,
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
				invoker_type::template operator_div<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																			)
			)
		)
			requires invoker_trait_type::divide
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_div<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_div<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_div<trait::value_type>,
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
				invoker_type::template operator_model<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																				)
			)
		)
			requires invoker_trait_type::model
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_model<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_model<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_model<
																	trait::value_type>,
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
				invoker_type::template operator_and<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																			)
			)
		)
			requires invoker_trait_type::bit_and
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_and<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_and<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_and<trait::value_type>,
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
				invoker_type::template operator_or<math_trait<U>::value_type>(
																			std::declval<value_type&>(),
																			std::declval<typename math_trait<
																				U>::value_type>()
																			)
			)
		)
			requires invoker_trait_type::bit_or
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_or<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_or<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_or<trait::value_type>,
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
				invoker_type::template operator_xor<math_trait<U>::value_type>(
																				std::declval<value_type&>(),
																				std::declval<typename math_trait<
																					U>::value_type>()
																			)
			)
		)
			requires invoker_trait_type::bit_xor
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_xor<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_xor<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_xor<trait::value_type>,
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
				invoker_type::template operator_left_shift<math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename
																						math_trait<
																							U>::value_type>()
																					)
			)
		)
			requires invoker_trait_type::left_shift
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_left_shift<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_left_shift<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_left_shift<
																	trait::value_type>,
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
				invoker_type::template operator_right_shift<math_trait<U>::value_type>(
																						std::declval<value_type&>(),
																						std::declval<typename
																							math_trait<
																								U>::value_type>()
																					)
			)
		)
			requires invoker_trait_type::right_shift
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					invoker_type::template invoke_vec<vector_size>(
																	iterator_,
																	invoker_type::template operator_right_shift<
																		trait::value_type>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					invoker_type::template invoke_dup<vector_size>(
																	iterator_,
																	invoker_type::template operator_right_shift<
																		trait::value_type>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				invoker_type::template invoke_dup<vector_size>(
																iterator_,
																invoker_type::template operator_right_shift<
																	trait::value_type>,
																scalar
															);
			}
		}

		template <typename U>
		constexpr bool operator==(const U& scalar)
		noexcept(
			noexcept(
				invoker_type::template operator_equal_to<math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait
																						<
																							U>::value_type>()
																					)
			)
		)
			requires invoker_trait_type::equal_to
		{
			using trait = math_trait<U>;
			if constexpr (trait::value)
			{
				// scalar is a vector
				if constexpr (trait::size == vector_size)
				{
					// have the same size
					return invoker_type::template invoke_vec_r<vector_size>(
																			iterator_,
																			invoker_type::template operator_equal_to<
																				trait::value_type>,
																			scalar
																			);
				}
				else
				{
					// duplicate scalar 's first element it
					return invoker_type::template invoke_dup_r<vector_size>(
																			iterator_,
																			invoker_type::template operator_equal_to<
																				trait::value_type>,
																			scalar[0]
																			);
				}
			}
			else
			{
				// duplicate scalar
				return invoker_type::template invoke_dup_r<vector_size>(
																		iterator_,
																		invoker_type::template operator_equal_to<
																			trait::value_type>,
																		scalar
																		);
			}
		}

		template <typename U>
		constexpr void operator!=(const U& scalar)
		noexcept(
			noexcept(
				invoker_type::template operator_equal_to<math_trait<U>::value_type>(
																					std::declval<value_type&>(),
																					std::declval<typename math_trait
																						<
																							U>::value_type>()
																					)
			)
		)
			requires invoker_trait_type::equal_to && invoker_trait_type::not_equal_to
		{
			return !(*this == scalar);
		}

		constexpr void operator-() const
		noexcept(
			noexcept(
				invoker_type::template operator_unary_minus(
															std::declval<value_type&>()
															)
			)
		)
			requires invoker_trait_type::unary_minus
		{
			invoker_type::template invoke<vector_size>(
														iterator_,
														invoker_type::template operator_unary_minus
													);
		}

		constexpr void operator~() const
		noexcept(
			noexcept(
				invoker_type::template operator_unary_tilde(
															std::declval<value_type&>()
															)
			)
		)
			requires invoker_trait_type::unary_tilde
		{
			invoker_type::template invoke<vector_size>(
														iterator_,
														invoker_type::template operator_unary_tilde
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
