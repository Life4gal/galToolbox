#pragma once

#include "rb.hpp"
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

namespace gal::test
{
	template <typename T, std::size_t N, typename Alloc = std::allocator<T>>
	class fifo
	{
		template <typename A, std::size_t B, typename C>
		using fifo_reference = fifo<A, B, C>bitand;
		template <typename A, std::size_t B, typename C>
		using const_fifo_reference = const fifo<A, B, C>bitand;
		template <typename A, std::size_t B, typename C>
		using fifo_rreference = fifo<A, B, C>and;
	public:
		using internal_type = ring_buffer<T, N, Alloc>;

		using allocator_type = typename internal_type::allocator_type;
		using allocator_trait_type = typename internal_type::allocator_trait_type;

		using value_tye = typename internal_type::value_type;
		using size_type = typename internal_type::size_type;
		using iterator = typename internal_type::iterator;

		constexpr static size_type max_size = internal_type::max_size;

		using time_type = size_type;

		using reference = typename internal_type::reference;
		using const_reference = typename internal_type::const_reference;
		using pointer = typename internal_type::pointer;
		using const_pointer = typename internal_type::const_pointer;

		constexpr explicit fifo() noexcept(noexcept(std::is_nothrow_default_constructible_v<internal_type>)) = default;

		/**
		 * @brief push a new data into ring buffer
		 * it will not overwrite exist data, return false if push fail
		 * @param data new data
		 * @return push result
		*/
		constexpr bool push(const_reference data)
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<bool, decltype(fifo<value_tye, max_size, allocator_type>::full)>) and
			noexcept(std::is_nothrow_constructible_v<std::scoped_lock, std::mutex>) and
			noexcept(std::is_nothrow_invocable_v<decltype(internal_type::set_or_overwrite), iterator, const_reference>)
			and
			noexcept(std::is_nothrow_invocable_v<decltype(std::condition_variable::notify_one)>)
		)
		{
			if (full())
			{
				return false;
			}

			{
				std::scoped_lock lock(write_mutex_);
				if (full())
				{
					return false;
				}

				buffer_.set_or_overwrite(producer_, data);
				++producer_;
				++count_;
			}
			read_cond_.notify_one();
			return true;
		}

		/**
		 * @brief push a new data into ring buffer
		 * it will overwrite exist data
		 * @param data new data
		*/
		constexpr void push_force(const_reference data)
		noexcept(
			noexcept(std::is_nothrow_constructible_v<std::scoped_lock, std::mutex>) and
			noexcept(std::is_nothrow_invocable_v<decltype(internal_type::set_or_overwrite), iterator, const_reference>)
			and
			noexcept(std::is_nothrow_invocable_v<decltype(std::condition_variable::notify_one)>)
		)
		{
			{
				std::scoped_lock lock(write_mutex_);

				buffer_.set_or_overwrite(producer_, data);
				++producer_;
				++count_;
			}
			read_cond_.notify_one();
		}

		/**
		 * @brief pop a data from ring buffer
		 * @param data a reference to receive data
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * time == -1 means you are not willing to wait, if nothing to pop, return false
		 * time == 0  means you have enough patience to wait for a necessary data
		 * otherwise, wait for the given time, if there is still no data, return false
		 * @return pop success or not
		*/
		constexpr bool pop(reference data, time_type wait_milliseconds_time = 0)
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<bool, decltype(fifo<value_tye, max_size, allocator_type>::empty)>)
			and
			noexcept(std::is_nothrow_constructible_v<std::unique_lock, std::mutex>) and
			noexcept(std::is_nothrow_invocable_v<decltype(internal_type::set_or_overwrite), iterator>) and
			noexcept(std::is_nothrow_invocable_v<decltype(std::condition_variable::wait), std::unique_lock<std::mutex>>)
			and
			noexcept(std::is_nothrow_constructible_v<std::chrono::milliseconds, time_type>) and
			noexcept(std::is_nothrow_invocable_v<
				decltype(std::condition_variable::wait_for), std::mutex, std::chrono::milliseconds>)
		)
		{
			std::unique_lock lock(read_mutex_);
			if (empty())
			{
				if (wait_milliseconds_time == static_cast<time_type>(-1))
				{
					return false;
				}
				if (wait_milliseconds_time == 0)
				{
					read_cond_.wait(lock);
				}
				else
				{
					read_cond_.wait_for(lock, std::chrono::milliseconds(wait_milliseconds_time));
				}

				if (empty())
				{
					return false;
				}
			}

			data = buffer_.get(consumer_);
			++consumer_;
			--count_;
			return true;
		}

		/**
		 * @brief pop a data from ring buffer but ignore what is popped
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * for more detail, see `pop` with two arguments
		 * @return pop success or not
		*/
		constexpr bool pop(time_type wait_milliseconds_time = 0)
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<
				bool, decltype(fifo<value_tye, max_size, allocator_type>::pop), reference, time_type>)
		)
		{
			value_tye dummy;
			return pop(dummy, wait_milliseconds_time);
		}

		/**
		 * @brief get the size of the currently existing data
		 * @return size
		*/
		constexpr size_type size() const
		noexcept(
			noexcept(std::is_nothrow_invocable_r_v<size_type, decltype(internal_type::distance), iterator, iterator>)
		)
		{
			return buffer_.distance(consumer_, producer_);
		}

		[[nodiscard]] constexpr bool full() const
		{
			return count_.load() == max_size;
		}

		[[nodiscard]] constexpr bool empty() const
		{
			return count_.load() == 0;
		}

	private:
		internal_type buffer_;

		iterator producer_;
		iterator consumer_;

		std::atomic<size_type> count_;

		std::mutex read_mutex_;
		std::mutex write_mutex_;

		std::condition_variable read_cond_;
	};
}
