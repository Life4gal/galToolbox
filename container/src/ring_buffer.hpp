#pragma once

#include <bitset>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>

namespace gal::toolbox::container
{
	/**
	 * @brief a common ring buffer with a fixed capacity
	 * @tparam T value type
	 * @tparam Alloc allocator type
	*/
	template<typename T, std::size_t Size, typename Alloc = std::allocator<T>>
	class ring_buffer
	{
	public:
		using allocator_type				= Alloc;
		using allocator_trait_type			= std::allocator_traits<allocator_type>;

		using value_type					= typename allocator_type::value_type;
		using size_type						= typename allocator_type::size_type;

		constexpr static size_type max_size = Size;
		constexpr static size_type mask		= max_size - 1;
		static_assert((max_size & mask) == 0, "capacity must be 2^n");

		using bit_checker_type = std::bitset<max_size>;

		using reference		   = value_type&;
		using const_reference  = const value_type&;
		using pointer		   = value_type*;
		using const_pointer	   = const value_type*;

		constexpr ring_buffer() noexcept(
				noexcept(allocator_trait_type::allocate(std::declval<allocator_type&>(), std::declval<size_type>())))
			: buffer_(allocator_trait_type::allocate(allocator_, max_size)) {}

		template<std::size_t... I, typename... Args>
		constexpr explicit ring_buffer(std::index_sequence<I...>, Args&&... args)
			: ring_buffer()
		{
			(set_or_overwrite(I, std::forward<Args>(args)), ...);
		}

		constexpr ~ring_buffer() noexcept
		{
			for (size_type i = 0; i < max_size; ++i)
			{
				if (bit_checker_[i])
				{
					// the elements of the current bit have been constructed
					allocator_trait_type::destroy(allocator_, std::next(buffer_, i));
				}
			}

			allocator_trait_type::deallocate(allocator_, buffer_, max_size);
			buffer_ = nullptr;
		}

		template<std::convertible_to<value_type> U, size_type N, typename Allocator>
		constexpr explicit ring_buffer(const ring_buffer<U, N, Allocator>& other) noexcept(noexcept(std::is_nothrow_convertible_v<U, value_type>))
			: buffer_(allocator_trait_type::allocate(allocator_, max_size))
		{
			paste(other);
		}

		template<std::convertible_to<value_type> U, size_type N, typename Allocator>
		constexpr ring_buffer& operator=(const ring_buffer<U, N, Allocator>& other) noexcept(noexcept(std::is_nothrow_convertible_v<U, value_type>))
		{
			if constexpr (std::is_same_v<U, value_type>)
			{
				if (std::addressof(other) == this)
				{
					return *this;
				}
			}

			paste(other);

			return *this;
		}

		constexpr ring_buffer(ring_buffer&& other) noexcept
			: buffer_(std::exchange(other.buffer_, nullptr)),
			  bit_checker_(std::exchange(other.bit_checker_, {})) {}

		constexpr ring_buffer& operator=(ring_buffer&& other) noexcept
		{
			buffer_		 = std::exchange(other.buffer_, nullptr);
			bit_checker_ = std::exchange(other.bit_checker_, {});
			return *this;
		}

		/**
		 * @brief get an element's reference
		 * @param index element's index
		 * @return reference
		 * @note if the element of the given index has not been constructed, the behavior is undefined
		*/
		constexpr reference operator[](size_type index) noexcept(noexcept(std::declval<pointer>()[0]))
		{
			return *std::next(buffer_, index bitand mask);
		}

		/**
		 * @brief get an element's reference
		 * @param index element's index
		 * @return const_reference
		 * @note if the element of the given index has not been constructed, the behavior is undefined
		*/
		constexpr const_reference operator[](size_type index) const
				noexcept(noexcept(std::declval<pointer>()[0]))
		{
			return *std::next(buffer_, index bitand mask);
		}

		/**
		 * @brief paste another ring_buffer's elements into this ring_buffer
		*/
		template<std::convertible_to<value_type> U, size_type N, typename Allocator>
		constexpr void paste(const ring_buffer<U, N, Allocator>& other) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			// we only copy parts of the same length, other parts do not provide any guarantee
			for (size_type i = 0; i < std::min(max_size, ring_buffer<U, N, Allocator>::max_size); ++i)
			{
				if (other.exist(i))
				{
					// the elements of the current bit have been constructed
					set_or_overwrite(i, other[i]);
				}
			}
		}

		/**
		 * @brief check the element of the give index has been constructed or not
		 * @param index element's index
		 * @return constructed or not
		*/
		constexpr bool exist(size_type index) const noexcept(noexcept(noexcept(std::declval<bit_checker_type>()[0])))
		{
			return bit_checker_[index];
		}

