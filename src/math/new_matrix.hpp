#pragma once

#include <array>
#include "new_vector.hpp"
#include "../utils/tuple_maker.hpp"
#include "../iterator/stride_iterator.hpp"

namespace gal::test::new_math
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

	template<typename T>
	constexpr static bool is_matrix_type = is_matrix_v<T> || is_matrix_view_v<T>;
}

namespace gal::test::utils
{
	/**
	 * @brief specialize the matrix/matrix_view to support the construction of other matrix
	 * @tparam T matrix/matrix_view
	*/
	template <typename T>
		requires (new_math::is_matrix_v<T> || new_math::is_matrix_view_v<T>)
	struct tuple_maker_trait<T> : std::true_type
	{
		using value_type = typename T::value_type;
		constexpr static std::size_t size = T::data_size;
	};
}

namespace gal::test::new_math
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
		using const_self_reference = const matrix<value_type, column_size, row_size>&;
		using self_rreference = matrix<value_type, column_size, row_size>&&;

		template<typename U>
		using copy_type = matrix<U, column_size, row_size>;

		using row_type = vector<value_type, row_size>;
		using column_type = vector<value_type, column_size>;

		using row_view_type = typename row_type::template view_type<1, iterator>;
		using const_row_view_type = typename row_type::template const_view_type<1, const_iterator>;
		using column_view_type = typename column_type::template view_type<row_size, iterator>;
		using const_column_view_type = typename column_type::template const_view_type<row_size, const_iterator>;

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
		constexpr explicit matrix(Args&&...args)
		noexcept(noexcept(std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...)))
			: matrix(std::tuple_cat(utils::tuple_maker::to_rref_tuple(std::forward<Args>(args))...),
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

		template <std::size_t Stride, typename Iterator>
			requires std::is_convertible_v<typename matrix_view<Stride, column_size, row_size, Iterator>::value_type,
											value_type>
		[[nodiscard]] constexpr friend bool operator==(
			const_self_reference                                        lhs,
			const matrix_view<Stride, column_size, row_size, Iterator>& rhs
			)
		noexcept(std::is_nothrow_constructible_v<
			typename matrix_view<Stride, column_size, row_size, Iterator>::value_type, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs[I])) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <std::convertible_to<value_type> U>
		[[nodiscard]] constexpr friend bool operator==(
			const_self_reference                    lhs,
			const matrix<U, column_size, row_size>& rhs
			)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs[I])) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
			requires
			(is_matrix_v<U> || is_matrix_view_v<U>) &&
			(U::data_size == data_size) &&
			std::is_convertible_v<typename U::value_type, value_type>
		[[nodiscard]] constexpr friend bool operator==(const_self_reference lhs, const U& rhs)
		noexcept(std::is_nothrow_constructible_v<typename U::value_type, value_type>)
		{
			return [&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return ((lhs.data_[I] == static_cast<value_type>(rhs[I])) && ...);
			}(std::make_index_sequence<data_size>{});
		}

		template <typename U>
		[[nodiscard]] constexpr friend bool operator!=(const_self_reference lhs, const U& rhs)
		noexcept(noexcept(std::declval<const_self_reference>().operator==(std::declval<const U&>())))
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

		template<typename U>
		using copy_type = matrix<U, column_size, row_size>;

		using row_type = typename matrix_type::row_type;
		using column_type = typename matrix_type::column_type;

		using row_view_type = typename matrix_type::row_view_type;
		using const_row_view_type = typename matrix_type::const_row_view_type;
		using column_view_type = typename matrix_type::column_view_type;
		using const_column_view_type = typename matrix_type::const_column_view_type;

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

	private:
		view_iterator iterator_;
	};
}
