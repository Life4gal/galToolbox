export module gal.toolbox.ring_buffer;

import <type_traits>;
import <iterator>;
import <memory>;
import gal.toolbox.assert;

namespace gal::toolbox {
	export {
		template <typename T, typename Alloc = std::allocator<T>>
		class ring_buffer;
	}

	/**
	 * @brief a common ring buffer with a fixed capacity
	 * @tparam T value type
	 * @tparam Alloc allocator type
	*/
	template <typename T, typename Alloc>
	class ring_buffer {
		using ring_buffer_reference = ring_buffer<T, Alloc>bitand;
		using const_ring_buffer_reference = const ring_buffer<T, Alloc>bitand;
		using ring_buffer_rreference = ring_buffer<T, Alloc>and;
	public:
		using allocator_type = Alloc;
		using allocator_trait_type = std::allocator_traits<allocator_type>;

		using value_type = typename allocator_type::value_type;
		using size_type = typename allocator_type::size_type;
		using mask_type = size_type;
		using iterator = size_type;

		using reference = value_type bitand;
		using const_reference = const value_type bitand;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		/**
	 * @brief use `capacity` to initialize `ring buffer` and specify its `capacity`
	 * @param capacity capacity must be 2 xor n
	*/
		constexpr explicit ring_buffer(const size_type capacity)
			noexcept(
				noexcept(allocator_trait_type::allocate(std::declval<allocator_type>(), std::declval<size_type>()))
				//noexcept(std::is_nothrow_constructible_v<pointer, decltype(new value_type[])>)
				)
			: buffer_(allocator_trait_type::allocate(allocator_, capacity)),
			size_(0),
			capacity_(capacity),
			mask_(capacity - 1)
			//	: buffer_(new value_type[capacity]), size_(0), capacity_(capacity), mask_(capacity - 1)
		{
			gal::toolbox::assert((capacity_ bitand mask_) == 0);
			gal::toolbox::assert(buffer_ not_eq nullptr);
		}

		constexpr compl ring_buffer() noexcept
		{
			for (size_type i = size_ - 1; i >= 0; --i)
			{
				allocator_trait_type::destroy(allocator_, std::next(buffer_, i));
			}
			allocator_trait_type::deallocate(allocator_, buffer_, capacity_);

			//delete[] buffer_;
			buffer_ = nullptr;
		}

		constexpr ring_buffer(const_ring_buffer_reference other)
			noexcept(
				noexcept(std::is_nothrow_copy_constructible_v<value_type>)
				)
			: buffer_(allocator_trait_type::allocate(allocator_, other.capacity_)),
			size_(other.size_),
			capacity_(other.capacity_),
			mask_(other.mask_)
			//: buffer_(new value_type[other.capacity_]), size_(other.size_), capacity_(other.capacity_), mask_(other.mask_)
		{
			static_assert(std::is_copy_constructible_v<value_type>);
			for (size_type i = 0; i < other.size_; ++i)
			{
				allocator_trait_type::construct(allocator_, std::next(buffer_, i), *std::next(other.buffer_, i));
				// new(std::next(buffer_, i)) value_type{std::next(other.buffer_, i)};
			}
		}

		constexpr ring_buffer_reference operator=(const_ring_buffer_reference other)
			noexcept(
				noexcept(std::is_nothrow_copy_assignable_v<value_type>)
				)
		{
			static_assert(std::is_copy_assignable_v<value_type>);

			if (std::addressof(other) not_eq this)
			{
				// we only copy parts of the same length, other parts do not provide any guarantee
				for (size_type i = 0; i < std::min(other.size_, size_); ++i)
				{
					*std::next(buffer_, i) = *std::next(other.buffer_, i);
				}
			}

			return *this;
		}

		constexpr ring_buffer(ring_buffer_rreference other) noexcept
			: buffer_(std::exchange(other.buffer_, nullptr)),
			size_(std::exchange(other.size_, 0)),
			capacity_(other.capacity_),
			mask_(other.mask_) { }

		constexpr ring_buffer_reference operator=(ring_buffer_rreference other) noexcept
		{
			buffer_ = std::exchange(other.buffer_, buffer_);
			size_ = std::exchange(other.size_, 0);
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
			return *std::next(buffer_, index bitand mask_);
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
			return *std::next(buffer_, index bitand mask_);
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
		{
			const auto real_index = index bitand mask_;
			if (real_index > size_)
			{
				// has not been constructed yet
				allocator_trait_type::construct(allocator_,
					std::next(buffer_, real_index),
					std::forward<Args>(args)...);
			}

			return *std::next(buffer_, real_index);
		}

		constexpr size_type size() const noexcept
		{
			return size_;
		}

		constexpr size_type capacity() const noexcept
		{
			return capacity_;
		}

		/**
	 * @brief get where the pos actually in
	 * @param pos given pos
	 * @return actually index
	*/
		constexpr size_type index(size_type pos) const noexcept {
			return pos bitand mask_;
		}

		/**
		 * @brief get the distance between begin and end in buffer,
		 * begin is greater than end or vice versa does not affect the result
		 * @param begin given begin iterator
		 * @param end given end iterator
		 * @return distance
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
		pointer         buffer_;
		size_type       size_;
		const size_type capacity_;
		const mask_type mask_;

		// todo: do we really need this allocator? Or is it worth it?
		inline static allocator_type allocator_;
	};
}
