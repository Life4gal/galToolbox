#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include <container/ring_buffer.hpp>

namespace gal::toolbox::container
{
	template<typename T, std::size_t N, typename Alloc = std::allocator<T>>
	class fifo
	{
	public:
		using internal_type					= ring_buffer<T, N, Alloc>;

		using value_tye						= typename internal_type::value_type;
		using size_type						= typename internal_type::size_type;

		constexpr static size_type max_size = internal_type::max_size;

		using time_type						= size_type;

		using reference						= typename internal_type::reference;
		using const_reference				= typename internal_type::const_reference;
		using pointer						= typename internal_type::pointer;
		using const_pointer					= typename internal_type::const_pointer;

		constexpr explicit fifo() noexcept	= default;

		/**
		 * @brief get the size of the currently existing data
		 * @return size
		*/
		constexpr size_type size() const noexcept
		{
			return buffer_.distance(consumer_, producer_);
		}

		[[nodiscard]] constexpr bool full() const noexcept
		{
			return count_.load() == max_size;
		}

		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return count_.load() == 0;
		}

		/**
		 * @brief push a new data into ring buffer
		 * @tparam Args args' type
		 * @param args the parameters must be constructable into the value_type
		 * @return push result
		*/
		template<typename... Args>
		bool push(Args&&... args) noexcept(
				noexcept(std::declval<internal_type>().template set_or_overwrite(std::declval<size_type>(), std::declval<decltype(std::forward<Args>(args))>()...)))
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

				buffer_.set_or_overwrite(producer_, std::forward<Args>(args)...);
				++producer_;
				++count_;
			}
			read_cond_.notify_one();
			return true;
		}

		/**
		 * @brief push a new data into ring buffer, it will overwrite exist data
		 * @tparam Args args' type
		 * @param args the parameters must be constructable into the value_type
		*/
		template<typename... Args>
		void push_force(Args&&... args) noexcept(
				noexcept(std::declval<internal_type>().template set_or_overwrite(std::declval<size_type>(), std::declval<decltype(std::forward<Args>(args))>()...)))
		{
			{
				std::scoped_lock lock(write_mutex_);

				buffer_.set_or_overwrite(producer_, std::forward<Args>(args)...);
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
		bool pop(reference data, time_type wait_milliseconds_time = 0)
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

			data = buffer_[consumer_];
			// the following line is just to prevent the IDE from warning us "Never used value"
			(void)data;
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
		bool pop(time_type wait_milliseconds_time = 0)
				requires std::is_default_constructible_v<value_tye>
		{
			static value_tye dummy{};
			return pop(dummy, wait_milliseconds_time);
		}

	private : internal_type		buffer_;
		size_type				producer_;
		size_type				consumer_;

		std::atomic<size_type>	count_;

		std::mutex				read_mutex_;
		std::mutex				write_mutex_;

		std::condition_variable read_cond_;
	};
}// namespace gal::toolbox::container
