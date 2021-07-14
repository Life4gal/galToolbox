export module gal.toolbox.ring_buffer;

import <cstdint>;
import gal.toolbox.assert;

namespace gal::toolbox {
	export {
		template<typename T>
		class RingBuffer;
	}

	/**
	 * @brief a common ringbuffer with a fixed size
	 * @tparam T 
	*/
	template<typename T>
	class RingBuffer {
	public:
		using buffer_type = T;
		using size_type = uint64_t;
		using mask_type = uint64_t;

		using reference = buffer_type bitand;
		using const_reference = const reference;
		using pointer = buffer_type*;
		using const_pointer = const pointer;
		using iterator = size_type;

		/**
		 * @brief use `size` to initialize `ringbuffer` and specify its `size`
		 * @param size size must be 2 xor n
		*/
		constexpr explicit RingBuffer(size_type size)
			noexcept(
				noexcept(std::is_nothrow_constructible_v<pointer, decltype(new buffer_type[])>)
			)
			: size_(size), mask_(size - 1), buffer_(new buffer_type[size]) {
			gal::toolbox::assert((size_ bitand mask_) == 0);
			gal::toolbox::assert(buffer_ not_eq nullptr);
		}

		constexpr compl RingBuffer() noexcept {
			delete[] buffer_;
			buffer_ = nullptr;
		}

		constexpr reference operator[](size_type index) noexcept(noexcept(std::declval<pointer>().operator[](std::declval<size_type>()))) {
			return buffer_[index bitand mask_];
		}
		constexpr const_reference operator[](size_type index) const noexcept(noexcept(std::declval<pointer>().operator[](std::declval<size_type>()))) {
			return buffer_[index bitand mask_];
		}

		/**
		 * @brief get buffer's total size
		 * @return 
		*/
		constexpr size_type size() const noexcept {
			return size_;
		}

		/**
		 * @brief get where the pos actually in
		 * @param pos 
		 * @return 
		*/
		constexpr size_type index(size_type pos) const noexcept {
			return pos bitand mask_;
		}

		/**
		 * @brief get the distance between begin and end in buffer, 
		 * begin is greater than end or vice versa does not affect the result
		 * @param begin 
		 * @param end 
		 * @return 
		*/
		constexpr size_type distance(iterator begin, iterator end) noexcept {
			begin and_eq mask_;
			end and_eq mask_;

			if (end >= begin) {
				return end - begin;
			}
			else {
				return size_ - (begin - end);
			}
		}

	private:
		pointer const buffer_;
		const size_type size_;
		const mask_type mask_;
	};
}
