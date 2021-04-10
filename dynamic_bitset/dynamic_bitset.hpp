#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_HPP

#include <algorithm>
#include <dynamic_bitset/dynamic_bitset_fwd.hpp>
#include <limits>
#include <locale>

#ifndef GAL_ASSERT
#include <cassert>
#define GAL_ASSERT assert
#endif

namespace gal::toolbox::dynamic_bitset {
	namespace detail {
		template<typename T>
		requires std::unsigned_integral<T>
		[[nodiscard("Meaningless overhead")]] constexpr size_t bits_of_type() {
			// sizeof(T) * CHAR_BIT;
			return std::numeric_limits<T>::digits;
		}

		template<typename T, typename Allocator, template<typename, typename> typename Container>
		class bit_ref {
			friend class basic_dynamic_bitset<T, Allocator, Container>;
			using ref_container = basic_dynamic_bitset<T, Allocator, Container>;

			using value_type = typename ref_container::value_type;
			using size_type = typename ref_container::size_type;

			constexpr static auto bits_of_type = ref_container::bits_of_type;

			// note offset should less than bits_of_type
			constexpr bit_ref(value_type& value, size_type offset) noexcept
				: value(value),
				  mask(value_type{1} << offset) {
				GAL_ASSERT(offset < bits_of_type);
			}

			constexpr void do_set_value() noexcept { value |= mask; }
			constexpr void do_reset_value() noexcept { value &= ~mask; }
			constexpr void do_flip() noexcept { value ^= mask; }
			constexpr void do_assign(bool v) noexcept(noexcept(do_set_value()) && noexcept(do_reset_value())) { v ? do_set_value() : do_reset_value(); }

		public:
			auto operator&() = delete; /* NOLINT, let IDE stop prompting us that 'Do not overload unary operator&, it is dangerous' */

			[[nodiscard("Meaningless overhead")]] constexpr operator bool() const noexcept /* NOLINT, let IDE stop prompting us to add explicit for implicit conversion */ {
				return (value & mask) != 0;
			}

			[[nodiscard("Meaningless overhead")]] constexpr bool operator~() const noexcept {
				return !this->operator bool();
			}

			constexpr bit_ref& flip() noexcept(noexcept(do_flip())) {
				do_flip();
				return *this;
			}

			constexpr bit_ref& operator=(bool v) noexcept(noexcept(do_assign(v))) {
				do_assign(v);
				return *this;
			}

			constexpr bit_ref& operator=(const bit_ref& other) noexcept(noexcept(do_assign(other))) {
				do_assign(other);
				return *this;
			}

			constexpr bit_ref& operator|=(bool v) noexcept(noexcept(do_reset_value(v))) {
				if (v) {
					do_set_value();
				}
				return *this;
			}

			constexpr bit_ref& operator&=(bool v) noexcept(noexcept(do_reset_value())) {
				if (!v) {
					do_reset_value();
				}
				return *this;
			}

			constexpr bit_ref& operator^=(bool v) noexcept(noexcept(do_flip())) {
				if (v) {
					do_flip();
				}
				return *this;
			}

			constexpr bit_ref& operator-=(bool v) noexcept(noexcept(do_reset_value())) {
				if (v) {
					do_reset_value();
				}
				return *this;
			}

			constexpr auto operator<=>(const bit_ref& other) noexcept {
				return value <=> other.value;
			}

		private:
			value_type& value;
			const value_type mask;
		};
	}// namespace detail

	template<typename T, typename Allocator, template<typename, typename> typename Container>
	requires std::is_integral_v<T> &&
			requires {
		typename Container<T, Allocator>::value_type;
		typename Container<T, Allocator>::size_type;
		typename Container<T, Allocator>::allocator_type;
	} &&
			requires(Container<T, Allocator> c) {
		c.begin();
		c.end();
		c[0];
		c.get_allocator();
		c.size();
		c.resize(typename Container<T, Allocator>::size_type{}, typename Container<T, Allocator>::value_type{});
		c.clear();
		c.push_back({});
		c.pop_back();
	}
	class basic_dynamic_bitset {
	public:
		using container_type = Container<T, Allocator>;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using allocator_type = typename container_type::allocator_type;

		using reference_type = detail::bit_ref<T, Allocator, Container>;

