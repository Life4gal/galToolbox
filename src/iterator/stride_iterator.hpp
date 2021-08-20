#pragma once

#include <iterator>

namespace gal::test::iterator
{
	template <std::size_t Stride, std::random_access_iterator Iterator>
	struct stride_iterator
	{
		constexpr static auto stride = Stride;
		using iterator_type = Iterator;
		using value_type = std::iter_value_t<iterator_type>;
		using difference_type = std::iter_difference_t<iterator_type>;
		using reference = std::iter_reference_t<iterator_type>;

		using iterator_concept = std::random_access_iterator_tag;
		using iterator_category = std::random_access_iterator_tag;

		constexpr reference operator*() const noexcept(noexcept(*std::declval<iterator_type>()))
		{
			return *base_;
		}

		constexpr reference operator[](difference_type n) const noexcept(noexcept(std::declval<iterator_type>()[0]))
		{
			return base_[n * stride];
		}

		constexpr stride_iterator& operator+=(difference_type n) noexcept(noexcept(std::declval<iterator_type>() += 1))
		{
			base_ += n * stride;
			return *this;
		}

		constexpr stride_iterator& operator-=(difference_type n) noexcept(noexcept(std::declval<iterator_type>() -= 1))
		{
			base_ -= n * stride;
			return *this;
		}

		constexpr stride_iterator& operator++() noexcept(noexcept(std::declval<iterator_type>() += 1))
		{
			this->operator+=(1);
			return *this;
		}

		constexpr stride_iterator operator++(int) noexcept(noexcept(++std::declval<stride_iterator>()))
		{
			auto copy{*this};
			++*this;
			return copy;
		}

		constexpr stride_iterator& operator--() noexcept(noexcept(std::declval<iterator_type>() += 1))
		{
			this->operator-=(1);
			return *this;
		}

		constexpr stride_iterator operator--(int) noexcept(noexcept(--std::declval<stride_iterator>()))
		{
			auto copy{*this};
			--*this;
			return copy;
		}

		constexpr stride_iterator operator+(
			difference_type n) const noexcept(noexcept(std::declval<iterator_type>() += 1))
		{
			auto copy{*this};
			copy += n;
			return copy;
		}

		constexpr stride_iterator operator-(
			difference_type n) const noexcept(noexcept(std::declval<iterator_type>() -= 1))
		{
			auto copy{*this};
			copy -= n;
			return copy;
		}

		constexpr difference_type operator-(
			const stride_iterator& other) const noexcept(noexcept(std::declval<iterator_type>() - std::declval<
			iterator_type>()))
		{
			return base_ - other.base_;
		}

		iterator_type base_;

		constexpr friend stride_iterator operator+(difference_type         n,
													const stride_iterator& it) noexcept(noexcept(std::declval<
			iterator_type>() + 1))
		{
			return {it + n};
		}

		constexpr friend bool operator==(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs.base_ == rhs.base_;
		}

		constexpr friend bool operator!=(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		constexpr friend bool operator>(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs.base_ > rhs.base_;
		}

		constexpr friend bool operator<(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs.base_ < rhs.base_;
		}

		constexpr friend bool operator>=(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs > rhs || lhs == rhs;
		}

		constexpr friend bool operator<=(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs < rhs || lhs == rhs;
		}

		constexpr friend auto operator<=>(const stride_iterator& lhs, const stride_iterator& rhs) noexcept
		{
			return lhs.base_ <=> rhs.base_;
		}
	};

	template <std::size_t Stride, std::random_access_iterator Iterator>
	constexpr stride_iterator<Stride, Iterator> make_stride_iterator(Iterator iterator) noexcept
	{
		return {iterator};
	}
}
