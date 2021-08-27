#pragma once

#include <array>

#include "math_fwd.hpp"
#include "../utils/tuple_maker.hpp"
#include "../utils/sequence_invoker.hpp"
#include "../iterator/stride_iterator.hpp"

namespace gal::test::math
{
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

	template<typename T>
	struct vector_value_type_helper
	{
		using value_type = T;
	};

	template<typename T>
	requires (is_vector_v<T> || is_vector_view_v<T>)
	struct vector_value_type_helper<T>
	{
		using value_type = typename T::value_type;
	};

	/**
	 * @brief if the target is a vector, get the value_type of the vector, otherwise the value_type is T,
	 * which can (and generally used) prevent the compilation of operations that do not satisfy std::is_convertible_v<T, value_type>
	 * @tparam T vector_type/class_type
	*/
	template <typename T>
	using vector_value_type = typename vector_value_type_helper<T>::value_type;

	/**
	 * @brief specialize integral and floating_point to support corresponding operations
	 * @tparam T vector/vector_view
	 * @note if you want to include non-arithmetic vectors to support corresponding operations, you need to specialize math_invoker_trait<vector<my_type>> and give all corresponding values(see below)
	*/
	template <typename T>
		requires
		(is_vector_v<T> || is_vector_view_v<T>) &&
		(
			std::is_integral_v<vector_value_type<T>> ||
			std::is_floating_point_v<vector_value_type<T>>
		)
	struct math_invoker_trait<T> : std::true_type
	{
		using value_type = vector_value_type<T>;

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
}

namespace gal::test::utils
{
	/**
	 * @brief specialize the vector/vector_view to support the construction of other vectors
	 * @tparam T vector/vector_view
	*/
	template<typename T>
	requires (math::is_vector_v<T> || math::is_vector_view_v<T>)
	struct tuple_maker_trait<T> : std::true_type
	{
		using value_type = typename T::value_type;
		constexpr static std::size_t size = T::data_size;
	};
}

namespace gal::test::math
{
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
		using const_self_reference = const vector<value_type, data_size>&;
		using self_rreference = vector<value_type, data_size>&&;

		// Stride may not be 1, and even Iterator is not std::array<value_type, data_size>::iterator but std::array<value_type, N>::iterator (N != data_size)
		template <std::size_t Stride, typename Iterator = iterator>
		using view_type = vector_view<Stride, data_size, Iterator>;
		// Stride may not be 1, and even Iterator is not std::array<value_type, data_size>::const_iterator but std::array<value_type, N>::const_iterator (N != data_size)
		template <std::size_t Stride, typename Iterator = const_iterator>
		using const_view_type = vector_view<Stride, data_size, Iterator>;

