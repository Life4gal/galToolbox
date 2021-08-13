#pragma once

#include "precision.hpp"
#include <array>

namespace gal::test
{
	template <arithmetic T, precision P>
	struct vector<1, T, P>
	{
		using self_type = vector<1, T, P>;
		using self_reference = vector<1, T, P>&;
		using const_self_reference = const vector<1, T, P>&;

		template <vector_size_type Size, arithmetic U>
		using acceptable_type = vector<Size, U, P>;

		using value_type = T;
		using size_type = vector_size_type;

		using reference = value_type&;
		using const_reference = const value_type&;

		constexpr static size_type data_index = 0;
		constexpr static size_type data_size  = 1;

		constexpr          vector() noexcept = default;
		constexpr explicit vector(value_type scalar) noexcept : data_({scalar}) {}

		template <vector_size_type Size, typename U>
			requires (Size >= 1)
		constexpr explicit vector(const acceptable_type<Size, U>& other) noexcept
			: data_({other[data_index]}) {}

		constexpr static size_type size() noexcept
		{
			return data_size;
		}

		constexpr reference operator[](size_type) noexcept
		{
			return data_[data_index];
		}

		constexpr const_reference operator[](size_type) const noexcept
		{
			return data_[data_index];
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator+=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] += static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator-=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] -= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator*=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] *= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator/=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] /= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator%=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] %= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator&=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] &= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator|=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] |= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator^=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] ^= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator<<=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] <<= static_cast<value_type>(scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator>>=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[data_index] >>= static_cast<value_type>(scalar);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator+=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			this->operator+=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator-=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<U>())))
		{
			this->operator-=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator*=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<U>())))
		{
			this->operator*=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator/=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<U>())))
		{
			this->operator/=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator%=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<U>())))
		{
			this->operator%=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator&=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<U>())))
		{
			this->operator&=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator|=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<U>())))
		{
			this->operator|=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator^=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<U>())))
		{
			this->operator^=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator<<=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<U>())))
		{
			this->operator<<=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
		constexpr self_reference operator>>=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<U>())))
		{
			this->operator>>=(other[data_index]);
			return *this;
		}

		constexpr self_reference operator--() noexcept
		{
			--data_[data_index];
			return *this;
		}

		constexpr self_type operator--(int) noexcept
		{
			auto copy{*this};
			this->operator--();
			return copy;
		}

		constexpr self_reference operator++() noexcept
		{
			++data_[data_index];
			return *this;
		}

		constexpr self_type operator++(int) noexcept
		{
			auto copy{*this};
			this->operator++();
			return copy;
		}

		constexpr self_type operator+() const noexcept
		{
			return self_type{data_[data_index]};
		}

		constexpr self_type operator-() const noexcept
		{
			return self_type{-data_[data_index]};
		}

		constexpr self_type operator~() const noexcept
		{
			return self_type{~data_[data_index]};
		}

		template <typename U>
		constexpr self_type operator+(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			auto copy{*this};
			copy += scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator+(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator+=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy += self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator-(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<U>())))
		{
			auto copy{*this};
			copy -= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator-(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator-=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy -= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator*(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<U>())))
		{
			auto copy{*this};
			copy *= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator*(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator*=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy *= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator/(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<U>())))
		{
			auto copy{*this};
			copy /= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator/(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator/=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy /= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator%(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<U>())))
		{
			auto copy{*this};
			copy %= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator%(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator%=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy %= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator&(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<U>())))
		{
			auto copy{*this};
			copy &= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator&(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator&=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy &= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator|(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<U>())))
		{
			auto copy{*this};
			copy |= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator|(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator|=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy |= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator^(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<U>())))
		{
			auto copy{*this};
			copy ^= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator^(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator^=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy ^= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator<<(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<U>())))
		{
			auto copy{*this};
			copy <<= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator<<(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator
			<<=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy <<= self;
			return copy;
		}

		template <typename U>
		constexpr self_type operator>>(U scalar) const
		noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<U>())))
		{
			auto copy{*this};
			copy >>= scalar;
			return copy;
		}

		template <arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator>>(U scalar, const_self_reference self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator
			>>=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{scalar};
			copy >>= self;
			return copy;
		}

		template <typename U>
		constexpr bool operator==(const acceptable_type<data_size, U>& other) const
		{
			return other[data_index] == data_[data_index];
		}

		template <typename U>
		constexpr bool operator!=(const acceptable_type<data_size, U>& other) const
		{
			return !this->operator==(other);
		}

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr explicit(!std::is_same_v<value_type, bool>) operator bool() const noexcept
		{
			return data_[data_index];
		}

	private:
		std::array<value_type, 1> data_;
	};
}