		constexpr static auto bits_of_type = detail::bits_of_type<value_type>();
		constexpr static auto bits_of_unsigned_long = detail::bits_of_type<unsigned long>();
		constexpr static auto npos = static_cast<size_type>(-1);

	private:
		container_type container;
		size_type total_size;

		static size_type block_index(size_type pos) noexcept { return pos / bits_of_type; }
		static size_type bit_index(size_type pos) noexcept { return pos % bits_of_type; }
		static value_type bit_mask(size_type pos) noexcept(noexcept(bit_index(pos))) { return value_type{1} << bit_index(pos); }
		static value_type bit_mask(size_type first, size_type last) noexcept {
			auto res = (last == bits_of_type - 1) ? static_cast<value_type>(~0) : ((value_type{1} << (last + 1)) - 1);
			return res ^ ((value_type{1} << first) - 1);
		}
		static value_type set_block_bits(value_type block, size_type first, size_type last, bool value) noexcept(noexcept(bit_mask(first, last))) {
			if (value) {
				return block | bit_mask(first, last);
			} else {
				return block & static_cast<value_type>(~bit_mask(first, last));
			}
		}

		// Functions for operations on ranges
		static value_type set_block_partial(value_type block, size_type first, size_type last) noexcept(noexcept(set_block_bits(block, first, last))) {
			return set_block_bits(block, first, last);
		}
		static value_type set_block_full() noexcept {
			return static_cast<value_type>(~0);
		}
		static value_type reset_block_partial(value_type block, size_type first, size_type last) noexcept(noexcept(set_block_bits(block, first, last, false))) {
			return set_block_bits(block, first, last, false);
		}
		static value_type reset_block_full() noexcept {
			return 0;
		}
		static value_type flip_block_partial(value_type block, size_type first, size_type last) noexcept(noexcept(bit_mask(first, last))) {
			return block ^ bit_mask(first, last);
		}
		static value_type flip_block_full(value_type block) noexcept {
			return ~block;
		}

	public:
		constexpr basic_dynamic_bitset() noexcept(std::is_nothrow_default_constructible_v<container_type>)
			: container(),
			  total_size() {}

		constexpr explicit basic_dynamic_bitset(const allocator_type& alloc) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>)
			: container(alloc),
			  total_size() {}

		constexpr explicit basic_dynamic_bitset(size_type size, value_type value = {}, const allocator_type& alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(init_from_value_type(size, value)))
			: container(alloc),
			  total_size() {
			init_from_value_type(size, value);
		}

		template<typename Char, typename Traits, typename Alloc>
		constexpr basic_dynamic_bitset(const std::basic_string<Char, Traits, Alloc>& str,
									   typename std::basic_string<Char, Traits, Alloc>::size_type pos,
									   typename std::basic_string<Char, Traits, Alloc>::size_type n,
									   size_type size = npos,
									   const allocator_type& alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(init_from_basic_string(str, pos, n, size)))
			: container(alloc),
			  total_size() {
			init_from_basic_string(
					str,
					pos,
					n,
					size);
		}