		using math_invoker_type = math_invoker<value_type>;
		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value,
			"there is no specialization of vector<value_type> to math_invoker_trait, all operations will be unavailable"
		);

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
		noexcept(noexcept(std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...)))
			:
			// vector(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...), std::make_index_sequence<data_size>{}) {}
			vector(
					std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...),
					std::make_index_sequence<std::min(
													data_size,
													std::tuple_size_v<decltype(
														std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<
																											Args>(args))
																		...))>
													)>{}
				) {}

		template <std::size_t Stride = 1>
		[[nodiscard]] constexpr view_type<Stride> to_view() noexcept
		{
			return view_type<Stride>{data_.begin()};
		}

		template <std::size_t Stride = 1>
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
			requires std::is_convertible_v<U, value_type>
		constexpr bool operator!=(const vector<U, data_size>& other) const
		noexcept(noexcept(std::declval<self_type>().operator==(std::declval<const vector<U, data_size>&>())))
			requires invoker_trait_type::equal_to && invoker_trait_type::not_equal_to
		{
			return !(*this == other);
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
		constexpr static auto stride    = Stride;
		constexpr static auto data_size = Size;

		using view_iterator = iterator::stride_iterator<Stride, Iterator>;
		using iterator_type = typename view_iterator::iterator_type;
		using value_type = typename view_iterator::value_type;
		using difference_type = typename view_iterator::difference_type;
		using reference = typename view_iterator::reference;
		using rreference = typename view_iterator::rreference;

		using self_type = vector_view<stride, data_size, iterator_type>;
		using self_reference = vector_view<stride, data_size, iterator_type>&;
		using const_self_reference = const vector_view<stride, data_size, iterator_type>&;
		using self_rreference = vector_view<stride, data_size, iterator_type>&&;

		using vector_type = vector<value_type, data_size>;
		using size_type = typename vector_type::size_type;

		using math_invoker_type = math_invoker<value_type>;
		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value,
			"there is no specialization of vector<value_type> to math_invoker_trait, all operations will be unavailable"
		);

		constexpr explicit vector_view(iterator_type iterator) : iterator_(std::move(iterator), data_size) {}

		[[nodiscard]] constexpr vector_type copy_vector() const noexcept
		{
			return {utils::tuple_maker::to_tuple<data_size>(iterator_), std::make_index_sequence<data_size>{}};
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
		[[nodiscard]] constexpr auto begin() const noexcept
		{
			return iterator_.begin();
		}

		// stl compatible, can also be used for for-loop
		[[nodiscard]] constexpr auto end() const noexcept
		{
			return iterator_.end();
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
				math_invoker_type::template operator_add<vector_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<vector_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::add
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_add<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_add<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_add<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_subtract<vector_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<vector_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::subtract
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_subtract<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_subtract<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_subtract<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_multiply<vector_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<vector_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::multiply
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_multiply<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_multiply<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_multiply<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_divide<vector_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<vector_value_type<U>>()
																				)
			)
		)
			requires invoker_trait_type::divide
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_divide<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_divide<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_divide<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_model<vector_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<vector_value_type<U>>()
																				)
			)
		)
			requires invoker_trait_type::model
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_model<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_model<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_model<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_and<vector_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<vector_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_and
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_and<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_and<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_and<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_or<vector_value_type<U>>(
																			std::declval<value_type&>(),
																			std::declval<vector_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_or
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_or<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_or<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_or<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_xor<vector_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<vector_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_xor
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_xor<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_xor<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_xor<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_left_shift<vector_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<vector_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::left_shift
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_left_shift<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_left_shift<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_left_shift<
																	vector_value_type<U>>,
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
				math_invoker_type::template operator_right_shift<vector_value_type<U>>(
																						std::declval<value_type&>(),
																						std::declval<vector_value_type<
																							U>>()
																					)
			)
		)
			requires invoker_trait_type::right_shift
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_right_shift<
																		vector_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_right_shift<
																		vector_value_type<U>>,
																	scalar[0]
																);
				}
			}
			else
			{
				// duplicate scalar
				utils::sequence_invoker::invoke_dup<data_size>(
																iterator_,
																math_invoker_type::template operator_right_shift<
																	vector_value_type<U>>,
																scalar
															);
			}
		}

		template <typename U>
		constexpr bool operator==(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_equal_to<vector_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<vector_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::equal_to
		{
			if constexpr (is_vector_v<U> || is_vector_view_v<U>)
			{
				// scalar is a vector
				if constexpr (U::data_size == data_size)
				{
					// have the same size
					return utils::sequence_invoker::invoke_seq_r<data_size>(
																			iterator_,
																			math_invoker_type::template
																			operator_equal_to<
																				vector_value_type<U>>,
																			scalar
																			);
				}
				else
				{
					// duplicate scalar 's first element it
					return utils::sequence_invoker::invoke_dup_r<data_size>(
																			iterator_,
																			math_invoker_type::template
																			operator_equal_to<
																				vector_value_type<U>>,
																			scalar[0]
																			);
				}
			}
			else
			{
				// duplicate scalar
				return utils::sequence_invoker::invoke_dup_r<data_size>(
																		iterator_,
																		math_invoker_type::template operator_equal_to<
																			vector_value_type<U>>,
																		scalar
																		);
			}
		}

		template <typename U>
		constexpr friend bool operator==(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().operator==(std::declval<const_self_reference>())
			)
		)
		{
			return self == scalar;
		}

		template <typename U>
		constexpr bool operator!=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_equal_to<vector_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<vector_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::equal_to && invoker_trait_type::not_equal_to
		{
			return !(*this == scalar);
		}

		template <typename U>
		constexpr friend bool operator!=(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().operator==(std::declval<const_self_reference>())
			)
		)
		{
			return self != scalar;
		}

		constexpr void operator-() const
		noexcept(
			noexcept(
				math_invoker_type::template operator_unary_minus(
																std::declval<value_type&>()
																)
			)
		)
			requires invoker_trait_type::unary_minus
		{
			utils::sequence_invoker::invoke<data_size>(
														iterator_,
														math_invoker_type::template operator_unary_minus
													);
		}

		constexpr void operator~() const
		noexcept(
			noexcept(
				math_invoker_type::template operator_unary_tilde(
																std::declval<value_type&>()
																)
			)
		)
			requires invoker_trait_type::unary_tilde
		{
			utils::sequence_invoker::invoke<data_size>(
														iterator_,
														math_invoker_type::template operator_unary_tilde
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
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
			vector<vector_value_type<U>, data_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() >>= self;
			return copy;
		}

	private:
		view_iterator iterator_;
	};
}
