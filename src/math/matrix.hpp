#pragma once

#include <array>
#include <ranges>

#include "vector.hpp"

namespace gal::test::math
{
	template <arithmetic T, std::size_t Row, std::size_t Column>
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

		using row_type = vector<value_type, row_size>;
		using column_type = vector<value_type, column_size>;

		using self_type = matrix<value_type, column_size, row_size>;
		using self_reference = matrix<value_type, column_size, row_size>&;
		using self_const_reference = const matrix<value_type, column_size, row_size>&;
		using self_rreference = matrix<value_type, column_size, row_size>&&;

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
		noexcept(noexcept(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...)))
			:
			// matrix(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...), std::make_index_sequence<data_size>{}) {}
			matrix(std::tuple_cat(to_rref_tuple(std::forward<Args>(args))...),
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
				to_tuple<row_size>(data_.cbegin() + which_row * row_size), std::make_index_sequence<row_size>{}
			};
		}

		[[nodiscard]] constexpr auto get_row_view(size_type which_row) noexcept
		{
			return data_ | std::views::drop(which_row * row_size) | std::views::take(row_size);
		}

		[[nodiscard]] constexpr auto get_row_view(size_type which_row) const noexcept
		{
			return data_ | std::views::drop(which_row * row_size) | std::views::take(row_size);
		}

		[[nodiscard]] constexpr column_type get_column(size_type which_column) const noexcept
		{
			return {
				to_tuple<column_size>(data_.cbegin() + which_column, row_size), std::make_index_sequence<row_size>{}
			};
		}

		[[nodiscard]] constexpr auto get_column_view(size_type which_column) noexcept
		{
			// todo: we need a way to filter based on the target index or write an iterator supported by ranges and overload operator++
			// return data_ | std::views::drop(which_column) |
			// 	std::views::filter(
			// 						[begin = data_.data() + which_column](const value_type& v) constexpr
			// 						{
			// 							return (std::addressof(v) - begin) % row_size == 0;
			// 						}
			// 					) |
			// 	std::views::take(column_size);
			return
				std::views::iota(data_.begin() + which_column, data_.end() - (row_size - which_column - 1)) |
				std::views::filter(
									[begin = data_.begin() + which_column](iterator it)
									{
										return (it - begin) % row_size == 0;
									}
								) |
				std::views::transform(
									[](iterator it)
									{
										return *it;
									}
									);
		}

		[[nodiscard]] constexpr auto get_column_view(size_type which_column) const noexcept
		{
			// todo: we need a way to filter based on the target index or write an iterator supported by ranges and overload operator++
			// return data_ | std::views::drop(which_column) |
			// 	std::views::filter(
			// 						[begin = data_.data() + which_column](const value_type& v) constexpr
			// 						{
			// 							return (std::addressof(v) - begin) % row_size == 0;
			// 						}
			// 					) |
			// 	std::views::take(column_size);
			return
				std::views::iota(data_.cbegin() + which_column, data_.cend() - (row_size - which_column - 1)) |
				std::views::filter(
									[begin = data_.cbegin() + which_column](const_iterator it)
									{
										return (it - begin) % row_size == 0;
									}
								) |
				std::views::transform(
									[](const_iterator it)
									{
										return *it;
									}
									);
		}

	private:
		container_type data_;
	};
}
