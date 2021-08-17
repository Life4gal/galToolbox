#pragma once

#include "precision.hpp"
#include "vector_trait.hpp"
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

		using container_type = std::array<T, 1>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;

		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;

		constexpr static size_type data_index = 0;
		constexpr static size_type data_size  = 1;

		constexpr vector() noexcept = default;

		template <arithmetic U>
		constexpr explicit vector(U scalar)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
			: data_({static_cast<value_type>(scalar)}) {}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr explicit vector(const acceptable_type<Size, U>& other)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
			: data_({static_cast<value_type>(other[data_index])}) {}

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
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_add<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator-=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_minus<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator*=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_multi<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator/=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_div<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator%=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_percent<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator&=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_and<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator|=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_or<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator^=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_xor<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator<<=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_left_shift<value_type, U>,
											scalar);
			return *this;
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr self_reference operator>>=(U scalar) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_right_shift<value_type, U>,
											scalar);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator+=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator+=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_add<value_type, U>);
			this->operator+=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator-=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator-=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_minus<value_type, U>);
			this->operator-=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator*=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator*=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_multi<value_type, U>);
			this->operator*=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator/=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator/=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_div<value_type, U>);
			this->operator/=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator%=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator%=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_percent<value_type, U>);
			this->operator%=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator&=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator&=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_and<value_type, U>);
			this->operator&=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator|=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator|=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_or<value_type, U>);
			this->operator|=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator^=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator^=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_xor<value_type, U>);
			this->operator^=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator<<=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator<<=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_left_shift<value_type, U>);
			this->operator<<=(other[data_index]);
			return *this;
		}

		template <vector_size_type Size, typename U>
			requires (Size >= data_size)
		constexpr self_reference operator>>=(const acceptable_type<Size, U>& other)
		noexcept(noexcept(std::declval<self_type>().operator>>=(std::declval<U>())))
		{
			// vector_trait::invoke_vec<data_size>(data_,
			// 	other,
			// 	vector_trait::operator_right_shift<value_type, U>);
			this->operator>>=(other[data_index]);
			return *this;
		}

		constexpr self_reference operator--() noexcept
		{
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_minus<value_type, value_type>,
											1);
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
			vector_trait::invoke<data_size>(data_,
											vector_trait::operator_add<value_type, value_type>,
											1);
			return *this;
		}

		constexpr self_type operator++(int) noexcept
		{
			auto copy{*this};
			this->operator++();
			return copy;
		}

		constexpr auto operator-() const noexcept
		{
			if constexpr (std::is_unsigned_v<value_type>)
			{
				using signed_type = std::make_signed_t<value_type>;
				acceptable_type<data_size, signed_type> copy{*this};
				vector_trait::invoke<data_size>(copy,
												vector_trait::operator_unary_minus<value_type>
												);
				return copy;
			}
			else
			{
				self_type copy{*this};
				vector_trait::invoke<data_size>(copy,
												vector_trait::operator_unary_minus<value_type>
												);
				return copy;
			}
		}

		constexpr self_type operator~() const noexcept
		{
			self_type copy{*this};
			vector_trait::invoke<data_size>(copy,
											vector_trait::operator_unary_tilde<value_type>
											);
			return copy;
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
			requires std::is_convertible_v<U, value_type>
		constexpr bool operator==(
			const acceptable_type<data_size, U>& other) const noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return vector_trait::invoke_vec_r<data_size>(data_,
														other,
														vector_trait::operator_equal_to<value_type, U>);
		}

		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr bool operator==(U u) const noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return vector_trait::invoke_r<data_size>(data_,
													vector_trait::operator_equal_to<value_type, U>,
													u);
		}

		template <typename U>
		constexpr bool operator!=(
			const acceptable_type<data_size, U>& other) const noexcept(noexcept(std::declval<self_type>().operator
			==(std::declval<const acceptable_type<data_size, U>&>())))
		{
			return !this->operator==(other);
		}

		template <typename U>
		constexpr bool operator!=(U u) const noexcept(noexcept(std::declval<self_type>().operator==(std::declval<U>())))
		{
			return !this->operator==(u);
		}

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr explicit(!std::is_same_v<value_type, bool>) operator bool() const noexcept
		{
			return vector_trait::invoke_r<data_size>(data_,
													vector_trait::operator_to_bool<value_type>
													);
		}

	private:
		container_type data_;
	};
}
