#pragma once

#include "vector.hpp"
#include "matrix.hpp"
#include <numeric>
#include <algorithm>

namespace gal::test::math
{
	template <math_type_t T>
	constexpr math_value_type<T> sum(const T& t) noexcept
	{
		return [&]<std::size_t...I>(std::index_sequence<I...>)
		{
			return (t[I] + ...);
		}(std::make_index_sequence<T::data_size>{});
	}

	template <math_type_t T, typename Pred, typename Projection = std::identity>
	constexpr bool all(const T& t, Pred pred, Projection projection = {})
	noexcept(noexcept(
		std::ranges::all_of(
							std::declval<const T&>(),
							std::declval<Pred>(),
							std::declval<Projection>()
							)
	))
	{
		return std::ranges::all_of(t, pred, projection);
	}

	template <math_type_t T>
		requires std::is_same_v<math_value_type<T>, bool>
	constexpr bool all(const T& t)
	noexcept(noexcept(
		std::ranges::all_of(
							std::declval<const T&>(),
							std::declval<decltype(
								[](const bool b)
								{
									return b;
								}
							)>()
							)
	))
	{
		return std::ranges::all_of(t,
									[](const bool b)
									{
										return b;
									});
	}

	template <math_type_t T, typename Pred, typename Projection = std::identity>
	constexpr bool any(const T& t, Pred pred, Projection projection = {})
	noexcept(noexcept(
		std::ranges::any_of(
							std::declval<const T&>(),
							std::declval<Pred>(),
							std::declval<Projection>()
							)
	))
	{
		return std::ranges::any_of(t, pred, projection);
	}

	template <math_type_t T>
		requires std::is_same_v<math_value_type<T>, bool>
	constexpr bool any(const T& t)
	noexcept(noexcept(
		std::ranges::any_of(
							std::declval<const T&>(),
							std::declval<decltype(
								[](const bool b)
								{
									return b;
								}
							)>()
							)
	))
	{
		return std::ranges::any_of(t,
									[](const bool b)
									{
										return b;
									});
	}

	template <math_type_t T, typename Pred, typename Projection = std::identity>
	constexpr bool none(const T& t, Pred pred, Projection projection = {})
	noexcept(noexcept(
		std::ranges::none_of(
							std::declval<const T&>(),
							std::declval<Pred>(),
							std::declval<Projection>()
							)
	))
	{
		return std::ranges::none_of(t, pred, projection);
	}

	template <math_type_t T>
		requires std::is_same_v<math_value_type<T>, bool>
	constexpr bool none(const T& t)
	noexcept(noexcept(
		std::ranges::none_of(
							std::declval<const T&>(),
							std::declval<decltype(
								[](const bool b)
								{
									return b;
								}
							)>()
							)
	))
	{
		return std::ranges::none_of(t,
									[](const bool b)
									{
										return b;
									});
	}
}
