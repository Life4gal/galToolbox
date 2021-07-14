export module gal.toolbox.sequence;

import <cstdint>;
import <thread>;
import <atomic>;
import <mutex>;
import <condition_variable>;
import <chrono>;
import gal.toolbox.ring_buffer;
import gal.toolbox.assert;

namespace gal::toolbox {
	export {
		template<typename T, typename ProducerCursor, typename ConsumerCursor, typename WaitStrategy>
		class Sequence;
	}

	class PlainCursor {
	public:
		using size_type = uint64_t;

		constexpr static size_type padding_size = 8;

		constexpr PlainCursor() noexcept : sequence_(0), padding_() {}

		constexpr explicit PlainCursor(size_type sequence) noexcept : sequence_(sequence), padding_() {}

		constexpr size_type get() const noexcept {
			return sequence_;
		}

		constexpr void set(size_type sequence) noexcept {
			sequence_ = sequence;
		}

		constexpr void operator++() noexcept {
			++sequence_;
		}

		constexpr void operator++(int) noexcept {
			++sequence_;
		}

	private:
		size_type sequence_;
		size_type padding_[padding_size];
	};
}