		template<typename Char, typename Traits, typename Alloc>
		constexpr explicit basic_dynamic_bitset(const std::basic_string<Char, Traits, Alloc>& str,
												typename std::basic_string<Char, Traits, Alloc>::size_type pos = {},
												const allocator_type& alloc = {}) noexcept(noexcept(basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Alloc>::npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Alloc>::npos, npos, alloc) {}

		template<typename ContainerInputIterator>
		requires std::input_iterator<ContainerInputIterator> && requires(container_type c) {
			c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
		}
		constexpr basic_dynamic_bitset(ContainerInputIterator first, ContainerInputIterator last, const allocator_type& alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(init_from_range(first, last)))
			: container(alloc),
			  total_size() {
			init_from_range(first, last);
		}

		template<typename ContainerInputIterator>
		requires std::input_iterator<ContainerInputIterator> && requires(container_type c) {
			c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
		}
		constexpr void reinit(ContainerInputIterator first, ContainerInputIterator last) noexcept(noexcept(init_from_range(first, last))) {
			GAL_ASSERT(total_size == 0);
			init_from_range(first, last);
		}

		constexpr basic_dynamic_bitset(std::initializer_list<value_type>&& args, const allocator_type& alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, decltype(args)>&& noexcept(init_from_range(args.begin(), args.end())))
			: container(alloc),
			  total_size() {
			init_from_range(args.begin(), args.end());
		}

		~basic_dynamic_bitset() {
			GAL_ASSERT(check_invariants());
		}

		constexpr basic_dynamic_bitset(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_constructible_v<container_type>)
			: container(other.container),
			  total_size(other.total_size) {}

		basic_dynamic_bitset& operator=(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_assignable_v<container_type>) {
			container = other.container;
			total_size = other.total_size;
			return *this;
		}

		basic_dynamic_bitset(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_constructible_v<container_type>)
			: container(std::move(other.container)),
			  total_size(std::move(other.total_size)) {
			// Required so that check_invariants() works.
			GAL_ASSERT((other.container = container_type{}).empty());
			other.total_size = 0;
		}

		basic_dynamic_bitset& operator=(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_assignable_v<container_type>) {
			if (std::addressof(other) == this) {
				return *this;
			}
			container = std::move(other.container);
			total_size = std::move(other.total_size);
			// Required so that check_invariants() works.
			GAL_ASSERT((other.container = container_type{}).empty());
			other.total_size = 0;
			return *this;
		}

		void swap(basic_dynamic_bitset& other) noexcept(noexcept(std::swap(container, other.container))) {
			std::swap(container, other.container);
			std::swap(total_size, other.total_size);
		}

		allocator_type get_allocator() const noexcept(noexcept(container.get_allocator())) {
			return container.get_allocator();
		}

		size_type size() const noexcept {
			return total_size;
		}

		size_type container_size() const noexcept(noexcept(container.size())) {
			return container.size();
		}

		auto max_size() const noexcept(noexcept(container.max_size()) && noexcept(get_allocator()) && noexcept(get_allocator().max_size())) {
			auto container_max = container.max_size();
			auto alloc_max = get_allocator().max_size();

			auto max = std::min(container_max, alloc_max);
			return max <= static_cast<size_type>(-1) / bits_of_type ? max * bits_of_type : static_cast<size_type>(-1);
		}

		bool empty() const noexcept(noexcept(size())) {
			return size() == 0;
		}

		size_type capacity() const noexcept(noexcept(container.capacity())) {
			return container.capacity() * bits_of_type;
		}

		void reserve(size_type size) noexcept(noexcept(container.reserve(size))) {
			container.reserve(calc_num_blocks(size));
		}

		void shrink_to_fit() noexcept(noexcept(container.size()) && noexcept(container.capacity()) && std::is_nothrow_swappable_v<container_type>) {
			if (container.size() < container.capacity()) {
				container_type{container}.swap(container);
			}
		}

	private:
		bool unchecked_test(size_type pos) const noexcept(noexcept(bit_mask(pos)) && noexcept(block_index(pos)) && noexcept(container[0])) {
			return (container[block_index(pos)] & bit_mask(pos)) != 0;
		}

		size_type calc_num_blocks(size_type num_bits) const noexcept {
			return num_bits / bits_of_type + static_cast<size_type>(num_bits % bits_of_type != 0);
		}

		value_type& highest_block() noexcept(noexcept(container.back())) {
			GAL_ASSERT(size() > 0 && container_size() > 0);
			return container.back();
		}

		const value_type& highest_block() const noexcept(noexcept(container.back())) {
			GAL_ASSERT(size() > 0 && container_size() > 0);
			return container.back();
		}

		void init_from_value_type(size_type size, value_type value) noexcept(noexcept(calc_num_blocks(size)) && noexcept(container.resize(calc_num_blocks(size)))) {
			GAL_ASSERT(container_size() == 0);

			container.resize(calc_num_blocks(size));
			total_size = size;

			// zero out all bits at pos >= num_bits, if any;
			// note that: num_bits == 0 implies value == 0
			if (size < static_cast<size_type>(bits_of_unsigned_long)) {
				value &= ((value_type{1} << size) - 1);
			}

			constexpr auto left_shifter = [](size_type & value) constexpr->void {
				bits_of_type >= bits_of_unsigned_long ? (value = 0) : (value >>= bits_of_type);
			};

			for (auto it = container.begin();; ++it) {
				*it = static_cast<value_type>(value);
				left_shifter(value);

				if (value == 0) break;
			}
		}

		template<typename Char, typename Traits, typename Alloc>
		void init_from_basic_string(const std::basic_string<Char, Traits, Alloc>& str,
									typename std::basic_string<Char, Traits, Alloc>::size_type pos,
									typename std::basic_string<Char, Traits, Alloc>::size_type n,
									size_type size) noexcept(noexcept(calc_num_blocks(size)) && noexcept(container.resize(size))) {
			GAL_ASSERT(pos <= str.size());

			auto len = std::min(n, str.size() - pos);
			auto num_bits = size != npos ? size : len;
			container.resize(calc_num_blocks(num_bits));
			total_size = num_bits;

			auto& fac = std::use_facet<std::ctype<Char>>(std::locale());
			auto one = fac.widen('1');

			auto shorter = std::min(num_bits, len);
			using str_size_type = typename std::basic_string<Char, Traits, Alloc>::size_type;
			using str_trait_type = typename std::basic_string<Char, Traits, Alloc>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i) {
				auto c = str[begin - i];
				GAL_ASSERT(str_trait_type::eq(c, one) || str_trait_type::eq(c, fac.widen('0')));
				if (str_trait_type::eq(c, one)) {
					set(i);
				}
			}
		}

