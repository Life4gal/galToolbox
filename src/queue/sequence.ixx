export module gal.toolbox.sequence;

import <cstdint>;
import <thread>;
import <atomic>;
import <mutex>;
import <condition_variable>;
import <chrono>;
import gal.toolbox.ring_buffer;

namespace gal::toolbox {
	export {
		template<typename T, typename ProducerCursor, typename ConsumerCursor, typename WaitStrategy>
		class Sequence;
	}

	class PlainCursor {
	public:
		using size_type = uint64_t;

	private:

	};
}