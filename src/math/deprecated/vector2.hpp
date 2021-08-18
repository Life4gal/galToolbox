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
		constexpr static size_type data_size  = 2;

		constexpr vector() noexcept = default;

		template <arithmetic U>
		constexpr explicit vector(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
			: data_({static_cast<value_type>(scalar), static_cast<value_type>(scalar)}) {}

		template <typename U>
		constexpr explicit vector(const acceptable_type<vector1_type::data_size, U>& other)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
			: data_({
						static_cast<value_type>(other[vector1_type::data_index]),
						static_cast<value_type>(other[vector1_type::data_index])
					}) {}

		template <arithmetic U1, arithmetic U2>
		constexpr vector(U1 x, U2 y)
		noexcept(std::is_nothrow_convertible_v<U1, value_type> && std::is_nothrow_convertible_v<U2, value_type>)
			: data_({static_cast<value_type>(x), static_cast<value_type>(y)}) {}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr explicit vector(const acceptable_type<Size, U>& other)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
			: data_({
						static_cast<value_type>(other[vector1_type::data_index]),
						static_cast<value_type>(other[data_index])
					}) {}

		template <vector_size_type Size1, typename U1, vector_size_type Size2, typename U2>
			requires (Size1 >= vector1_type::data_size && Size2 >= vector1_type::data_size)
		constexpr vector(
			const acceptable_type<Size1, U1>& v1,
			const acceptable_type<Size2, U2>& v2)
		noexcept(std::is_nothrow_convertible_v<U1, value_type> && std::is_nothrow_convertible_v<U2, value_type>)
			: data_({
						static_cast<value_type>(v1[vector1_type::data_index]),
						static_cast<value_type>(v2[vector1_type::data_index])
					}) {}

		template <vector_size_type Size, typename U, typename A>
			requires (Size >= vector1_type::data_size) && std::is_convertible_v<U, value_type> && std::is_convertible_v<
				A, value_type>
		constexpr vector(const acceptable_type<Size, U>& other, A scalar)
		noexcept(std::is_nothrow_convertible_v<U, value_type> && std::is_nothrow_convertible_v<A, value_type>)
			: data_({static_cast<value_type>(other[vector1_type::data_index]), static_cast<value_type>(scalar)}) {}

		template <vector_size_type Size, typename U, typename A>
			requires (Size >= vector1_type::data_size) && std::is_convertible_v<U, value_type> && std::is_convertible_v<
				A, value_type>
		constexpr vector(A scalar, const acceptable_type<Size, U>& other)
		noexcept(std::is_nothrow_convertible_v<U, value_type> && std::is_nothrow_convertible_v<A, value_type>)
			: data_({static_cast<value_type>(scalar), static_cast<value_type>(other[vector1_type::data_index])}) {}

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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		constexpr self_reference operator+=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			if constexpr (Size == vector1_type::data_size)
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

		template <vector_size_type Size, typename U>
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

		template <vector_size_type Size, typename U>
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

		template <vector_size_type Size, typename U>
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
			this->percent_support(scalar, scalar);
			return *this;
		}

		template <vector_size_type Size, typename U>
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
				this->percent_support(other[vector1_type::data_index], other[data_index]);
			}
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator&=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				data_[vector1_type::data_index] &= static_cast<value_type>(scalar);
				data_[data_index] &= static_cast<value_type>(scalar);
			}
			else
			{
				// do nothing here
			}

			return *this;
		}

		template <vector_size_type Size, typename U>
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
				if constexpr (std::is_integral_v<value_type>)
				{
					data_[vector1_type::data_index] &= static_cast<value_type>(other[vector1_type::data_index]);
					data_[data_index] &= static_cast<value_type>(other[data_index]);
				}
				else
				{
					// do nothing here
				}
			}

			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator|=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				data_[vector1_type::data_index] |= static_cast<value_type>(scalar);
				data_[data_index] |= static_cast<value_type>(scalar);
			}
			else
			{
				// do nothing here
			}

			return *this;
		}

		template <vector_size_type Size, typename U>
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
				if constexpr (std::is_integral_v<value_type>)
				{
					data_[vector1_type::data_index] |= static_cast<value_type>(other[vector1_type::data_index]);
					data_[data_index] |= static_cast<value_type>(other[data_index]);
				}
				else
				{
					// do nothing here
				}
			}

			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator^=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				data_[vector1_type::data_index] ^= static_cast<value_type>(scalar);
				data_[data_index] ^= static_cast<value_type>(scalar);
			}
			else
			{
				// do nothing here
			}

			return *this;
		}

		template <vector_size_type Size, typename U>
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
				if constexpr (std::is_integral_v<value_type>)
				{
					data_[vector1_type::data_index] ^= static_cast<value_type>(other[vector1_type::data_index]);
					data_[data_index] ^= static_cast<value_type>(other[data_index]);
				}
				else
				{
					// do nothing here
				}
			}

			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator<<=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			this->template shift_support<true>(scalar, scalar);
			return *this;
		}

		template <vector_size_type Size, typename U>
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
				this->template shift_support<true>(other[vector1_type::data_index], other[data_index]);
			}
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator>>=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			this->template shift_support<false>(scalar, scalar);
			return *this;
		}

		template <vector_size_type Size, typename U>
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
				this->template shift_support<true>(other[vector1_type::data_index], other[data_index]);
			}
			return *this;
		}

		constexpr self_reference operator--() noexcept
		{
			data_[vector1_type::data_index] -= 1;
			data_[data_index] -= 1;

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
			data_[vector1_type::data_index] += 1;
			data_[data_index] += 1;
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
			return self_type{data_[vector1_type::data_index], data_[data_index]};
		}

		constexpr auto operator-() const noexcept
		{
			if constexpr (std::is_unsigned_v<value_type>)
			{
				using signed_type = std::make_signed_t<value_type>;
				return acceptable_type<data_size, signed_type>{
					-static_cast<signed_type>(data_[vector1_type::data_index]),
					-static_cast<signed_type>(data_[data_index])
				};
			}
			else
			{
				return self_type{-data_[vector1_type::data_index], -data_[data_index]};
			}
		}

		constexpr self_type operator~() const noexcept
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				return self_type{~data_[vector1_type::data_index], ~data_[data_index]};
			}
			else
			{
				// do nothing here
				return *this;
			}
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator+(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator+=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator-(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator-=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator*(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator*=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator/(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator/=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator%(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator%=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator&(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator&=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator|(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator|=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator^(const acceptable_type<Size, U>& vec,
																const_self_reference             self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator^=(std::declval<const_self_reference>())
		))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator<<(const acceptable_type<Size, U>& vec,
																const_self_reference              self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator
			<<=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{vec};
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

		template <vector_size_type Size, typename U>
			requires (Size >= vector1_type::data_size)
		friend constexpr acceptable_type<data_size, U> operator>>(const acceptable_type<Size, U>& vec,
																const_self_reference              self)
		noexcept(noexcept(std::declval<acceptable_type<data_size, U>>().operator
			>>=(std::declval<const_self_reference>())))
		{
			acceptable_type<data_size, U> copy{vec};
			copy >>= self;
			return copy;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr bool operator==(
			const acceptable_type<data_size, U>& other) const noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return static_cast<value_type>(other[vector1_type::data_index]) == data_[vector1_type::data_index] &&
				static_cast<value_type>(other[data_index]) == data_[data_index];
		}

		template <typename U>
		constexpr bool operator!=(
			const acceptable_type<data_size, U>& other) const noexcept(noexcept(std::declval<self_type>().operator
			==(std::declval<const acceptable_type<data_size, U>&>())))
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

		template <typename U>
		constexpr void percent_support(U s1, U s2) noexcept
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				data_[vector1_type::data_index] %= static_cast<value_type>(s1);
				data_[data_index] %= static_cast<value_type>(s2);
			}
			else
			{
				if constexpr (std::is_signed_v<U>)
				{
					if (s1 < 0)
					{
						s1 = -s1;
					}
					if (s2 < 0)
					{
						s2 = -s2;
					}
				}

				constexpr auto f = [](value_type& value, U s) constexpr -> void
				{
					if (value < 0)
					{
						auto n = static_cast<std::conditional_t<
							(sizeof(value_type) > sizeof(std::int32_t)), std::int64_t, std::int32_t>>(-value / static_cast<value_type>(s));
						value += static_cast<value_type>(n) * static_cast<decltype(n)>(s);
					}
					else
					{
						auto n = static_cast<std::conditional_t<
							(sizeof(value_type) > sizeof(std::uint32_t)), std::uint64_t, std::uint32_t>>(value / static_cast<value_type>(s));
						value -= static_cast<value_type>(n) * static_cast<decltype(n)>(s);
					}
				};
				f(data_[vector1_type::data_index], s1);
				f(data_[data_index], s2);
			}
		}

		template <bool Left, typename U>
		constexpr void shift_support(U s1, U s2) noexcept
		{
			if constexpr (std::is_integral_v<value_type>)
			{
				if constexpr (Left)
				{
					data_[vector1_type::data_index] <<= static_cast<value_type>(s1);
					data_[data_index] <<= static_cast<value_type>(s2);
				}
				else
				{
					data_[vector1_type::data_index] >>= static_cast<value_type>(s1);
					data_[data_index] >>= static_cast<value_type>(s2);
				}
			}
			else
			{
				bool s1_lt_0 = false;
				bool s2_lt_0 = false;
				if constexpr (std::is_signed_v<U>)
				{
					if (s1 < 0)
					{
						s1_lt_0 = true;
						s1      = -s1;
					}
					if (s2 < 0)
					{
						s2_lt_0 = true;
						s2      = -s2;
					}
				}

				if constexpr (
					constexpr auto shift = [](value_type& value, const bool left, auto scalar) constexpr -> void
					{
						if (
							constexpr auto pow = [](auto v, auto p) constexpr -> auto
							{
								auto ret = v;
								while (--p)
								{
									ret *= v;
								}
								return ret;
							}; left)
						{
							value *= pow(2,
										static_cast<std::conditional_t<
											(sizeof(U) > sizeof(std::conditional_t<
												std::is_signed_v<U>, std::int32_t, std::uint32_t>)),
											std::conditional_t<std::is_signed_v<U>, std::int64_t, std::uint64_t>,
											std::conditional_t<std::is_signed_v<U>, std::int32_t, std::uint32_t>>>(
											scalar));
						}
						else
						{
							value /= pow(2,
										static_cast<std::conditional_t<
											(sizeof(U) > sizeof(std::conditional_t<
												std::is_signed_v<U>, std::int32_t, std::uint32_t>)),
											std::conditional_t<std::is_signed_v<U>, std::int64_t, std::uint64_t>,
											std::conditional_t<std::is_signed_v<U>, std::int32_t, std::uint32_t>>>(
											scalar));
						}
					}; Left)
				{
					shift(data_[vector1_type::data_index], !s1_lt_0, s1);
					shift(data_[data_index], !s2_lt_0, s2);
				}
				else
				{
					shift(data_[vector1_type::data_index], s1_lt_0, s1);
					shift(data_[data_index], s2_lt_0, s2);
				}
			}
		}
	};
}
