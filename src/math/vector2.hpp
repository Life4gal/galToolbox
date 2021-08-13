#pragma once

#include "precision.hpp"

#include <array>

namespace gal::test
{
	template <arithmetic T, precision P>
	struct vector<2, T, P>
	{
		using self_type = vector<2, T, P>;
		using self_reference = vector<2, T, P>&;
		using const_self_reference = const vector<2, T, P>&;

		using vector1_type = vector<1, T, P>;

		template <vector_size_type Size, arithmetic U>
		using acceptable_type = vector<Size, U, P>;

		using value_type = T;
		using size_type = vector_size_type;

		using reference = value_type&;
		using const_reference = const value_type&;

		constexpr static size_type data_index = 1;
		constexpr static size_type data_size = 2;

		constexpr          vector() noexcept = default;
		constexpr explicit vector(value_type scalar) noexcept : x(scalar), y(scalar) {}
		constexpr vector(value_type x, value_type y) noexcept : x(x), y(y) {}

		template <vector_size_type Size, typename U>
		requires (Size >= 2)
		constexpr explicit vector(const acceptable_type<Size, U>& other) noexcept
			: x(other[vector1_type::data_index]), y(other[data_index]) {}

		template<vector_size_type Size1, typename U1, vector_size_type Size2, typename U2>
		requires (Size1 >= 1 && Size2 >= 1)
		constexpr vector(const acceptable_type<Size1, U1>& v1, const acceptable_type<Size2, U2>& v2) noexcept
			: x(v1[vector1_type::data_index]), y(v2[vector1_type::data_index]) {}

		template <vector_size_type Size, typename U>
		requires (Size >= 1)
		constexpr vector(const acceptable_type<Size, U>& other, value_type scalar) noexcept
		: x(other[vector1_type::data_index]), y(scalar) {}

		template <vector_size_type Size, typename U>
		requires (Size >= 1)
			constexpr vector(value_type scalar, const acceptable_type<Size, U>& other) noexcept
			: x(scalar), y(other[vector1_type::data_index]) {}

		constexpr static size_type size() noexcept
		{
			return data_size;
		}

		constexpr reference operator[](size_type index) noexcept
		{
			return x;
		}

		constexpr const_reference operator[](size_type index) const noexcept
		{
			return x;
		}
		
		value_type x;
		value_type y;
	};
}
