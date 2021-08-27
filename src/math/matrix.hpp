#pragma once

#include <array>

#include "vector.hpp"
#include "math_fwd.hpp"
#include "../utils/tuple_maker.hpp"
#include "../utils/sequence_invoker.hpp"
#include "../iterator/stride_iterator.hpp"

namespace gal::test::math
{
	template <typename T, std::size_t Row, std::size_t Column>
	class matrix;

	template <typename T>
	struct is_matrix : std::false_type {};

	template <typename T, std::size_t Row, std::size_t Column>
	struct is_matrix<matrix<T, Row, Column>> : std::true_type {};

	template <typename T>
	constexpr static bool is_matrix_v = is_matrix<T>::value;

	template <std::size_t Stride, std::size_t Row, std::size_t Column, typename Iterator>
	class matrix_view;

	template <typename T>
	struct is_matrix_view : std::false_type {};

	template <std::size_t Stride, std::size_t Row, std::size_t Column, typename Iterator>
	struct is_matrix_view<matrix_view<Stride, Row, Column, Iterator>> : std::true_type {};

	template <typename T>
	constexpr static bool is_matrix_view_v = is_matrix_view<T>::value;

	template <typename T>
	struct matrix_value_type_helper
	{
		using value_type = T;
	};

	template <typename T>
		requires (is_matrix_v<T> || is_matrix_view_v<T>)
	struct matrix_value_type_helper<T>
	{
		using value_type = typename T::value_type;
	};

	/**
	 * @brief if the target is a matrix, get the value_type of the matrix, otherwise the value_type is T,
	 * which can (and generally used) prevent the compilation of operations that do not satisfy std::is_convertible_v<T, value_type>
	 * @tparam T matrix_type/class_type
	*/
	template <typename T>
	using matrix_value_type = typename matrix_value_type_helper<T>::value_type;

	/**
	 * @brief specialize integral and floating_point to support corresponding operations
	 * @tparam T matrix/matrix_view
	*/
	template <typename T>
		requires
		(is_matrix_v<T> || is_matrix_view_v<T>) &&
		(
			std::is_integral_v<matrix_value_type<T>> ||
			std::is_floating_point_v<matrix_value_type<T>>
		)
	struct math_invoker_trait<T> : std::true_type
	{
		using value_type = matrix_value_type<T>;

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

		constexpr static bool left_product  = true;
		constexpr static bool right_product = true;
		constexpr static bool inner_product = true;
		constexpr static bool outer_product = true;
	};
}

namespace gal::test::utils
{
	/**
	 * @brief specialize the matrix/matrix_view to support the construction of other matrix
	 * @tparam T matrix/matrix_view
	*/
	template <typename T>
		requires (math::is_matrix_v<T> || math::is_matrix_view_v<T>)
	struct tuple_maker_trait<T> : std::true_type
	{
		using value_type = typename T::value_type;
		constexpr static std::size_t size = T::data_size;
	};
}

namespace gal::test::math
{
	template <typename T, std::size_t Row, std::size_t Column>
	class matrix
	{
	public:
		using container_type = std::array<T, Row * Column>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;

		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

		constexpr static size_type row_size    = Column;
		constexpr static size_type column_size = Row;
		constexpr static size_type data_size   = row_size * column_size;

		using self_type = matrix<value_type, column_size, row_size>;
		using self_reference = matrix<value_type, column_size, row_size>&;
		using self_const_reference = const matrix<value_type, column_size, row_size>&;
		using self_rreference = matrix<value_type, column_size, row_size>&&;

		using row_type = vector<value_type, row_size>;
		using column_type = vector<value_type, column_size>;

		using row_view_type = typename row_type::template view_type<1, iterator>;
		using const_row_view_type = typename row_type::template const_view_type<1, const_iterator>;
		using column_view_type = typename column_type::template view_type<row_size, iterator>;
		using const_column_view_type = typename column_type::template const_view_type<row_size, const_iterator>;

		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value,
			"there is no specialization of matrix<value_type> to math_invoker_trait, all operations will be unavailable"
		);

		/**
		 * @brief default construction
		*/
		constexpr matrix() noexcept(std::is_nothrow_default_constructible_v<container_type>) = default;

