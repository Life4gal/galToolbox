export module gal.toolbox.message_queue;

import <cstdint>;
import <thread>;
import <atomic>;
import <mutex>;
import <condition_variable>;
import gal.toolbox.assert;

namespace gal::toolbox {
	export {
		template<typename T>
		class MssageQueue;
	}

	/**
	 * @brief a common message queue with fixed size
	 * @tparam T 
	*/
	template<typename T>
	class MessageQueue {
	public:
		using value_type = T;
		using size_type = uint64_t;
		using time_type = size_type;

		constexpr static size_type padding_size = 64;

		using pointer = value_type*;
		using const_pointer = const pointer;
		using reference = value_type bitand;
		using const_reference = const value_type bitand;
		using iterator = size_type;

		/**
		 * @brief use `capacity` to initialize `queue` and specify its `capacity`
		 * @param capacity 
		*/
		constexpr explicit MessageQueue(size_type capacity)
			: queue_(new value_type[capacity]), capacity_(capacity), producer_(), consumer_() {
			gal::toolbox::assert(queue_ not_eq nullptr);
		}

		constexpr compl MessageQueue() {
			delete[] queue_;
			queue_ = nullptr;
		}

		MessageQueue(const MessageQueue&) = delete;
		MessageQueue& operator=(const MessageQueue&) = delete;

		/**
		 * @brief push a new data into queue
		 * @param data 
		 * @return 
		*/
		constexpr bool push(const_reference data)
		{
			{
				std::scoped_lock lock(producer_lock_);
				auto next = get_next(producer_);
				if (next == consumer_.load(std::memory_order_acquire)) {
					return false;
				}

				queue_[producer_] = data;
				producer_.store(next, std::memory_order_release);
			}

			cond_.notify_all();
			return true;
		}
		
		/**
		 * @brief pop a data from ringbuffer
		 * @param data a reference to recive data
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * @return pop success or not
		*/
		constexpr bool pop(reference data, time_type wait_milliseconds_time)
		{
			std::unique_lock lock(consumer_lock_);
			auto next = get_next(consumer_);
			if (consumer_ == producer_.load()) {
				if (not cond_.wait_for(lock, std::chrono::milliseconds(wait_milliseconds_time))) {
					return false;
				}
			}

			if (consumer_ == producer_.load()) {
				return false;
			}

			data = queue_[consumer_];
			consumer_.store(next, std::memory_order_release);
			return true;
		}

		/**
		 * @brief pop a data from ringbuffer but ignore what is poped
		 * @param wait_milliseconds_time maximum time willing to wait (milliseconds)
		 * @return pop success or not
		*/
		constexpr bool pop(time_type wait_milliseconds_time) {
			value_type dummy;
			return pop(dummy, wait_milliseconds_time);
		}

	private:
		constexpr iterator get_next(iterator curr) {
			auto next = curr + 1;
			return static_cast<size_type>(next) == capacity_ ? 0 : next;
		}

		pointer const queue_;
		const size_type capacity_;
		std::atomic<iterator> producer_;
		char padding_1[padding_size];
		std::atomic<iterator> consumer_;
		char padding_2[padding_size];

		std::mutex producer_lock_;
		std::mutex consumer_lock_;

		std::condition_variable cond_;
	};
}