		/**
		 * @brief get an element's reference by given index, if the element of the given index has not been constructed, construct it by args
		 * @tparam Args args' type
		 * @param index element's index
		 * @param args args
		 * @return reference
		*/
		template<std::constructible_from<value_type>... Args>
		constexpr reference get(size_type index, Args&&... args) noexcept(
				noexcept(std::declval<bit_checker_type>()[0]) and noexcept(std::declval<bit_checker_type>().set(0)) and noexcept(std::is_nothrow_constructible_v<value_type, Args...>))
		{
			const auto real_index = index bitand mask;
			if (not bit_checker_[real_index])
			{
				// has not been constructed yet
				bit_checker_.set(real_index);
				allocator_trait_type::construct(allocator_,
												std::next(buffer_, real_index),
												std::forward<Args>(args)...);
			}

			return *std::next(buffer_, real_index);
		}

		/**
		 * @brief set or overwrite an element by given index, if given args is empty, just destroy the element, else if the element of the given index has not been constructed, construct it by args(destruct first)
		 * @tparam Args args' type
		 * @param index element's index
		 * @param args args
		 * @return nothing
		*/
		template<std::constructible_from<value_type>... Args>
		constexpr void set_or_overwrite(size_type index, Args&&... args) noexcept(
				noexcept(std::declval<bit_checker_type>()[0]) and noexcept(std::declval<bit_checker_type>().set(0)) and noexcept(std::is_nothrow_constructible_v<value_type, Args...>))
		{
			if (const auto real_index = index bitand mask; not bit_checker_[real_index])
			{
				// has not been constructed yet
				if constexpr (sizeof...(args) == 0)
				{
					// user just want to destroy it
					return;
				}
				else
				{
					allocator_trait_type::construct(allocator_,
													std::next(buffer_, real_index),
													std::forward<Args>(args)...);
					bit_checker_.set(real_index);
				}
			}
			else
			{
				allocator_trait_type::destroy(allocator_, std::next(buffer_, real_index));
				if constexpr (sizeof...(args) == 0)
				{
					// user just want to destroy it
					bit_checker_.set(real_index, false);
					return;
				}
				else
				{
					allocator_trait_type::construct(allocator_,
													std::next(buffer_, real_index),
													std::forward<Args>(args)...);
					bit_checker_.set(real_index);
					return;
				}
			}
		}

		[[nodiscard]] constexpr pointer begin() noexcept
		{
			return buffer_;
		}

		[[nodiscard]] constexpr const_pointer begin() const noexcept
		{
			return buffer_;
		}

		[[nodiscard]] constexpr const_pointer cbegin() const noexcept
		{
			return buffer_;
		}

		[[nodiscard]] constexpr pointer end() noexcept
		{
			return buffer_ + max_size;
		}

		[[nodiscard]] constexpr const_pointer end() const noexcept
		{
			return buffer_ + max_size;
		}

		[[nodiscard]] constexpr const_pointer cend() const noexcept
		{
			return buffer_ + max_size;
		}

		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return bit_checker_.count();
		}

		/**
		 * @brief get where the pos actually in
		 * @param pos given pos
		 * @return actually index
		*/
		[[nodiscard]] constexpr size_type index(size_type pos) const noexcept
		{
			return pos bitand mask;
		}

		/**
		 * @brief get the distance between begin and end in buffer,
		 * begin is greater than end or vice versa does not affect the result
		 * @param begin given begin
		 * @param end given end
		 * @return distance
		*/
		[[nodiscard]] constexpr size_type distance(size_type begin, size_type end) noexcept
		{
			begin and_eq mask;
			end and_eq mask;

			if (end >= begin)
			{
				return end - begin;
			}
			return max_size - (begin - end);
		}

	private:
		pointer								 buffer_;
		bit_checker_type					 bit_checker_;

		[[no_unique_address]] allocator_type allocator_;
	};

	template<typename U1, std::size_t S, typename Allocator1, std::convertible_to<U1> U2, typename Allocator2>
	constexpr bool operator==(const ring_buffer<U1, S, Allocator1>& lhs, const ring_buffer<U2, S, Allocator2>& rhs)
			noexcept(noexcept(std::declval<typename ring_buffer<U1, S, Allocator1>::const_reference>() == std::declval<typename ring_buffer<U2, S, Allocator2>::const_reference>()))
	{
		for (std::size_t i = 0; i < S; ++i)
		{
			if (lhs.exist(i))
			{
				if (not rhs.exist(i))
				{
					return false;
				}

				if (not(lhs[i] == rhs[i]))
				{
					return false;
				}
			}
			else if (rhs.exist(i))
			{
				return false;
			}
		}

		return true;
	}

	template<typename U1, std::size_t S, typename Allocator1, std::convertible_to<U1> U2, typename Allocator2>
	constexpr bool operator!=(const ring_buffer<U1, S, Allocator1>& lhs, const ring_buffer<U2, S, Allocator2>& rhs)
			noexcept(noexcept(std::declval<const ring_buffer<U1, S, Allocator1>&>() == std::declval<const ring_buffer<U2, S, Allocator2>&>()))
	{
		return not(lhs == rhs);
	}
}// namespace gal::toolbox::utils
