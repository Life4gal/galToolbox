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

		using container_type = std::array<T, 2>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;

		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;

		constexpr static size_type data_index = 1;
		constexpr static size_type data_size = 2;

		constexpr          vector() noexcept = default;
		template<arithmetic U>
		constexpr explicit vector(U scalar) noexcept : data_({static_cast<value_type>(scalar), static_cast<value_type>(scalar) }) {}
		template<arithmetic U1, arithmetic U2>
		constexpr          vector(U1 x, U2 y) noexcept : data_({ static_cast<value_type>(x), static_cast<value_type>(y)}) {}

		template <vector_size_type Size, typename U>
		requires (Size >= data_size)
		constexpr explicit vector(const acceptable_type<Size, U>& other) noexcept
			: data_({static_cast<value_type>(other[vector1_type::data_index]), static_cast<value_type>(other[data_index])}) {}

		template<vector_size_type Size1, typename U1, vector_size_type Size2, typename U2>
		requires (Size1 >= vector1_type::data_size && Size2 >= vector1_type::data_size)
		constexpr vector(const acceptable_type<Size1, U1>& v1, const acceptable_type<Size2, U2>& v2) noexcept
			: data_({static_cast<value_type>(v1[vector1_type::data_index]), static_cast<value_type>(v2[vector1_type::data_index])}) {}

		template <vector_size_type Size, typename U, typename A>
		requires (Size >= vector1_type::data_size) && std::is_convertible_v<U, value_type>and std::is_convertible_v<A, value_type>
		constexpr vector(const acceptable_type<Size, U>& other, A scalar) noexcept(std::is_nothrow_convertible_v<U, value_type> && std::is_nothrow_convertible_v<A, value_type>)
		: data_({static_cast<value_type>(other[vector1_type::data_index]), static_cast<value_type>(scalar)}) {}

		template <vector_size_type Size, typename U, typename A>
		requires (Size >= vector1_type::data_size) && std::is_convertible_v<U, value_type>and std::is_convertible_v<A, value_type>
			constexpr vector(A scalar, const acceptable_type<Size, U>& other) noexcept(std::is_nothrow_convertible_v<U, value_type>&& std::is_nothrow_convertible_v<A, value_type>)
			: data_({ static_cast<value_type>(scalar), static_cast<value_type>(other[vector1_type::data_index]) }) {}

		constexpr static size_type size() noexcept
		{
			return data_size;
		}

		constexpr reference operator[](size_type index) noexcept
		{
			return data_[index];
		}

		constexpr const_reference operator[](size_type index) const noexcept
		{
			return data_[index];
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator+=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] += static_cast<value_type>(scalar);
			data_[data_index] += static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
		constexpr self_reference operator+=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			if constexpr(Size == vector1_type::data_size)
			{
				this->operator+=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] += static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] += static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator-=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] -= static_cast<value_type>(scalar);
			data_[data_index] -= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator-=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator-=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] -= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] -= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator*=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] *= static_cast<value_type>(scalar);
			data_[data_index] *= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator*=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator*=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] *= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] *= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator/=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] /= static_cast<value_type>(scalar);
			data_[data_index] /= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator/=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator/=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] /= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] /= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator%=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] %= static_cast<value_type>(scalar);
			data_[data_index] %= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator%=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator%=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] %= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] %= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator&=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] &= static_cast<value_type>(scalar);
			data_[data_index] &= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator&=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator&=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] &= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] &= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator|=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] |= static_cast<value_type>(scalar);
			data_[data_index] |= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator|=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator|=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] |= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] |= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator^=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] ^= static_cast<value_type>(scalar);
			data_[data_index] ^= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator^=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator^=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] ^= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] ^= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator<<=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] <<= static_cast<value_type>(scalar);
			data_[data_index] <<= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator<<=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator<<=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] <<= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] <<= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
			constexpr self_reference operator>>=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			data_[vector1_type::data_index] >>= static_cast<value_type>(scalar);
			data_[data_index] >>= static_cast<value_type>(scalar);
			return *this;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			constexpr self_reference operator>>=(const acceptable_type<Size, U>& other)
			noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
			{
				this->operator>>=(other[vector1_type::data_index]);
			}
			else
			{
				data_[vector1_type::data_index] >>= static_cast<value_type>(other[vector1_type::data_index]);
				data_[data_index] >>= static_cast<value_type>(other[data_index]);
			}
			return *this;
		}

		constexpr self_reference operator--() noexcept
		{
			--data_[vector1_type::data_index];
			--data_[data_index];
			return *this;
		}

		constexpr self_type operator--(int) noexcept
		{
			auto copy{ *this };
			this->operator--();
			return copy;
		}

		constexpr self_reference operator++() noexcept
		{
			++data_[vector1_type::data_index];
			++data_[data_index];
			return *this;
		}

		constexpr self_type operator++(int) noexcept
		{
			auto copy{ *this };
			this->operator++();
			return copy;
		}

		constexpr self_type operator+() const noexcept
		{
			return self_type{ data_[vector1_type::data_index], data_[data_index] };
		}

		constexpr self_type operator-() const noexcept
		{
			return self_type{ -data_[vector1_type::data_index], -data_[data_index] };
		}

		constexpr self_type operator~() const noexcept
		{
			return self_type{ ~data_[vector1_type::data_index], ~data_[data_index] };
		}

		template<typename U>
		constexpr self_type operator+(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			auto copy{ *this };
			copy += scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator+(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator+=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy += self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator+(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator+=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy += self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator-(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<U>())))
		{
			auto copy{ *this };
			copy -= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator-(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator-=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy -= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator-(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator-=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy -= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator*(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<U>())))
		{
			auto copy{ *this };
			copy *= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator*(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator*=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy *= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator*(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator*=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy *= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator/(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<U>())))
		{
			auto copy{ *this };
			copy /= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator/(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator/=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy /= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator/(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator/=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy /= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator%(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<U>())))
		{
			auto copy{ *this };
			copy %= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator%(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator%=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy %= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator%(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator%=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy %= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator&(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<U>())))
		{
			auto copy{ *this };
			copy &= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator&(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator&=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy &= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator&(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator&=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy &= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator|(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<U>())))
		{
			auto copy{ *this };
			copy |= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator|(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator|=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy |= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator|(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator|=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy |= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator^(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<U>())))
		{
			auto copy{ *this };
			copy ^= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator^(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator^=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy ^= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator^(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator^=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy ^= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator<<(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<U>())))
		{
			auto copy{ *this };
			copy <<= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator<<(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator<<=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy <<= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator<<(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator<<=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy <<= self;
			return copy;
		}

		template<typename U>
		constexpr self_type operator>>(U scalar) const
			noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<U>())))
		{
			auto copy{ *this };
			copy >>= scalar;
			return copy;
		}

		template<arithmetic U>
		friend constexpr acceptable_type<data_size, U> operator>>(U scalar, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator>>=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ scalar };
			copy >>= self;
			return copy;
		}

		template<vector_size_type Size, typename U>
		requires (Size >= vector1_type::data_size)
			friend constexpr acceptable_type<data_size, U> operator>>(const acceptable_type<Size, U>& vec, const_self_reference self)
			noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator>>=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{ vec };
			copy >>= self;
			return copy;
		}

		template <typename U>
		requires std::is_convertible_v<U, value_type>
		constexpr bool operator==(const acceptable_type<data_size, U>& other) const noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return static_cast<value_type>(other[vector1_type::data_index]) == data_[vector1_type::data_index] && static_cast<value_type>(other[data_index]) == data_[data_index];
		}

		template <typename U>
		constexpr bool operator!=(const acceptable_type<data_size, U>& other) const noexcept(noexcept(std::declval<self_type>().operator==(std::declval<const acceptable_type<data_size, U>&>())))
		{
			return !this->operator==(other);
		}

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr explicit(!std::is_same_v<value_type, bool>) operator bool() const noexcept
		{
			return data_[vector1_type::data_index] && data_[data_index];
		}

	private:
		container_type data_;
	};
}