		template<typename ContainerInputIterator>
		requires std::input_iterator<ContainerInputIterator> && requires(container_type c) {
			c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
		}
		void init_from_range(ContainerInputIterator first, ContainerInputIterator end) noexcept(noexcept(container.end()) && noexcept(container.insert(container.end(), first, end)) && noexcept(container.size())) {
			GAL_ASSERT(container.size() == 0);

			container.insert(container.end(), first, end);
			total_size = container.size() * bits_of_type;
		}

		basic_dynamic_bitset& range_operation(size_type pos, size_type len,
											  value_type (*partial_block_operation)(value_type, size_type, size_type),
											  value_type (*full_block_operation)(value_type)) noexcept(noexcept(block_index(0)) && noexcept(bit_index(0)) && noexcept(container[0]) &&
																									   std::is_nothrow_invocable_v<decltype(partial_block_operation), value_type, size_type, size_type> &&
																									   std::is_nothrow_invocable_v<decltype(full_block_operation), value_type>) {
			GAL_ASSERT(pos + len <= total_size);

			// Do nothing in case of zero length
			if (len == 0) {
				return *this;
			}

			// Use an additional asserts in order to detect size_type overflow
			// For example: pos = 10, len = size_type_limit - 2, pos + len = 7
			// In case of overflow, 'pos + len' is always smaller than 'len'
			GAL_ASSERT(pos + len >= len);

			// Start and end blocks of the [pos; pos + len - 1] sequence
			auto first_block = block_index(pos);
			auto last_block = block_index(pos + len - 1);
			auto first_bit = bit_index(pos);
			auto last_bit = bit_index(pos + len - 1);

			if (first_block == last_block) {
				// Filling only a sub-block of a block
				container[first_block] = partial_block_operation(container[first_block], first_bit, last_bit);
			} else {
				// Check if the corner blocks won't be fully filled with 'value'
				auto first_block_shift = bit_index(pos) ? 1 : 0;
				auto last_block_shift = (bit_index(pos + len - 1) == bits_of_type - 1) ? 0 : 1;

				// Blocks that will be filled with ~0 or 0 at once
				auto first_full_block = first_block + first_block_shift;
				auto last_full_block = last_block - last_block_shift;

				for (auto i = first_full_block; i <= last_full_block; ++i) {
					container[i] = full_block_operation(container[i]);
				}

				// Fill the first block from the 'first' bit index to the end
				if (first_block_shift) {
					container[first_block] = partial_block_operation(container[first_block], first_bit, bits_of_type - 1);
				}

				// Fill the last block from the start to the 'last' bit index
				if (last_block_shift) {
					container = partial_block_operation(container[last_block], 0, last_bit);
				}
			}

			return *this;
		}

		/*
		 * If size() is not a multiple of bits_per_block
		 * then not all the bits in the last block are used.
		 * This function resets the unused bits (convenient
		 * for the implementation of many member functions)
		 */
		void zero_unused_bits() noexcept(noexcept(container_size()) && noexcept(calc_num_blocks(total_size)) && noexcept(count_extra_bits()) && noexcept(highest_block())) {
			GAL_ASSERT(container_size() == calc_num_blocks(total_size));

			// if != 0 this is the number of bits used in the last block
			auto extra_bits = count_extra_bits();
			if (extra_bits != 0) {
				highest_block() &= (value_type{1} << extra_bits) - 1;
			}
		}

