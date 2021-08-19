#pragma once

#include "common_fwd.hpp"
#include <tuple>

namespace gal::test::math
{
	template<typename T>
	struct common_trait
	{
		using value_type = T;
		constexpr static bool value = false;
		constexpr static std::size_t size = 1;
	};

	template <typename T>
	struct vector_trait
	{
		using trait = common_trait<T>;

		using value_type = typename trait::value_type;
		constexpr static bool        value = trait::value;
		constexpr static std::size_t size = trait::size;
	};

	template <arithmetic T, std::size_t N>
	struct vector_trait<vector<T, N>>
	{
		using value_type = T;
		constexpr static bool        value = true;
		constexpr static std::size_t size = N;
	};

	template<typename T>
	struct matrix_trait
	{
		using trait = common_trait<T>;

		using value_type = typename trait::value_type;
		constexpr static bool        value = trait::value;
		constexpr static std::size_t size = trait::size;
	};

	template<arithmetic T, std::size_t Row, std::size_t Column>
	struct matrix_trait<matrix<T, Row, Column>>
	{
		using value_type = T;
		constexpr static bool value = true;
		constexpr static std::size_t size = Row * Column;
	};

	/**
	 * @brief construction assistance: take out the vector/matrix data and put it into a tuple
	 * @tparam T arg type
	 * @param arg arg
	 * @return tuple(rvalue reference) containing all data
	*/
	template <typename T>
	constexpr auto to_rref_tuple(T&& arg) noexcept
	{
		using type = std::remove_cvref_t<T>;
		if constexpr (vector_trait<type>::value || matrix_trait<type>::value)
		{
			using trait = std::conditional_t<vector_trait<type>::value, vector_trait<type>, matrix_trait<type>>;
			return[&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return std::forward_as_tuple(std::forward<T>(arg)[I]...);
			}(std::make_index_sequence<trait::size>{});
		}
		else
		{
			return std::forward_as_tuple(std::forward<T>(arg));
		}
	}

	/**
	 * @brief construction assistance: take out the vector/matrix data and put it into a tuple
	 * @tparam T arg type
	 * @param arg arg
	 * @return tuple containing all data
	*/
	template <typename T>
	constexpr auto to_tuple(const T& arg) noexcept
	{
		using type = std::remove_cvref_t<T>;
		if constexpr (vector_trait<type>::value || matrix_trait<type>::value)
		{
			using trait = std::conditional_t<vector_trait<type>::value, vector_trait<type>, matrix_trait<type>>;
			return[&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return std::make_tuple(arg[I]...);
			}(std::make_index_sequence<trait::size>{});
		}
		else
		{
			return std::make_tuple(arg);
		}
	}

	/**
	 * @brief get N elements in a continuous range starting from begin
	 * @tparam N the number of elements
	 * @tparam Iterator iterator type
	 * @param begin iterator begin
	 * @param stride stride
	 * @return tuple containing all data
	*/
	template<std::size_t N, typename Iterator>
	constexpr auto to_tuple(Iterator begin, std::size_t stride = 1)
	{
		return [&]<std::size_t... I>(std::index_sequence<I...>)
		{
			return std::tuple_cat(to_tuple(*std::next(begin, stride * I))...);
		}(std::make_index_sequence<N>{});
	}

	/**
	 * @brief construction assistance: can be used when trying to assign all values of the entire container with one parameter
	 * @tparam T arg type
	 * @param arg arg
	 * @return tuple containing repeated parameters
	*/
	template <std::size_t N, typename T>
	constexpr auto duplicate(const T& arg) noexcept
	{
		if constexpr (N == 0)
		{
			return std::make_tuple();
		}
		else
		{
			using type = std::remove_cvref_t<T>;
			if constexpr (vector_trait<type>::value || matrix_trait<type>::value)
			{
				using trait = std::conditional_t<vector_trait<type>::value, vector_trait<type>, matrix_trait<type>>;
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
}
