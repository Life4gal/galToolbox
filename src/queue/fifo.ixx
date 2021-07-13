export module gal.toolbox.fifo;

import <mutex>;
import <atomic>;
import <condition_variable>;
import <chrono>;
import gal.toolbox.ring_buffer;

namespace gal::toolbox {
	export {
		template<typename T>
		class Fifo;
	}

	/**
	 * @brief a common fifo ringbuffer(fixed size) 
	 * @tparam T 
	*/
	template<typename T>
	class Fifo {
	public:
		using internal_type = RingBuffer<T>;

		using buffer_type = internal_type::buffer_type;
		using size_type = internal_type::size_type;
		using time_type = size_type;

		using reference = internal_type::reference;
		using const_reference = internal_type::const_reference;
		using iterator = internal_type::iterator;

		/**
		 * @brief use `size` to initialize `ringbuffer` and specify its `size`
		 * @param size 
		*/
		constexpr explicit Fifo(size_type size)
			: ring_(size), producer_(), consumer_(), count_(0) {}

		/**
		 * @brief push a new data into ringbuffer
		 * it will not overwrite exist data, return false if push fail
		 * @param data 
		 * @return 
		*/
		constexpr bool push(const_reference data) {
			if (full()) {
				return false;
			}

			{
				std::scoped_lock lock(write_lock_);
				if (full) {
					return false;
				}
				ring_[producer_] = data;
				++producer_;
				++count_;
			}
			read_cond_.notify_one();
			return true;
		}

		/**
		 * @brief push a new data into ringbuffer
		 * it will overwrite exist data
		 * @param data 
		*/
		constexpr void push_force(const_reference data) {
			{
				std::lock_guard lock(write_lock_);
				ring_[producer_] = data;
				++producer_;
				++count_;
			}
			read_cond_.notify_one();
		}

		/**
		 * @brief pop a data from ringbuffer
		 * @param data a reference to recive data
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * time == -1 means you are not willing to wait, if nothing to pop, return false
		 * time == 0  means you have enough patience to wait for a necessary data
		 * otherwise, wait for the given time, if there is still no data, return false
		 * @return pop success or not
		*/
		constexpr bool pop(reference data, time_type wait_milliseconds_time = 0) {
			std::unique_lock lock(read_lock_);
			if (empty()) {
				if (wait_milliseconds_time == static_cast<time_type>(-1)) {
					return false;
				}
				else if (wait_milliseconds_time == 0) {
					read_cond_.wait(lock);
				}
				else {
					read_cond_.wait_for(lock, std::chrono::milliseconds(wait_milliseconds_time));
				}

				if (empty()) {
					return false;
				}
			}

			data = ring_[consumer_];
			++consumer_;
			--count_;
			return true;
		}

		/**
		 * @brief pop a data from ringbuffer but ignore what is poped
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * for more detail, see `pop` with two arguments
		 * @return pop success or not
		*/
		constexpr bool pop(time_type wait_milliseconds_time = 0) {
			buffer_type dummy;
			return pop(dummy, wait_milliseconds_time);
		}

		/**
		 * @brief get exist data size current
		 * @return 
		*/
		constexpr size_type size() const {
			return ring_.distance(consumer_, producer_);
		}

		constexpr size_type capacity() const {
			return ring_.size();
		}

		constexpr bool full() {
			return count_ == capacity();
		}
		constexpr bool empty() {
			return count_ == 0;
		}

	private:
		internal_type ring_;
		iterator producer_;
		iterator consumer_;
		std::atomic<size_type> count_;

		std::mutex read_lock_;
		std::mutex write_lock_;

		std::condition_variable read_cond_;
	};
}