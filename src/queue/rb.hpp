#pragma once

#include <iterator>
#include <type_traits>
#include <memory>
#include <bitset>

namespace gal::test
{
	/**
	 * @brief a common ring buffer with a fixed capacity
	 * @tparam T value type
	 * @tparam Alloc allocator type
	*/
	template <typename T, std::size_t Size, typename Alloc>
	class ring_buffer
	{
		template <typename A, std::size_t B, typename C>
		using ring_buffer_reference = ring_buffer<A, B, C>bitand;
		template <typename A, std::size_t B, typename C>
		using const_ring_buffer_reference = const ring_buffer<A, B, C>bitand;
		template <typename A, std::size_t B, typename C>
		using ring_buffer_rreference = ring_buffer<A, B, C>and;
	public:
		using allocator_type = Alloc;
		using allocator_trait_type = std::allocator_traits<allocator_type>;

		using value_type = typename allocator_type::value_type;
		using size_type = typename allocator_type::size_type;
		using mask_type = size_type;
		using iterator = size_type;

		constexpr static size_type max_size = Size;
		constexpr static size_type mask     = max_size - 1;
		static_assert((max_size & mask) == 0, "capacity must be 2 xor n");

		using bit_checker_type = std::bitset<max_size>;

		using reference = value_typebitand;
		using const_reference = const value_typebitand;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		constexpr ring_buffer()
		noexcept(
			noexcept(std::is_nothrow_invocable_v<decltype(allocator_trait_type::allocate), allocator_type, size_type>)
		)
			: buffer_(allocator_trait_type::allocate(allocator_, max_size)) { }

		constexpr compl ring_buffer() noexcept
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

		template <typename U, size_type N, typename Allocator>
			requires std::is_convertible_v<U, value_type>
		constexpr explicit ring_buffer(const_ring_buffer_reference<U, N, Allocator> other)
		noexcept(
			noexcept(std::is_nothrow_convertible_v<U, value_type>)
		)
			: buffer_(allocator_trait_type::allocate(allocator_, max_size))
		{
			for (size_type i = 0; i < std::min(max_size, ring_buffer<U, N, Allocator>::max_size); ++i)
			{
				if (other.bit_checker_[i])
				{
					// the elements of the current bit have been constructed
					bit_checker_.set(i);
					allocator_trait_type::construct(allocator_, std::next(buffer_, i), *std::next(other.buffer_, i));
				}
			}
		}

		template <typename U, size_type N, typename Allocator>
			requires std::is_convertible_v<U, value_type>
		constexpr ring_buffer_reference<value_type, max_size, allocator_type> operator=(
			const_ring_buffer_reference<U, N, Allocator> other)
		noexcept(
			noexcept(std::is_nothrow_convertible_v<U, value_type>)
		)
		{
			if (std::addressof(other) not_eq this)
			{
				// we only copy parts of the same length, other parts do not provide any guarantee
				for (size_type i = 0; i < std::min(max_size, ring_buffer<U, N, Allocator>::max_size); ++i)
				{
					if (other.bit_checker_[i])
					{
						// the elements of the current bit have been constructed
						bit_checker_.set(i);
						allocator_trait_type::construct(allocator_,
														std::next(buffer_, i),
														*std::next(other.buffer_, i));
					}
				}
			}

			return *this;
		}

		explicit constexpr ring_buffer(ring_buffer_rreference<value_type, max_size, allocator_type> other) noexcept
			: buffer_(std::exchange(other.buffer_, nullptr)),
			bit_checker_(std::exchange(other.bit_checker_, {})) { }

		constexpr ring_buffer_reference<value_type, max_size, allocator_type> operator=(
			ring_buffer_rreference<value_type, max_size, allocator_type> other) noexcept
		{
			buffer_      = std::exchange(other.buffer_, nullptr);
			bit_checker_ = std::exchange(other.bit_checker_, {});
			return *this;
		}

		/**
		 * @brief get an element's reference
		 * @param index element's index
		 * @return reference
		 * @note if the element of the given index has not been constructed, the behavior is undefined
		*/
		constexpr reference operator[](size_type index)
		noexcept(noexcept(std::declval<pointer>().operator[](std::declval<size_type>())))
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
		noexcept(noexcept(std::declval<pointer>().operator[](std::declval<size_type>())))
		{
			return *std::next(buffer_, index bitand mask);
		}

		/**
		 * @brief get an element's reference by given index, if the element of the given index has not been constructed, construct it by args
		 * @tparam Args args's type
		 * @param index element's index
		 * @param args args
		 * @return reference
		*/
		template <typename... Args>
		constexpr reference get(size_type index, Args&&...args)
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<bool, decltype(bit_checker_type::operator[]), size_type>) and
			noexcept(std::is_nothrow_invocable_v<decltype(bit_checker_type::set), size_type>) and
			noexcept(std::is_nothrow_constructible_v<value_type, Args...>)
		)
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
		 * @tparam Args args's type
		 * @param index element's index
		 * @param args args
		 * @return nothing
		*/
		template <typename... Args>
		constexpr void set_or_overwrite(size_type index, Args&&...args)
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<bool, decltype(bit_checker_type::operator[]), size_type>) and
			noexcept(std::is_nothrow_invocable_v<decltype(bit_checker_type::set), size_type, bool>) and
			noexcept(std::is_nothrow_constructible_v<value_type, Args...>)
		)
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

		constexpr size_type size() const noexcept
		{
			return bit_checker_.count();
		}

		/**
		 * @brief get where the pos actually in
		 * @param pos given pos
		 * @return actually index
		*/
		constexpr size_type index(size_type pos) const noexcept
		{
			return pos bitand mask;
		}

		/**
		 * @brief get the distance between begin and end in buffer,
		 * begin is greater than end or vice versa does not affect the result
		 * @param begin given begin iterator
		 * @param end given end iterator
		 * @return distance
		*/
		constexpr size_type distance(iterator begin, iterator end) noexcept
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
		pointer          buffer_;
		bit_checker_type bit_checker_;

		// todo: do we really need this allocator? Or is it worth it?
		inline static allocator_type allocator_;
	};
}