		/*
		 * check class invariants
		 */
		[[nodiscard]] bool check_invariants() const
				noexcept(noexcept(count_extra_bits()) && noexcept(highest_block()) && noexcept(container.size()) && noexcept(container.capacity()) && noexcept(container_size()) && noexcept(size()) && noexcept(calc_num_blocks(size()))) {
			auto extra_bits = count_extra_bits();
			if (extra_bits > 0) {
				auto mask = static_cast<value_type>(~0) << extra_bits;
				if ((highest_block() & mask) != 0) {
					return false;
				}
			}
			if (container.size() > container.capacity() || container_size() != calc_num_blocks(size())) {
				return false;
			}
			return true;
		}

		size_type count_extra_bits() const noexcept(noexcept(size()) && noexcept(bit_index(size()))) {
			return bit_index(size());
		}

	public:
		// basic bit operations
		basic_dynamic_bitset& set(size_type pos, size_type len, bool value) noexcept(noexcept(range_operation(pos, len, set_block_partial, set_block_full)) && noexcept(range_operation(pos, len, reset_block_partial, reset_block_full))) {
			if (value) {
				return range_operation(pos, len, set_block_partial, set_block_full);
			} else {
				return range_operation(pos, len, reset_block_partial, reset_block_full);
			}
		}
		basic_dynamic_bitset& set(size_type pos, bool value = true) noexcept(noexcept(bit_mask(pos)) && noexcept(block_index(pos)) && noexcept(container[0]) && noexcept(reset(pos))) {
			GAL_ASSERT(pos < total_size);

			if (value) {
				container[block_index(pos)] |= bit_mask(pos);
			} else {
				reset(pos);
			}

			return *this;
		}
		basic_dynamic_bitset& set() noexcept(noexcept(container.begin()) && noexcept(container.end()) &&
											 std::is_nothrow_invocable_v<decltype(std::fill<decltype(container.begin()), decltype(container.end()), value_type>), decltype(container.begin()), decltype(container.end()), value_type>&& noexcept(zero_unused_bits())) {
			std::fill(container.begin(), container.end(), static_cast<value_type>(~0));
			zero_unused_bits();
			return *this;
		}
		basic_dynamic_bitset& reset(size_type pos, size_type len) noexcept(noexcept(range_operation(pos, len, reset_block_partial, reset_block_full))) {
			return range_operation(pos, len, reset_block_partial, reset_block_full);
		}
		basic_dynamic_bitset& reset(size_type pos) noexcept(noexcept(bit_mask(pos)) && noexcept(block_index(pos)) && noexcept(container[0])) {
			GAL_ASSERT(pos < total_size);

			container[block_index(pos)] &= ~bit_mask(pos);
			return *this;
		}
		basic_dynamic_bitset& reset() noexcept(std::is_nothrow_invocable_v<decltype(std::fill<decltype(container.begin()), decltype(container.end()), value_type>), decltype(container.begin()), decltype(container.end()), value_type>) {
			std::fill(container.begin(), container.end(), value_type{0});
			return *this;
		}
		basic_dynamic_bitset& flip(size_type pos, size_type len) noexcept(noexcept(range_operation(pos, len, flip_block_partial, flip_block_full))) {
			return range_operation(pos, len, flip_block_partial, flip_block_full);
		}
		basic_dynamic_bitset& flip(size_type pos) noexcept(noexcept(bit_mask(pos)) && noexcept(block_index(pos)) && noexcept(container[0])) {
			GAL_ASSERT(pos < total_size);

			container[block_index(pos)] ^= bit_mask(pos);
			return *this;
		}
		basic_dynamic_bitset& flip() noexcept(
				//						std::is_nothrow_invocable_v<decltype(std::for_each<decltype(container.begin()), decltype(container.end()), decltype([](value_type&) -> void {})>), decltype(container.begin()), decltype(container.end()), decltype([](value_type&) -> void {})> &&
				noexcept(container_size()) && noexcept(zero_unused_bits())) {
			//			std::for_each(container.begin(), container.end(), [](value_type& value) { value = ~value; });
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] = ~container[i];
			}
			zero_unused_bits();
			return *this;
		}

		[[nodiscard]] bool test(size_type pos) const noexcept(noexcept(unchecked_test(pos))) {
			GAL_ASSERT(pos < total_size);
			return unchecked_test(pos);
		}

		bool test_and_set(size_type pos, bool value) noexcept(noexcept(test(pos)) && noexcept(set(pos, value))) {
			auto b = test(pos);
			if (b != value) {
				set(pos, value);
			}
			return b;
		}

		[[nodiscard]] bool all() const
				noexcept(noexcept(empty()) && noexcept(count_extra_bits()) && noexcept(container_size()) && noexcept(container[0])) {
			if (empty()) {
				return true;
			}

			auto extra = count_extra_bits();
			auto all_one = static_cast<value_type>(~0);

			if (extra == 0) {
				for (size_type i = 0; i < container_size(); ++i) {
					if (container[i] != all_one) {
						return false;
					}
				}
			} else {
				for (size_type i = 0; i < container_size() - 1; ++i) {
					if (container[i] != all_one) {
						return false;
					}
				}
				if (highest_block() != (value_type{1} << extra) - 1) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] bool any() const noexcept(noexcept(container_size()) && noexcept(container[0])) {
			for (size_type i = 0; i < container_size(); ++i) {
				if (container[i]) {
					return true;
				}
			}
			return false;
		}

		[[nodiscard]] bool none() const noexcept(noexcept(any())) {
			return !any();
		}

		size_type count() const noexcept(noexcept(container_size())) {
			static_assert(sizeof(std::byte) == sizeof(unsigned char));
			static_assert(sizeof(unsigned char) == 1);
			constexpr auto table_width = 8;
			constexpr unsigned char count_table[] =
					{
							// Automatically generated by GPTableGen.exe v.1.0
							0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
							1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
							1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
							2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
							1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
							2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
							2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
							3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};
			constexpr auto pop_count = [](value_type value) constexpr noexcept {
				size_type num{};
				while (value) {
					num += count_table[value & ((value_type{1} << table_width) - 1)];
					value >>= table_width;
				}
				return num;
			};

			size_type total{};
			for (size_type i = 0; i < container_size(); ++i) {
				total += pop_count(container[i]);
			}
			return total;
		}

		reference_type operator[](size_type index) noexcept(std::is_nothrow_constructible_v<reference_type, value_type&, size_type>) {
			return {container[block_index(index)], bit_index(index)};
		}

		[[nodiscard]] bool operator[](size_type index) const noexcept(noexcept(test(index))) {
			return test(index);
		}

		void resize(size_type size, bool value = false) noexcept(noexcept(container_size()) && noexcept(calc_num_blocks(size)) && noexcept(container.resize(size_type{}, value_type{})) && noexcept(count_extra_bits()) && noexcept(container.size()) && noexcept(container[0]) && noexcept(zero_unused_bits())) {
			auto old_size = container_size();
			auto required_size = calc_num_blocks(size);

			auto v = value ? ~value_type{0} : value_type{0};

			if (required_size != old_size) {
				container.resize(required_size, v);
			}

			// At this point:
			//
			//  - if the buffer was shrunk, we have nothing more to do,
			//    except a call to zero_unused_bits()
			//
			//  - if it was enlarged, all the (used) bits in the new blocks have
			//    the correct value, but we have not yet touched those bits, if
			//    any, that were 'unused bits' before enlarging: if value == true,
			//    they must be set.
			if (value && (size > total_size)) {
				auto extra = count_extra_bits();
				if (extra) {
					GAL_ASSERT(old_size >= 1 && old_size <= container.size());

					// Set them.
					container[old_size - 1] |= (v << extra);
				}
			}

			total_size = size;
			zero_unused_bits();
		}
		void clear() noexcept(noexcept(container.clear())) {
			container.clear();
			total_size = 0;
		}
		void push_back(bool value) noexcept(noexcept(size()) && noexcept(resize(0)) && noexcept(size(), value)) {
			auto sz = size();
			resize(sz + 1);
			set(sz, value);
		}
		void pop_back() noexcept(noexcept(container_size()) && noexcept(calc_num_blocks(total_size - 1)) && noexcept(container.pop_back()) && noexcept(zero_unused_bits())) {
			auto old_size = container_size();
			auto required_size = calc_num_blocks(total_size - 1);

			if (required_size != old_size) {
				container.pop_back();
			}
			--total_size;
			zero_unused_bits();
		}
		void append(value_type value) noexcept(noexcept(count_extra_bits()) && noexcept(container.push_back(value)) && noexcept(container[0])) {
			auto extra = count_extra_bits();
			if (extra == 0) {
				// the buffer is empty, or all blocks are filled
				container.push_back(value);
			} else {
				container.push_back(value >> (bits_of_type - extra));
				container[container.size() - 2] |= (value << extra);// container.size() >= 2
			}

			total_size += bits_of_type;
			GAL_ASSERT(check_invariants());
		}

		// bitset operations
		basic_dynamic_bitset& operator&=(const basic_dynamic_bitset& other) noexcept(noexcept(container_size()) && noexcept(container[0])) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] &= other.container[i];
			}
			return *this;
		}

		basic_dynamic_bitset& operator|=(const basic_dynamic_bitset& other) noexcept(noexcept(container_size()) && noexcept(container[0])) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] |= other.container[i];
			}
			return *this;
		}

		basic_dynamic_bitset& operator^=(const basic_dynamic_bitset& other) noexcept(noexcept(container_size()) && noexcept(container[0])) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] ^= other.container[i];
			}
			return *this;
		}
		basic_dynamic_bitset& operator-=(const basic_dynamic_bitset& other) noexcept(noexcept(container_size()) && noexcept(container[0])) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] &= ~other.container[i];
			}
			return *this;
		}
		basic_dynamic_bitset& operator<<=(size_type n) noexcept(noexcept(reset()) && noexcept(container_size()) && noexcept(bit_index(n)) && noexcept(container[0]) &&
																std::is_nothrow_invocable_v<decltype(std::fill_n<decltype(container.begin()), size_type, value_type>), decltype(container.begin()), size_type, value_type>&& noexcept(zero_unused_bits())) {
			if (n >= total_size) {
				return reset();
			}

			if (n > 0) {
				auto last = container_size() - 1;// container_size() >= 1
				auto div = n / bits_of_type;	 // div <= last
				auto r = bit_index(n);

				if (r != 0) {
					auto rs = bits_of_type - r;

					for (auto i = last - div; i > 0; --i) {
						container[i + div] = (container[i] << r) | (container[i - 1] >> rs);
					}
					container[div] = container[0] << r;
				} else {
					for (auto i = last - div; i > 0; --i) {
						container[i + div] = container[i];
					}
					container[div] = container[0];
				}

				// zero out div blocks at the less significant end
				std::fill_n(container.begin(), div, value_type{0});

				// zero out any 1 bit that flowed into the unused part
				zero_unused_bits();
			}
			return *this;
		}
		basic_dynamic_bitset& operator>>=(size_type n) noexcept(noexcept(reset()) && noexcept(container_size()) && noexcept(bit_index(n)) && noexcept(container[0]) &&
																std::is_nothrow_invocable_v<decltype(std::fill_n<decltype(container.begin()), size_type, value_type>), decltype(container.begin()), size_type, value_type>) {
			if (n >= total_size) {
				return reset();
			}

			if (n > 0) {
				auto last = container_size() - 1;
				auto div = n / bits_of_type;
				auto r = bit_index(n);

				if (r != 0) {
					auto ls = bits_of_type - r;

					for (size_type i = div; i < last; ++i) {
						container[i - div] = (container[i] >> r) | (container[i + 1] << ls);
					}
					// r bits go to zero
					container[last - div] = container[last] >> r;
				} else {
					for (size_type i = div; i <= last; ++i) {
						container[i - div] = container[i];
					}
				}

				// zero out div blocks at the less significant end
				std::fill_n(container.begin() + (container_size() - div), div, value_type{0});
			}
			return *this;
		}
		basic_dynamic_bitset operator<<(size_type n) const noexcept(noexcept(operator<<=(n))) {
			auto copy{*this};
			return copy <<= n;
		}
		basic_dynamic_bitset operator>>(size_type n) const noexcept(noexcept(operator>>=(n))) {
			auto copy{*this};
			return copy >>= n;
		}
		basic_dynamic_bitset operator~() const noexcept(noexcept(flip())) {
			auto copy{*this};
			copy.flip();
			return copy;
		}

		// comparison
		//		[[nodiscard]] bool operator<=>(const basic_dynamic_bitset& other) const noexcept
		//		{
		//			return total_size <=> other.total_size && container <=> other.container;
		//		}
		[[nodiscard]] bool operator==(const basic_dynamic_bitset& other) const noexcept {
			return total_size == other.total_size && container == other.container;
		}
		[[nodiscard]] bool operator!=(const basic_dynamic_bitset& other) const noexcept {
			return !operator==(other);
		}
		[[nodiscard]] bool operator<(const basic_dynamic_bitset& other) const
				noexcept(noexcept(empty()) && noexcept(size()) && noexcept(container_size()) && noexcept(container[0])) {
			if (other.empty()) {
				return false;
			} else if (empty()) {
				return true;
			} else if (auto sz = size(), sz_other = other.size(); sz == sz_other) {
				for (size_type i = container_size() - 1; i >= 0; --i) {
					auto curr = container[i];
					auto curr_other = other.container[i];

					if (curr != curr_other) {
						return curr < curr_other;
					}
				}
				return false;
			} else {
				size_type intersect_size = std::min(sz, sz_other);

				for (auto a = sz - 1, b = sz_other - 1, count = 0; count < intersect_size; ++count, --a, --b) {
					auto curr = container[a];
					auto curr_other = other.container[b];

					if (curr != curr_other) {
						return curr < curr_other;
					}
				}
				return sz < sz_other;
			}
		}

		[[nodiscard]] bool operator<=(const basic_dynamic_bitset& other) const noexcept(noexcept(operator==(other)) && noexcept(operator<(other))) {
			return operator==(other) || operator<(other);
		}

		[[nodiscard]] bool operator>(const basic_dynamic_bitset& other) const noexcept(noexcept(operator<=(other))) {
			return !operator<=(other);
		}

		[[nodiscard]] bool operator>=(const basic_dynamic_bitset& other) const noexcept(noexcept(operator<(other))) {
			return !operator<(other);
		}

		template<typename To = unsigned long>
		requires(sizeof(To) >= sizeof(T))
				To cast_to()
		const noexcept(noexcept(empty()) && noexcept(detail::bits_of_type<To>()) && noexcept(block_index(0))) {
			if (empty()) {
				return {};
			}

			auto maximum_size = std::min(total_size, static_cast<size_type>(detail::bits_of_type<To>()));
			auto last_block = block_index(maximum_size - 1);

			GAL_ASSERT((last_block * bits_of_type) < static_cast<size_type>(detail::bits_of_type<To>()));

			To result{};
			for (size_type i = 0; i < last_block; ++i) {
				auto offset = i * bits_of_type;
				result |= static_cast<To>(container[i]) << offset;
			}
			return result;
		}

		bool is_subset_of(const basic_dynamic_bitset& other) const
				noexcept(noexcept(container.cbegin()) &&
						 std::is_nothrow_invocable_v<
								 decltype(std::equal<typename container_type::const_iterator, typename container_type::const_iterator, decltype([](value_type, value_type) -> bool { return true; })>),
								 typename container_type::const_iterator, typename container_type::const_iterator,
								 typename container_type::const_iterator, typename container_type::const_iterator,
								 decltype([](value_type, value_type) -> bool { return true; })>) {
			GAL_ASSERT(size() == other.size());

			return std::equal(container.cbegin(), container.cend(), other.container.cbegin(), other.container.cend(), [](value_type v1, value_type v2) { return !(v1 & ~v2); });
		}

		bool is_proper_subset_of(const basic_dynamic_bitset& other) const
				noexcept(noexcept(container_size()) && noexcept(container[0])) {
			GAL_ASSERT(size() == other.size());
			GAL_ASSERT(container_size() == other.container_size());

			bool proper = false;
			for (size_type i = 0; i < container_size(); ++i) {
				auto a = container[i];
				auto b = other.container[i];

				if (a & ~b) {
					// not a subset at all
					return false;
				}
				if (b & ~a) {
					proper = true;
				}
			}
			return proper;
		}

		bool intersects(const basic_dynamic_bitset& other) const noexcept(noexcept(container_size()) && noexcept(container[0])) {
			auto intersect_size = std::min(container_size(), other.container_size());

			for (size_type i = 0; i < intersect_size; ++i) {
				if (container[i] & other.container[i]) {
					return true;
				}
			}
			return false;
		}
	};

}// namespace gal::toolbox::dynamic_bitset

//namespace std {
//	template<typename T>
//	struct iterator_traits<gal::toolbox::basic_dynamic_bitset::detail::bit_it<T>>
//		: public gal::toolbox::basic_dynamic_bitset::detail::bit_it<T> {};
//}// namespace std

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_HPP