		/**
		 * @brief tuple composition parameter construction
		 * @tparam Tuple Tuple type
		 * @tparam I index sequence
		 * @param tuple args tuple
		*/
		template <typename Tuple, size_type...I>
			requires (sizeof...(I) <= data_size)
		constexpr matrix(Tuple&& tuple, std::index_sequence<I...>)
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
		// requires((matrix_trait<std::remove_cvref_t<Args>>::size + ...) >= data_size)
		constexpr explicit matrix(Args&&...args)
		noexcept(noexcept(std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...)))
			:
			// matrix(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...), std::make_index_sequence<data_size>{}) {}
			matrix(std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...),
					std::make_index_sequence<std::min(data_size,
													std::tuple_size_v<decltype(
														std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<
																											Args>(args))
																		...))>)>
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

		template <typename U>
			requires std::is_convertible_v<U, value_type> && invoker_trait_type::equal_to
		[[nodiscard]] constexpr friend bool operator==(const self_type& lhs, const matrix<U, Row, Column>& rhs)
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
		[[nodiscard]] constexpr friend bool operator!=(const self_type& lhs, const matrix<U, Row, Column>& rhs)
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

		[[nodiscard]] constexpr reference get(size_type row, size_type column) noexcept
		{
			return data_[row * row_size + column];
		}

		[[nodiscard]] constexpr const_reference get(size_type row, size_type column) const noexcept
		{
			return data_[row * row_size + column];
		}

		[[nodiscard]] constexpr row_type get_row(size_type which_row) const noexcept
		{
			return {
				utils::tuple_maker::to_tuple<row_size>(data_.cbegin() + which_row * row_size),
				std::make_index_sequence<row_size>{}
			};
		}

		[[nodiscard]] constexpr row_view_type get_row_view(size_type which_row) noexcept
		{
			return row_view_type{data_.begin() + which_row * row_size};
		}

		[[nodiscard]] constexpr const_row_view_type get_row_view(size_type which_row) const noexcept
		{
			return const_row_view_type{data_.cbegin() + which_row * row_size};
		}

		[[nodiscard]] constexpr column_type get_column(size_type which_column) const noexcept
		{
			return {
				utils::tuple_maker::to_tuple<column_size>(data_.cbegin() + which_column, row_size),
				std::make_index_sequence<column_size>{}
			};
		}

		[[nodiscard]] constexpr column_view_type get_column_view(size_type which_column) noexcept
		{
			return column_view_type{data_.begin() + which_column};
		}

		[[nodiscard]] constexpr const_column_view_type get_column_view(size_type which_column) const noexcept
		{
			return const_column_view_type{data_.cbegin() + which_column};
		}

	private:
		container_type data_;
	};

	template <std::size_t Stride, std::size_t Row, std::size_t Column, typename Iterator>
	class matrix_view
	{
	public:
		constexpr static auto stride      = Stride;
		constexpr static auto row_size    = Column;
		constexpr static auto column_size = Row;
		constexpr static auto data_size   = Row * Column;

		using view_iterator = iterator::stride_iterator<Stride, Iterator>;
		using iterator_type = typename view_iterator::iterator_type;
		using value_type = typename view_iterator::value_type;
		using difference_type = typename view_iterator::difference_type;
		using reference = typename view_iterator::reference;
		using rreference = typename view_iterator::rreference;

		using self_type = matrix_view<stride, column_size, row_size, iterator_type>;
		using self_reference = matrix_view<stride, column_size, row_size, iterator_type>&;
		using const_self_reference = const matrix_view<stride, column_size, row_size, iterator_type>&;
		using self_rreference = matrix_view<stride, column_size, row_size, iterator_type>&&;

		using matrix_type = matrix<value_type, column_size, row_size>;
		using size_type = typename matrix_type::size_type;

		using row_type = typename matrix_type::row_type;
		using column_type = typename matrix_type::column_type;

		using row_view_type = typename matrix_type::row_view_type;
		using const_row_view_type = typename matrix_type::const_row_view_type;
		using column_view_type = typename matrix_type::column_view_type;
		using const_column_view_type = typename matrix_type::const_column_view_type;

		using math_invoker_type = math_invoker<value_type>;
		using invoker_trait_type = math_invoker_trait<self_type>;
		static_assert(invoker_trait_type::value,
			"there is no specialization of matrix<value_type> to math_invoker_trait, all operations will be unavailable"
		);

		constexpr explicit matrix_view(iterator_type iterator) : iterator_(std::move(iterator), data_size) {}

		[[nodiscard]] constexpr matrix_type copy_matrix() const noexcept
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
		 * @brief Add a value to the current matrix
		 * If the value is also a matrix:
		 *		two matrix have the same size(the same row_size and column_size): `add` the current matrix to the corresponding value of another matrix
		 *		otherwise: each value of the current matrix will `add` the first element of another matrix
		 * else if the value is a vector:
		 *		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		 *		the user should use matrix.get_row/get_column and then perform the corresponding operation
		 * else:
		 *		each value of the current matrix will `add` the value
		 * @tparam U value type
		 * @param scalar value
		*/
		template <typename U>
		constexpr void operator+=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_add<matrix_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<matrix_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::add
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_add<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_add<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Subtract a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `subtract` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `subtract` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `subtract` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator-=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_subtract<matrix_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<matrix_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::subtract
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_subtract<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_subtract<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Multiply a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `multiply` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `multiply` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `multiply` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator*=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_multiply<matrix_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<matrix_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::multiply
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_multiply<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_multiply<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Divide a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `divide` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `divide` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `divide` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator/=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_divide<matrix_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<matrix_value_type<U>>()
																				)
			)
		)
			requires invoker_trait_type::divide
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_divide<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_divide<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Model a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `model` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `model` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `model` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator%=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_model<matrix_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<matrix_value_type<U>>()
																				)
			)
		)
			requires invoker_trait_type::model
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_model<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_model<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Bit and a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `bit and` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `bit and` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `bit and` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator&=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_and<matrix_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<matrix_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_and
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_and<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_and<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Bit or a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `bit or` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `bit or` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `bit or` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator|=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_or<matrix_value_type<U>>(
																			std::declval<value_type&>(),
																			std::declval<matrix_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_or
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_or<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_or<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Bit xor a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `bit xor` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `bit xor` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `bit or` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator^=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_xor<matrix_value_type<U>>(
																				std::declval<value_type&>(),
																				std::declval<matrix_value_type<U>>()
																			)
			)
		)
			requires invoker_trait_type::bit_xor
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_xor<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_xor<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Left shift a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `left shift` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `left shift` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `left shift` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator<<=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_left_shift<matrix_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<matrix_value_type<U>>()
																					)
			)
		)
			requires invoker_trait_type::left_shift
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_left_shift<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_left_shift<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		/**
		* @brief Right shift a value to the current matrix
		* If the value is also a matrix:
		*		two matrix have the same size(the same row_size and column_size): `right shift` the current matrix to the corresponding value of another matrix
		*		otherwise: each value of the current matrix will `right shift` the first element of another matrix
		* else if the value is a vector:
		*		corresponding operations are prohibited, because there is no way to determine whether the target vector is a row vector or a column vector,
		*		the user should use matrix.get_row/get_column and then perform the corresponding operation
		* else:
		*		each value of the current matrix will `right shift` the value
		* @tparam U value type
		* @param scalar value
		*/
		template <typename U>
		constexpr void operator>>=(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_right_shift<matrix_value_type<U>>(
																						std::declval<value_type&>(),
																						std::declval<matrix_value_type<
																							U>>()
																					)
			)
		)
			requires invoker_trait_type::right_shift
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					utils::sequence_invoker::invoke_seq<data_size>(
																	iterator_,
																	math_invoker_type::template operator_right_shift<
																		matrix_value_type<U>>,
																	scalar
																);
				}
				else
				{
					// duplicate scalar 's first element it
					utils::sequence_invoker::invoke_dup<data_size>(
																	iterator_,
																	math_invoker_type::template operator_right_shift<
																		matrix_value_type<U>>,
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
																	matrix_value_type<U>>,
																scalar
															);
			}
		}

		template <typename U>
		constexpr bool operator==(const U& scalar)
		noexcept(
			noexcept(
				math_invoker_type::template operator_equal_to<matrix_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<matrix_value_type<
																						U>>()
																					)
			)
		)
			requires invoker_trait_type::equal_to
		{
			if constexpr (is_matrix_v<U> || is_matrix_view_v<U>)
			{
				// scalar is a matrix
				if constexpr (U::data_size == data_size && U::row_size == row_size && U::column_size == column_size)
				{
					// have the same size
					return utils::sequence_invoker::invoke_seq_r<data_size>(
																			iterator_,
																			math_invoker_type::template
																			operator_equal_to<
																				matrix_value_type<U>>,
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
																				matrix_value_type<U>>,
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
																			matrix_value_type<U>>,
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
				math_invoker_type::template operator_equal_to<matrix_value_type<U>>(
																					std::declval<value_type&>(),
																					std::declval<matrix_value_type<U>>()
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
		[[nodiscard]] constexpr matrix_type operator+(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator+=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() += scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator+(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator+=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() += self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator-(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator-=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() -= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator-(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator-=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() -= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator*(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator*=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() *= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator*(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator*=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() *= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator/(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator/=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() /= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator/(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator/=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() /= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator%(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator%=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() %= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator%(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator%=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() %= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator&(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator&=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() &= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator&(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator&=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() &= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator|(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator|=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() |= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator|(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator|=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() |= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator^(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator^=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() ^= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator^(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator^=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() ^= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator<<(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator<<=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() <<= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator<<(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator<<=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() <<= self;
			return copy;
		}

		template <typename U>
		[[nodiscard]] constexpr matrix_type operator>>(const U& scalar) const
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator>>=(std::declval<const U&>())
			)
		)
		{
			auto copy = copy_matrix();
			copy.to_view() >>= scalar;
			return copy;
		}

		template <typename U>
			requires (!is_matrix_view_v<U>)
		[[nodiscard]] constexpr friend auto operator>>(const U& scalar, const_self_reference self)
		noexcept(
			noexcept(
				std::declval<self_type>().copy_matrix()
			) &&
			noexcept(
				std::declval<self_type>().operator>>=(std::declval<const_self_reference>())
			)
		)
		{
			matrix<matrix_value_type<U>, column_size, row_size> copy{
				utils::tuple_maker::duplicate<data_size>(scalar), std::make_index_sequence<data_size>{}
			};
			copy.to_view() >>= self;
			return copy;
		}

	private:
		view_iterator iterator_;
	};
}
