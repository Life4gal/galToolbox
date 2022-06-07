#pragma once

#ifndef _MSC_VER
// gcc/clang not support 'constexpr vector' yet
#define GALTOOLBOX_DYNAMIC_BITSET_NOT_SUPPORTED
#endif

#ifndef GALTOOLBOX_DYNAMIC_BITSET_NOT_SUPPORTED

#include <array>
#include <iosfwd>
#include <locale>
#include <memory>
#include <string>
#include <vector>

#include <galToolbox/utils/assert.hpp>

namespace gal::toolbox::container
{
	class basic_dynamic_bitset
	{
	public:
		class bit_trait;
		class bit_const_reference;
		class bit_reference;
		class bit_const_iterator;
		class bit_iterator;

		friend class bit_reference;
		friend class bit_iterator;

		using container = std::vector<std::uint64_t>;

		using value_type = container::value_type;
		using size_type = container::size_type;

		using reference = bit_reference;
		using const_reference = bit_const_reference;
		using iterator = bit_iterator;
		using const_iterator = bit_const_iterator;

		using real_reference = container::reference;
		using real_const_reference = container::const_reference;
		using real_iterator = container::iterator;
		using real_const_iterator = container::const_iterator;

		constexpr static size_type bits_of_type = std::numeric_limits<value_type>::digits;
		constexpr static size_type bits_of_unsigned_long = std::numeric_limits<unsigned long>::digits;
		constexpr static size_type npos = static_cast<size_type>(-1);

		class bit_trait
		{
		public:
			/**
			 * @brief get the block where the current position is
			 * @param pos the pos of bit is
			 * @return index
			 */
			[[nodiscard]] constexpr static size_type block_index(const size_type pos) noexcept { return pos / bits_of_type; }

			/**
			 * @brief get the bit of the current position (in the block where the current position is)
			 * @param pos the pos of the bit is
			 * @return index
			 */
			[[nodiscard]] constexpr static size_type bit_index(const size_type pos) noexcept { return pos % bits_of_type; }

			/**
			 * @brief get the bit mask of the current position (in the block where the current position is)
			 * @param pos the pos of bit is
			 * @return mask
			 */
			[[nodiscard]] constexpr static value_type bit_mask(const size_type pos) noexcept(noexcept(bit_index(std::declval<size_type>()))) { return value_type{1} << bit_index(pos); }

			/**
			 * @brief get the bit mask from first to last
			 * @param first begin for get mask
			 * @param last end for get mask
			 * @return mask
			 */
			[[nodiscard]] GAL_ASSERT_CONSTEXPR static value_type bit_mask(
					const size_type first,
					const size_type last) noexcept
			{
				gal_assert(first <= last, "last must greater_equal than first");
				gal_assert(last < bits_of_type, "last should not greater than bits_of_type");
				const auto res = (last == bits_of_type - 1)
					                 ? compl static_cast<value_type>(0)
					                 : ((value_type{1} << (last + 1)) - 1);
				return res xor ((value_type{1} << first) - 1);
			}

			/**
			 * @brief set the block's set by the bit mask from first to last and set (wont change itself)
			 * @param block current block
			 * @param first begin for get mask
			 * @param last end for get mask
			 * @param set set or reset
			 * @return block after set
			 */
			[[nodiscard]] constexpr static value_type set_block_bits(
					const value_type block,
					const size_type first,
					const size_type last,
					const bool set) noexcept(noexcept(bit_mask(std::declval<size_type>(), std::declval<size_type>())))
			{
				if (set) { return block bitor bit_mask(first, last); }
				return block bitand compl bit_mask(first, last);
			}

			/**
			 * @brief set the block's value by the bit mask from first to last (wont change itself)
			 * @param block current block
			 * @param first begin for get mask
			 * @param last end for get mask
			 * @return block after set
			 */
			[[nodiscard]] constexpr static value_type set_block_partial(const value_type block,
			                                                            const size_type first,
			                                                            const size_type last) noexcept(noexcept(set_block_bits(std::declval<value_type>(),
			                                                                                                                   std::declval<size_type>(),
			                                                                                                                   std::declval<size_type>(),
			                                                                                                                   std::declval<bool>()))) { return set_block_bits(block, first, last, true); }

			/**
			 * @brief set the block's value to full (wont change itself)
			 * @return ~0
			 */
			[[nodiscard]] constexpr static value_type set_block_full(value_type) noexcept { return ~static_cast<value_type>(0); }

			/**
			 * @brief reset the block's value by the bit mask from first to last (wont change itself)
			 * @param block current block
			 * @param first begin for get mask
			 * @param last ebd for get mask
			 * @return block after reset
			 */
			[[nodiscard]] constexpr static value_type reset_block_partial(
					const value_type block,
					const size_type first,
					const size_type last) noexcept(noexcept(set_block_bits(std::declval<value_type>(),
					                                                       std::declval<size_type>(),
					                                                       std::declval<size_type>(),
					                                                       std::declval<bool>()))) { return set_block_bits(block, first, last, false); }

			/**
			 * @brief reset the block's value to full (wont change itself)
			 * @return 0
			 */
			[[nodiscard]] constexpr static value_type reset_block_full(value_type) noexcept { return 0; }

			/**
			 * @brief flip the block's value by the bit mask from first to last (wont change itself)
			 * @param block current block
			 * @param first begin for get mask
			 * @param last end for get mask
			 * @return block after flip
			 */
			[[nodiscard]] GAL_ASSERT_CONSTEXPR static value_type flip_block_partial(
					const value_type block,
					const size_type first,
					const size_type last) noexcept(noexcept(bit_mask(std::declval<size_type>(), std::declval<size_type>()))) { return block xor bit_mask(first, last); }

			/**
			 * @brief flip the block's value to full (wont change itself)
			 * @param block current block
			 * @return block after flip
			 */
			[[nodiscard]] constexpr static value_type flip_block_full(const value_type block) noexcept { return compl block; }
		};

		class bit_const_reference
		{
		public:
			friend class basic_dynamic_bitset;
			friend class bit_iterator;
			friend class bit_const_iterator;

			using container = basic_dynamic_bitset;

			using reference = real_reference;
			using const_reference = real_const_reference;

		private:
			constexpr bit_const_reference(const_reference value, const size_type offset) noexcept
				: value_(value),
				  mask_(value_type{1} << offset) {}

			// let IDE stop prompting us to add explicit for implicit conversion
			// ReSharper disable once CppNonExplicitConvertingConstructor
			constexpr bit_const_reference(const bit_reference& other) noexcept
				: value_(other.value_),
				  mask_(other.mask_) {}

		public:
			// let IDE stop prompting us that 'Do not overload unary operator bitand, it is dangerous'
			auto operator bitand() = delete;/* NOLINT */

			// let IDE stop prompting us to add explicit for implicit conversion
			// ReSharper disable once CppNonExplicitConversionOperator
			[[nodiscard]] constexpr operator bool() const noexcept { return (value_ bitand mask_) not_eq 0; }

			[[nodiscard]] constexpr bool operator~() const noexcept { return not this->operator bool(); }

		private:
			const_reference value_;
			const value_type mask_;
		};

		class bit_reference
		{
		public:
			friend class basic_dynamic_bitset;
			friend class bit_iterator;
			friend class bit_const_iterator;
			friend class bit_const_reference;

			using container = basic_dynamic_bitset;

			using reference = real_reference;
			using const_reference = real_const_reference;

		private:
			constexpr bit_reference(reference value, const size_type offset) noexcept
				: value_(value),
				  mask_(value_type{1} << offset) {}

		public:
			// let IDE stop prompting us that 'Do not overload unary operator bitand, it is dangerous'
			auto operator bitand() = delete;/* NOLINT */

			// let IDE stop prompting us to add explicit for implicit conversion
			// ReSharper disable once CppNonExplicitConversionOperator
			[[nodiscard]] constexpr operator bool() const noexcept { return (value_ bitand mask_) not_eq 0; }

			[[nodiscard]] constexpr bool operator~() const noexcept { return not this->operator bool(); }

			constexpr bit_reference& flip() noexcept
			{
				value_ xor_eq mask_;
				return *this;
			}

			constexpr bit_reference& operator=(const bool value) noexcept// NOLINT(misc-unconventional-assign-operator)
			{
				if (value) { value_ or_eq mask_; }
				else { value_ and_eq compl mask_; }
				return *this;
			}

			constexpr bit_reference& operator or_eq(const bool value) noexcept
			{
				if (value) { value_ or_eq mask_; }
				return *this;
			}

			constexpr bit_reference& operator and_eq(const bool value) noexcept
			{
				if (not value) { value_ and_eq compl mask_; }
				return *this;
			}

			constexpr bit_reference& operator xor_eq(const bool value) noexcept
			{
				if (value) { value_ xor_eq mask_; }
				return *this;
			}

			constexpr bit_reference& operator-=(const bool value) noexcept
			{
				if (value) { value_ and_eq compl mask_; }
				return *this;
			}

			// let IDE stop prompting us to add explicit for implicit conversion
			// ReSharper disable once CppNonExplicitConversionOperator
			constexpr operator bit_const_reference() const noexcept { return {*this}; }

		private:
			reference value_;
			const value_type mask_;
		};

		class bit_const_iterator
		{
		public:
			friend class basic_dynamic_bitset;

			using container = basic_dynamic_bitset;

			// bit_const_iterator 's value_type is bit_const_reference
			using value_type = bit_const_reference;

			using iterator = real_iterator;
			using const_iterator = real_const_iterator;
			using iterator_category = iterator::iterator_category;
			using difference_type = iterator::difference_type;

		private:
			constexpr bit_const_iterator(const const_iterator& it, const size_type offset)
			// NOLINT(clang-diagnostic-invalid-constexpr)
				: iterator_(it),
				  index_(offset) { }

		public:
			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_const_iterator operator+(const size_type n) const noexcept
			{
				gal_assert(index_ + n > index_, "offset overflow");
				return {iterator_, index_ + n};
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_const_iterator operator-(const size_type n) const noexcept
			{
				gal_assert(index_ >= n, "offset underflow");
				return {iterator_, index_ - n};
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR size_type distance(const bit_const_iterator& other) const noexcept
			{
				gal_assert(other.index_ <= index_, "target's offset greater than this");
				return index_ - other.index_;
			}

			[[nodiscard]] constexpr bool operator==(const bit_const_iterator& other) const noexcept { return index_ == other.index_ and iterator_ == other.iterator_; }

			[[nodiscard]] constexpr bool operator not_eq(const bit_const_iterator& other) const noexcept { return not this->operator==(other); }

			[[nodiscard]] constexpr value_type operator*() const noexcept// NOLINT(clang-diagnostic-invalid-constexpr)
			{
				return {*std::next(iterator_, static_cast<std::iter_difference_t<const_iterator>>(bit_trait::block_index(index_))), bit_trait::bit_index(index_)};
			}

		private:
			const_iterator iterator_;
			size_type index_;
		};

		class bit_iterator
		{
		public:
			friend class basic_dynamic_bitset;
			friend class bit_const_iterator;

			using container = basic_dynamic_bitset;

			// bit_iterator 's value_type is bit_reference
			using value_type = bit_reference;

			using iterator = real_iterator;
			using const_iterator = real_const_iterator;
			using iterator_category = iterator::iterator_category;
			using difference_type = iterator::difference_type;

		private:
			constexpr bit_iterator(const iterator& it, const size_type offset)
			// NOLINT(clang-diagnostic-invalid-constexpr)
				: iterator_(it),
				  index_(offset) { }

		public:
			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_iterator operator++(int) noexcept
			{
				gal_assert(index_ + 1 > index_, "offset overflow");
				auto copy{*this};
				++index_;
				return copy;
			}

			GAL_ASSERT_CONSTEXPR bit_iterator& operator++() noexcept
			{
				gal_assert(index_ + 1 > index_, "offset overflow");
				++index_;
				return *this;
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_iterator operator--(int) noexcept
			{
				gal_assert(index_ > 0, "offset underflow");
				auto copy{*this};
				--index_;
				return copy;
			}

			GAL_ASSERT_CONSTEXPR bit_iterator& operator--() noexcept
			{
				gal_assert(index_ > 0, "offset underflow");
				--index_;
				return *this;
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_iterator operator+(const size_type n) const noexcept
			{
				gal_assert(index_ + n > index_, "offset overflow");
				return {iterator_, index_ + n};
			}

			GAL_ASSERT_CONSTEXPR bit_iterator& operator+=(const size_type n) noexcept
			{
				gal_assert(index_ + n > index_, "offset overflow");
				index_ += n;
				return *this;
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR bit_iterator operator-(const size_type n) const noexcept
			{
				gal_assert(index_ >= n, "offset underflow");
				return {iterator_, index_ - n};
			}

			GAL_ASSERT_CONSTEXPR bit_iterator& operator-=(const size_type n) noexcept
			{
				gal_assert(index_ >= n, "offset underflow");
				index_ -= n;
				return *this;
			}

			[[nodiscard]] GAL_ASSERT_CONSTEXPR size_type distance(const bit_iterator& other) const noexcept
			{
				gal_assert(other.index_ <= index_, "target's offset greater than this");
				return index_ - other.index_;
			}

			[[nodiscard]] constexpr bool operator==(const bit_iterator& other) const noexcept { return index_ == other.index_ and iterator_ == other.iterator_; }

			[[nodiscard]] constexpr bool operator not_eq(const bit_iterator& other) const noexcept { return not this->operator==(other); }

			[[nodiscard]] constexpr value_type operator*() const noexcept// NOLINT(clang-diagnostic-invalid-constexpr)
			{
				return {*std::ranges::next(iterator_, static_cast<std::iter_difference_t<iterator>>(bit_trait::block_index(index_))), bit_trait::bit_index(index_)};
			}

			// let IDE stop prompting us to add explicit for implicit conversion
			// ReSharper disable once CppNonExplicitConversionOperator
			[[nodiscard]] constexpr operator bit_const_iterator() const noexcept { return {iterator_, index_}; }

		private:
			iterator iterator_;
			size_type index_;
		};

		/**
		 * @brief get size (bits we hold, not container size)
		 * @return bits we hold
		 */
		[[nodiscard]] constexpr size_type size() const noexcept { return total_; }

		/**
		 * @brief get container's real size we used
		 * @return container's size
		 */
		[[nodiscard]] constexpr size_type container_size() const
		noexcept(noexcept(std::declval<container>().size())) { return container_.size(); }

		/**
		 * @brief get the max bits we can hold (normally, bits_of_type * max_size_of_container)
		 * @return max bits size
		 */
		[[nodiscard]] constexpr size_type max_size() const
		noexcept(
			noexcept(std::declval<container>().max_size()))
		{
			const auto container_max = container_.max_size();
			return container_max <= static_cast<size_type>(-1) / bits_of_type
				       ? container_max * bits_of_type
				       : static_cast<size_type>(-1);
		}

		/**
		 * @brief are we hold zero bits?
		 * @return empty
		 */
		[[nodiscard]] constexpr bool empty() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().size())) { return size() == 0; }

		/**
		 * @brief get the capacity of bits we hold (normally, bits_of_type * capacity_of_container)
		 * @return what capacity is now
		 */
		[[nodiscard]] constexpr size_type capacity() const
		noexcept(noexcept(std::declval<container>().capacity())) { return container_.capacity() * bits_of_type; }

	private:
		/**
		 * @brief just check a bit
		 * @param pos bit's pos
		 * @return it this bit set
		 */
		[[nodiscard]] constexpr bool unchecked_test(const size_type pos) const
		noexcept(
			noexcept(std::declval<container>()[0])) { return (container_[bit_trait::block_index(pos)] bitand bit_trait::bit_mask(pos)) not_eq 0; }

		/**
		 * @brief calc how many blocks we need to store num_bits bits
		 * @param num_bits how many bits we want to hold
		 * @return blocks we need
		 */
		[[nodiscard]] constexpr static size_type calc_blocks_needed(const size_type num_bits) noexcept { return num_bits / bits_of_type + static_cast<size_type>(num_bits % bits_of_type not_eq 0); }

		/**
		 * @brief get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR real_reference highest_block() noexcept(noexcept(std::declval<container>().back()))
		{
			gal_assert(size() > 0 && container_size() > 0, "empty container");
			return container_.back();
		}

		/**
		 * @brief get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR real_const_reference highest_block() const
		noexcept(noexcept(std::declval<container>().back()))
		{
			gal_assert(size() > 0 && container_size() > 0, "empty container");
			return container_.back();
		}

		/**
		 * @brief init a dynamic_bitset from value_type
		 * @param size how many bits we want to hold
		 * @param value the value for init
		 */
		GAL_ASSERT_CONSTEXPR void init_from_value_type(const size_type size,
		                                               value_type value) noexcept(
			noexcept(std::declval<container>().resize(calc_blocks_needed(std::declval<size_type>()))))
		{
			gal_assert(container_size() == 0, "empty container");

			container_.resize(calc_blocks_needed(size));
			total_ = size;

			// zero out all bits at pos >= num_bits, if any;
			// note that: num_bits == 0 implies value == 0
			if (size < static_cast<size_type>(bits_of_unsigned_long)) { value and_eq ((value_type{1} << size) - 1); }

			constexpr auto left_shifter = [](real_reference v) constexpr-> void
			{
				if constexpr (bits_of_type >= bits_of_unsigned_long) { v = 0; }
				else { v = static_cast<value_type>(static_cast<decltype(bits_of_unsigned_long)>(v) >> bits_of_type); }
			};

			for (auto it = container_.begin();; ++it)
			{
				*it = value;
				left_shifter(value);

				if (value == 0) break;
			}
		}

		/**
		 * @brief init a dynamic_bitset from str[pos~pos+n]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Allocator alloc_type of str
		 * @param str the str we used
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size how many bits we want(will) to hold, better make sure it greater than n
		 */
		template<typename Char, typename Traits, typename Allocator>
		GAL_ASSERT_CONSTEXPR void init_from_basic_string(
				const std::basic_string<Char, Traits, Allocator>bitand str,
				typename std::basic_string<Char, Traits, Allocator>::size_type pos,
				typename std::basic_string<Char, Traits, Allocator>::size_type n,
				size_type size) noexcept(noexcept(std::declval<container>().resize(std::declval<size_type>())))
		{
			gal_assert(pos <= str.size(), "given pos greater than the str 's size");
			gal_assert(n <= size, "container's size cannot less than n");

			// make sure the end pos is valid
			auto len = std::min(n, str.size() - pos);
			auto num_bits = size not_eq npos
				                ? size
				                : len;
			container_.resize(calc_blocks_needed(num_bits));
			total_ = num_bits;

			auto& fac = std::use_facet<std::ctype<Char>>(std::locale());
			auto one = fac.widen('1');
			auto zero = fac.widen('0');

			auto shorter = std::min(num_bits, len);
			using str_size_type = typename std::basic_string<Char, Traits, Allocator>::size_type;
			using str_trait_type = typename std::basic_string<Char, Traits, Allocator>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i)
			{
				auto c = str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (not is_one and not str_trait_type::eq(c, zero)) { break; }

				if (is_one) { set(i); }
			}
		}

		/**
		 * @brief init a dynamic_bitset from str_view[pos~pos+n]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str the str_view we used
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size how many bits we want(will) to hold, better make sure it greater than n
		 */
		template<typename Char, typename Traits>
		GAL_ASSERT_CONSTEXPR void init_from_basic_string_view(
				const std::basic_string_view<Char, Traits> str,
				typename std::basic_string_view<Char, Traits>::size_type pos,
				typename std::basic_string_view<Char, Traits>::size_type n,
				size_type size) noexcept(noexcept(std::declval<container>().resize(std::declval<size_type>())))
		{
			gal_assert(pos <= str.size(), "given pos greater than the str 's size");
			gal_assert(n <= size, "container's size cannot less than n");

			// make sure the end pos is valid
			auto len = std::min(n, str.size() - pos);
			auto num_bits = size not_eq npos
				                ? size
				                : len;
			container_.resize(calc_blocks_needed(num_bits));
			total_ = num_bits;

			auto& fac = std::use_facet<std::ctype<Char>>(std::locale());
			auto one = fac.widen('1');
			auto zero = fac.widen('0');

			auto shorter = std::min(num_bits, len);
			using str_size_type = typename std::basic_string_view<Char, Traits>::size_type;
			using str_trait_type = typename std::basic_string_view<Char, Traits>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i)
			{
				auto c = str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (not is_one and not str_trait_type::eq(c, zero)) { break; }

				if (is_one) { set(i); }
			}
		}

		/**
		 * @brief process the block between pos and pos+len
		 * @param pos begin pos
		 * @param len length
		 * @param partial_block_operation range processor, process a block (first-arg) from second-arg to third-arg
		 * @param full_block_operation range processor, process a full block
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& range_operation(
				const size_type pos,
				const size_type len,
				value_type (*partial_block_operation)(value_type, size_type, size_type),
				value_type (*full_block_operation)(value_type))
		{
			gal_assert(pos + len <= total_, "offset greater than the total bits");

			// do nothing in case of zero length
			if (len == 0) { return *this; }

			// use an additional assert in order to detect size_type overflow
			// for example: pos = 10, len = size_type_limit - 2, pos + len = 7
			// in case of overflow, 'pos + len' is always smaller than 'len'
			gal_assert(pos + len >= len, "offset overflow");

			// start and end blocks of the [pos : pos + len - 1] sequence
			auto first_block = bit_trait::block_index(pos);
			auto last_block = bit_trait::block_index(pos + len - 1);
			auto first_bit = bit_trait::bit_index(pos);

			if (auto last_bit = bit_trait::bit_index(pos + len - 1); first_block == last_block)
			{
				// filling only a sub-block of a block
				container_[first_block] = partial_block_operation(container_[first_block], first_bit, last_bit);
			}
			else
			{
				// check if the corner blocks won't be fully filled with 'value'
				const decltype(first_bit) first_block_shift = (first_bit == 0)
					                                              ? 0
					                                              : 1;
				const decltype(last_bit) last_block_shift = (last_bit == bits_of_type - 1)
					                                            ? 0
					                                            : 1;

				// middle
				// blocks that will be filled with ~0 or 0 at once
				const decltype(first_block) first_full_block = first_block + first_block_shift;
				const decltype(last_block) last_full_block = last_block - last_block_shift;

				for (auto i = first_full_block; i <= last_full_block; ++i) { container_[i] = full_block_operation(container_[i]); }

				// left
				// fill the first block from the 'first' bit index to the end
				if (first_block_shift)
				{
					container_[first_block] = partial_block_operation(container_[first_block],
					                                                  first_bit,
					                                                  bits_of_type - 1);
				}

				// right
				// fill the last block from the start to the 'last' bit index
				if (last_block_shift) { container_[last_block] = partial_block_operation(container_[last_block], 0, last_bit); }
			}

			return *this;
		}

		/**
		 * @brief count extra bits (in last block)
		 * @return extra bits
		 */
		[[nodiscard]] constexpr size_type count_extra_bits() const
		noexcept(
			noexcept(bit_trait::bit_index(std::declval<basic_dynamic_bitset>().size()))) { return bit_trait::bit_index(size()); }

		/**
		 * @brief if size() is not a multiple of bits_per_block then not all the bits in the last block are used.
		 * this function resets the unused bits (convenient for the implementation of many member functions)
		 */
		GAL_ASSERT_CONSTEXPR void zero_unused_bits() noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().highest_block()))
		{
			gal_assert(container_size() == calc_blocks_needed(total_), "used size not equal the needed size");

			if (const auto extra_bits = count_extra_bits(); extra_bits not_eq 0) { highest_block() and_eq (value_type{1} << extra_bits) - 1; }
		}

		/**
		 * @brief check class invariants
		 * @return is invariant
		 */
		[[nodiscard]] constexpr bool check_invariants() const
		noexcept(
			noexcept(calc_blocks_needed(std::declval<size_type>())))
		{
			if (const auto extra_bits = count_extra_bits(); extra_bits > 0)
			{
				if (const auto mask = static_cast<value_type>(compl 0) << extra_bits;
					(highest_block() bitand mask) not_eq 0) { return false; }
			}
			if (container_.size() > container_.capacity() or container_size() not_eq calc_blocks_needed(size())) { return false; }
			return true;
		}

	public:
		/**
		 * @brief default ctor
		 */
		constexpr basic_dynamic_bitset() noexcept(std::is_nothrow_default_constructible_v<container>) = default;

		/**
		 * @brief ctor from size, maybe need exchange the arg order of value and alloc
		 * @param size how many bits we want(will) to hold
		 * @param value the value to init
		 */
		GAL_ASSERT_CONSTEXPR explicit basic_dynamic_bitset(// NOLINT(clang-diagnostic-invalid-constexpr)
				const size_type size,
				const value_type value = {}) noexcept(noexcept(init_from_value_type(std::declval<size_type>(), std::declval<value_type>()))) { init_from_value_type(size, value); }

		/**
		 * @brief ctor from str[pos~pos+n]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Allocator allocator_type of str
		 * @param str str
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 */
		template<typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		constexpr basic_dynamic_bitset(
				const std::basic_string<Char, Traits, Allocator>bitand str,
				typename std::basic_string<Char, Traits, Allocator>::size_type pos,
				typename std::basic_string<Char, Traits, Allocator>::size_type n,
				size_type size = npos) noexcept(noexcept(init_from_basic_string(std::declval<decltype(str)>(),
				                                                                std::declval<decltype(pos)>(),
				                                                                std::declval<decltype(n)>(),
				                                                                std::declval<decltype(size)>())))
		{
			this->init_from_basic_string(
					str,
					pos,
					n,
					size);
		}

		/**
		 * @brief ctor from str[str~end]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Allocator allocator_type of str
		 * @param str str
		 * @param pos begin pos
		 */
		template<typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		constexpr explicit basic_dynamic_bitset(
				const std::basic_string<Char, Traits, Allocator>bitand str,
				typename std::basic_string<Char, Traits, Allocator>::size_type pos = {}) noexcept(std::is_nothrow_constructible_v<basic_dynamic_bitset, decltype(str), decltype(pos), decltype(std::basic_string<Char, Traits, Allocator>::npos), decltype(npos)>)
			: basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Allocator>::npos, npos) { }

		/**
		 * @brief ctor from str_view[pos~pos+n]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str str_view
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 */
		template<typename Char, typename Traits = std::char_traits<Char>>
		constexpr basic_dynamic_bitset(
				std::basic_string_view<Char, Traits> str,
				typename std::basic_string_view<Char, Traits>::size_type pos,
				typename std::basic_string_view<Char, Traits>::size_type n,
				size_type size = npos) noexcept(noexcept(std::declval<basic_dynamic_bitset>().init_from_basic_string_view(str, pos, n, size)))
		{
			this->init_from_basic_string_view(
					str,
					pos,
					n,
					size);
		}

		/**
		 * @brief ctor from str_view[str~end]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str str_view
		 * @param pos begin pos
		 */
		template<typename Char, typename Traits = std::char_traits<Char>>
		constexpr explicit basic_dynamic_bitset(
				std::basic_string_view<Char, Traits> str,
				typename std::basic_string_view<Char, Traits>::size_type pos = {}) noexcept(std::is_nothrow_constructible_v<basic_dynamic_bitset, decltype(str), decltype(pos), decltype(std::basic_string_view<Char, Traits>::npos), decltype(npos)>)
			: basic_dynamic_bitset(str, pos, std::basic_string_view<Char, Traits>::npos, npos) { }

		template<typename Char>
		constexpr explicit basic_dynamic_bitset(
				const Char* str,
				size_type pos,
				size_type n,
				size_type size = npos) noexcept(std::is_nothrow_constructible_v<basic_dynamic_bitset, decltype(std::basic_string_view<Char>{str}), decltype(pos), decltype(n), decltype(size)>)
			: basic_dynamic_bitset(std::basic_string_view<Char>{str}, pos, n, size) { }

		template<typename Char>
		constexpr explicit basic_dynamic_bitset(
				const Char* str,
				size_type pos) noexcept(std::is_nothrow_constructible_v<basic_dynamic_bitset, decltype(str), decltype(pos), decltype(npos), decltype(npos)>)
			: basic_dynamic_bitset(str, pos, npos, npos) { }

		/**
		 * @brief ctor from range (at least input_iterator)
		 * @tparam ContainerInputIterator iterator type of range
		 * @param first begin iterator
		 * @param last end iterator
		 */
		template<std::input_iterator ContainerInputIterator>
			requires requires(ContainerInputIterator it)
			         {
				         std::is_convertible_v<decltype(*it), value_type>;
			         } and
			         requires(container c)
			         {
				         c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
			         }
		constexpr basic_dynamic_bitset(
				ContainerInputIterator first,
				ContainerInputIterator last) noexcept(noexcept(std::declval<container>().insert(std::declval<container>().begin(), first, last)))
		{
			container_.insert(container_.begin(), first, last);
			total_ += container_.size() * bits_of_type;
		}

		/**
		 * @brief ctor from std::basic_istream
		 * @tparam Char type of input char
		 * @tparam Trait trait of input char
		 * @param is basic_istream
		 */
		template<typename Char, typename Trait = std::char_traits<Char>>
		constexpr explicit basic_dynamic_bitset(
				std::basic_istream<Char, Trait>bitand is) { is >> *this; }

		/**
		 * @brief ctor from initializer_list
		 * @param args args for init
		 */
		template<typename T>
			requires std::is_convertible_v<T, value_type>
		constexpr basic_dynamic_bitset(std::initializer_list<T> args) noexcept(
			std::is_nothrow_convertible_v<T, value_type> and noexcept(
				std::declval<container>().insert(
						std::declval<container>().begin(),
						std::declval<value_type>())) and noexcept(std::declval<container>().push_back(std::declval<value_type>())))
		{
			if constexpr (std::is_same_v<T, value_type>) { container_.insert(container_.begin(), args); }
			else { for (auto it = args.begin(); it not_eq args.end(); ++it) { container_.push_back(static_cast<value_type>(*it)); } }

			total_ += container_.size() * bits_of_type;
		}

		/**
		 * @brief dtor
		 */
		GAL_ASSERT_CONSTEXPR ~basic_dynamic_bitset() noexcept { gal_assert(check_invariants(), "dtor check_invariants failed"); }

		/**
		 * @brief copy ctor
		 * @param other another dynamic_bitset for copy
		 */
		constexpr basic_dynamic_bitset(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_constructible_v<container>) = default;

		/**
		 * @brief copy assign operator
		 * @param other another dynamic_bitset for copy
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator=(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_assignable_v<container>) = default;

		/**
		 * @brief move ctor
		 * @param other another dynamic_bitset for move
		 */
		GAL_ASSERT_CONSTEXPR			basic_dynamic_bitset(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_constructible_v<container>)
			: container_(std::move(other.container_)),
			  total_(other.total_)
		{
			// Required so that check_invariants() works.
			gal_assert((other.container_ = container{}).empty(), "moved container not empty");
			other.total_ = 0;
		}

		/**
		 * @brief move assign operator
		 * @param other another dynamic_bitset for move
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator=(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_assignable_v<container>)
		{
			if (std::addressof(other) == this) { return *this; }
			container_ = std::move(other.container_);
			total_ = other.total_;
			// Required so that check_invariants() works.
			gal_assert((other.container_ = container{}).empty(), "moved container not empty");
			other.total_ = 0;
			return *this;
		}

		/**
		 * @brief swap container and size
		 * @param other another dynamic_bitset
		 */
		constexpr void swap(basic_dynamic_bitset& other)// NOLINT(clang-diagnostic-invalid-constexpr)
		noexcept(std::is_nothrow_swappable_v<container>)
		{
			std::swap(container_, other.container_);
			std::swap(total_, other.total_);
		}

		/**
		 * @brief reserve enough memory for use (normally, size / bits_of_type, or container_real_size)
		 * @param size how many size we need reserve
		 */
		constexpr void reserve(const size_type size) noexcept(
			noexcept(std::declval<container>().reserve(std::declval<size_type>()))) { container_.reserve(calc_blocks_needed(size)); }

		/**
		 * @brief shrink to fit (normally, make container shrink to fit)
		 */
		constexpr void shrink_to_fit() noexcept(
			// std::is_nothrow_swappable_v<container>
			noexcept(std::declval<container>().shrink_to_fit()))
		{
			// if (container_.size() < container_.capacity())
			// {
			// 		container{container_}.swap(container_);
			// }
			container_.shrink_to_fit();
		}

		/**
		 * @brief reset blocks between pos and pos+len
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& reset(const size_type pos,
		                                      const size_type len) noexcept(
			noexcept(
				std::declval<basic_dynamic_bitset>().range_operation(
						std::declval<size_type>(),
						std::declval<size_type>(),
						&bit_trait::reset_block_partial,
						&bit_trait::reset_block_full))) { return range_operation(pos, len, bit_trait::reset_block_partial, bit_trait::reset_block_full); }

		/**
		 * @brief reset a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& reset(const size_type pos) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(pos < total_, "given pos greater than the total bits");

			container_[bit_trait::block_index(pos)] and_eq compl bit_trait::bit_mask(pos);
			return *this;
		}

		/**
		 * @brief reset all block's value
		 * @return self
		 */
		constexpr basic_dynamic_bitset& reset() noexcept(
			noexcept(std::declval<container>().begin()))
		{
			for (auto& it: container_) { it = static_cast<value_type>(0); }
			return *this;
		}

		/**
		 * @brief set blocks between pos and pos+len (dependent set or not)
		 * @param pos begin pos
		 * @param len length
		 * @param set set or reset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& set(const size_type pos, const size_type len, const bool set)
		{
			if (set) { return range_operation(pos, len, bit_trait::set_block_partial, bit_trait::set_block_full); }
			return range_operation(pos, len, bit_trait::reset_block_partial, bit_trait::reset_block_full);
		}

		/**
		 * @brief set a bit's value (dependent set or not)
		 * @param pos pos of bit
		 * @param set set or reset
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& set(const size_type pos,
		                                               const bool set = true) noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().reset(std::declval<size_type>())))
		{
			gal_assert(pos < total_, "given pos greater than the total bits");

			if (set) { container_[bit_trait::block_index(pos)] or_eq bit_trait::bit_mask(pos); }
			else { reset(pos); }
			return *this;
		}

		/**
		 * @brief set all block's value
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& set() noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits()))
		{
			for (auto& it: container_) { it = ~static_cast<value_type>(0); }
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief flip blocks between pos and pos+len (dependent value)
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& flip(const size_type pos,
		                                     const size_type len) noexcept(
			noexcept(
				std::declval<basic_dynamic_bitset>().range_operation(
						std::declval<size_type>(),
						std::declval<size_type>(),
						&bit_trait::flip_block_partial,
						&bit_trait::flip_block_full))) { return range_operation(pos, len, bit_trait::flip_block_partial, bit_trait::flip_block_full); }

		/**
		 * @brief flip a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& flip(const size_type pos) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(pos < total_, "given pos greater than the total bits");

			container_[bit_trait::block_index(pos)] xor_eq bit_trait::bit_mask(pos);
			return *this;
		}

		/**
		 * @brief flip all block's value
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& flip() noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits()))
		{
			for (size_type i = 0; i < container_size(); ++i) { container_[i] = compl container_[i]; }
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief test a bit only
		 * @param pos pos of bit
		 * @return result
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR bool test(const size_type pos) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().unchecked_test(std::declval<size_type>())))
		{
			gal_assert(pos < total_, "given pos greater than the total bits");
			return unchecked_test(pos);
		}

		/**
		 * @brief test a bit and if it not equal to value, set it
		 * @param pos pos of bit
		 * @param value expect value
		 * @return tested value
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR bool test_and_set(const size_type pos,
		                                          const bool value) noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().set(std::declval<size_type>(), std::declval<bool>())))
		{
			const auto b = test(pos);
			if (b not_eq value) { set(pos, value); }
			return b;
		}

		/**
		 * @brief is all bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool all() const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			if (empty()) { return true; }

			const auto extra = count_extra_bits();

			if (constexpr auto all_one = ~static_cast<value_type>(0);
				extra == 0) { for (size_type i = 0; i < container_size(); ++i) { if (container_[i] not_eq all_one) { return false; } } }
			else
			{
				for (size_type i = 0; i < container_size() - 1; ++i) { if (container_[i] not_eq all_one) { return false; } }
				if (highest_block() not_eq (value_type{1} << extra) - 1) { return false; }
			}
			return true;
		}

		/**
		 * @brief is any bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool any() const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			if (empty()) { return false; }
			for (size_type i = 0; i < container_size(); ++i) { if (container_[i]) { return true; } }
			return false;
		}

		/**
		 * @brief is none bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool none() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().any())) { return not any(); }

		/**
		 * @brief count how many bit been set
		 * @return result
		 */
		[[nodiscard]] constexpr size_type count() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()))
		{
			constexpr auto pop_count = [](value_type value) constexpr noexcept
			{
				static_assert(sizeof(std::byte) == sizeof(unsigned char));
				static_assert(sizeof(unsigned char) == 1);
				constexpr std::array<unsigned char, 256> count_table =
				{
						0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
						1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
						1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
						2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
						1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
						2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
						2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
						3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
				};
				size_type num{};
				while (value)
				{
					constexpr auto table_width = 8;
					num += count_table[value bitand ((value_type{1} << table_width) - 1)];
					value >>= table_width;
				}
				return num;
			};

			size_type total{};
			for (size_type i = 0; i < container_size(); ++i) { total += pop_count(container_[i]); }
			return total;
		}

		/**
		 * @brief get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard]] constexpr reference operator[](const size_type index) noexcept(std::is_nothrow_constructible_v<reference, real_reference, size_type>) { return {container_[bit_trait::block_index(index)], bit_trait::bit_index(index)}; }

		/**
		 * @brief get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard]] constexpr const_reference operator[](const size_type index) const
		noexcept(std::is_nothrow_constructible_v<reference, real_reference, size_type>) { return {container_[bit_trait::block_index(index)], bit_trait::bit_index(index)}; }

		/**
		 * @brief get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard]] constexpr iterator begin() noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>) { return {container_.begin(), 0}; }

		/**
		 * @brief get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard]] constexpr const_iterator begin() const
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>) { return {container_.begin(), 0}; }

		/**
		 * @brief get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard]] constexpr iterator end() noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>) { return {container_.begin(), total_}; }

		/**
		 * @brief get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard]] constexpr const_iterator end() const
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>) { return {container_.begin(), total_}; }

		/**
		 * @brief resize the container to required size
		 * @param size required size
		 * @param value value to set
		 */
		GAL_ASSERT_CONSTEXPR void resize(const size_type size, const bool value = false)
		{
			const auto old_size = container_size();
			const auto required_size = calc_blocks_needed(size);

			const auto v = value
				               ? compl value_type{0}
				               : value_type{0};

			if (required_size != old_size) { container_.resize(required_size, v); }

			// At this point:
			//
			//  - if the buffer was shrunk, we have nothing more to do,
			//    except a call to zero_unused_bits()
			//
			//  - if it was enlarged, all the (used) bits in the new blocks have
			//    the correct value, but we have not yet touched those bits, if
			//    any, that were 'unused bits' before enlarging: if value == true,
			//    they must be set.
			if (value and size > total_)
			{
				if (const auto extra = count_extra_bits(); extra)
				{
					gal_assert(old_size >= 1 && old_size <= container_.size(), "impossible happened: old_size < 1 or old_size > current size");

					// Set them.
					container_[old_size - 1] or_eq (v << extra);
				}
			}

			total_ = size;
			zero_unused_bits();
		}

		/**
		 * @brief clear container
		 */
		constexpr void clear() noexcept(noexcept(std::declval<container>().clear()))
		{
			container_.clear();
			total_ = 0;
		}

		/**
		 * @brief push a bit from back
		 * @param value bit's value
		 */
		GAL_ASSERT_CONSTEXPR void push_back(const bool value) noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().resize(std::declval<size_type>(), std::declval<bool>())))
		{
			const auto sz = size();
			resize(sz + 1, value);
		}

		/**
		 * @brief pop a bit from back
		 */
		GAL_ASSERT_CONSTEXPR void pop_back() noexcept(
			noexcept(zero_unused_bits()))
		{
			const auto old_size = container_size();

			if (const auto required_size = calc_blocks_needed(total_ - 1); required_size not_eq old_size) { container_.pop_back(); }
			--total_;
			zero_unused_bits();
		}

		/**
		 * @brief append a value (not a bit) from back (it means you will append bits_of_type bits from back)
		 * @param value value for append
		 */
		GAL_ASSERT_CONSTEXPR void append(const value_type value)
		{
			if (const auto extra = count_extra_bits(); extra == 0)
			{
				// the buffer is empty, or all blocks are filled
				container_.push_back(value);
			}
			else
			{
				container_.push_back(value >> (bits_of_type - extra));
				container_[container_.size() - 2] or_eq (value << extra);// container.size() >= 2
			}

			total_ += bits_of_type;
			gal_assert(check_invariants(), "check_invariants failed");
		}

		/**
		 * @brief and_eq with another dynamic_bitset, get all bits' intersection set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& operator and_eq(const basic_dynamic_bitset& other) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");
			for (size_type i = 0; i < container_size(); ++i) { container_[i] and_eq other.container_[i]; }
			return *this;
		}

		/**
		 * @brief or_eq with another dynamic_bitset, get all bits' union set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& operator or_eq(const basic_dynamic_bitset& other) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");
			for (size_type i = 0; i < container_size(); ++i) { container_[i] or_eq other.container_[i]; }
			return *this;
		}

		/**
		 * @brief xor_eq with another dynamic_bitset, get all bits' symmetric
		 * @param other another dynamic_bitset
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& operator xor_eq(const basic_dynamic_bitset& other) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");
			for (size_type i = 0; i < container_size(); ++i) { container_[i] xor_eq other.container_[i]; }
			return *this;
		}

		/**
		 * @brief -= with another dynamic_bitset, get all bits' subtraction set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		GAL_ASSERT_CONSTEXPR basic_dynamic_bitset& operator-=(const basic_dynamic_bitset& other) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");
			for (size_type i = 0; i < container_size(); ++i) { container_[i] and_eq compl other.container_[i]; }
			return *this;
		}

		/**
		 * @brief left shift self
		 * @param n offset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator<<=(const size_type n) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			if (n >= total_) { return reset(); }

			// note that we store every bit from back to front, it means left shift actually is right shift
			// from blocks_need_discard to total_blocks is the part we need save
			// or container[blocks_need_discard : total_blocks]
			auto total_blocks = container_size() - 1;
			auto blocks_need_discard = bit_trait::block_index(n);

			if (const auto remained_bits = bit_trait::bit_index(n); remained_bits == 0)
			{
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = 0; i <= total_blocks - blocks_need_discard; ++i) { container_[blocks_need_discard + i] = container_[i]; }
			}
			else
			{
				// more work we need to do
				/*
				 *      remained_bits
				 *  |***|
				 *  |***********************************|
				 *      |******************************|
				 *      extra_bits_in_block
				 *
				 * actually, we save extra_bits_in_block part (container[0] << remained_bits) in last block (container[blocks_need_discard])
				 * and remained_bits part (container[i] >> extra_bits_in_block) | extra_bits_in_block part (container[i - 1] << remained_bits) in other block
				 */

				const auto extra_bits_in_block = bits_of_type - remained_bits;

				for (decltype(blocks_need_discard) i = 1; i <= total_blocks - blocks_need_discard; ++i)
				{
					// store this block's remained_bits part and last (actually, is next block, but we store every bit from back to front) block's extra_bits_in_block part
					container_[blocks_need_discard + i] =
							(container_[i - 1] >> extra_bits_in_block) bitor
							(container_[i] << remained_bits);
				}
				container_[blocks_need_discard] = container_[0] << remained_bits;
			}

			// zero out front part
			for (decltype(total_blocks) i = 0; i < blocks_need_discard; ++i) { container_[i] = static_cast<value_type>(0); }

			// zero out any 1 bit that flowed into the unused part
			zero_unused_bits();

			return *this;
		}

		/**
		 * @brief right shift self
		 * @param n offset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator>>=(const size_type n) noexcept(
			noexcept(std::declval<container>()[0]))
		{
			if (n >= total_) { return reset(); }

			// note that we store every bit from back to front, it means right shift actually is left shift
			// from 0 to blocks_need_discard is the part we need save
			// or container[0 : blocks_need_discard]
			auto total_blocks = container_size() - 1;
			auto blocks_need_discard = bit_trait::block_index(n);

			if (const auto remained_bits = bit_trait::bit_index(n); remained_bits == 0)
			{
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = blocks_need_discard; i <= total_blocks; ++i) { container_[i - blocks_need_discard] = container_[i]; }
			}
			else
			{
				// more work we need to do
				/*
				 *      extra_bits_in_block
				 *  |***|
				 *  |***********************************|
				 *      |******************************|
				 *      remained_bits
				 *
				 * actually, we save extra_bits_in_block part (container[total_blocks] >> remained_bits) in first block (container[total_blocks - blocks_need_discard])
				 * and remained_bits part (container[i + 1] << extra_bits_in_block) | extra_bits_in_block part (container[i] >> remained_bits) in other block
				 */
				const auto extra_bits_in_block = bits_of_type - bit_trait::bit_index(n);

				for (decltype(blocks_need_discard) i = blocks_need_discard; i < total_blocks; ++i)
				{
					// store this block's remained_bits part and next (actually, is last block, but we store every bit from back to front) block's extra_bits_in_block part
					container_[i - blocks_need_discard] = (container_[i + 1] << extra_bits_in_block) | (container_[i] >>
					                                                                                    remained_bits);
				}
				container_[total_blocks - blocks_need_discard] = container_[total_blocks] >> remained_bits;
			}

			// zero out back part
			for (decltype(total_blocks) i = 0, begin = total_blocks - blocks_need_discard; i < blocks_need_discard; ++i) { container_[begin + i] = static_cast<value_type>(0); }

			return *this;
		}

		/**
		 * @brief left shift a copy
		 * @param n offset
		 * @return copy
		 */
		[[nodiscard]] basic_dynamic_bitset operator<<(const size_type n) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<<=(std::declval<size_type>())))
		{
			auto copy{*this};
			copy <<= n;
			return copy;
		}

		/**
		 * @brief right shift a copy
		 * @param n offset
		 * @return copy
		 */
		[[nodiscard]] basic_dynamic_bitset operator>>(const size_type n) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator>>=(std::declval<size_type>())))
		{
			auto copy{*this};
			copy >>= n;
			return copy;
		}

		/**
		 * @brief flip a copy all bit
		 * @return copy
		 */
		[[nodiscard]] basic_dynamic_bitset operator compl() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().flip()))
		{
			auto copy{*this};
			copy.flip();
			return copy;
		}

		/**
		 * @brief is the same dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator==(const basic_dynamic_bitset& other) const
		noexcept { return total_ == other.total_ and container_ == other.container_; }

		/**
		 * @brief is not the same dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator not_eq(const basic_dynamic_bitset& other) const noexcept { return not operator==(other); }

		/**
		 * @brief ss self less than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator<(const basic_dynamic_bitset& other) const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			if (other.empty()) { return false; }
			if (empty()) { return true; }
			if (auto sz = size(), sz_other = other.size(); sz == sz_other)
			{
				for (size_type i = container_size() - 1; i not_eq static_cast<size_type>(-1); --i)
				{
					if (value_type current = container_[i], current_other = other.container_[i];
						current not_eq current_other) { return current < current_other; }
				}
				return false;
			}
			else
			{
				const size_type intersect_size = std::min(sz, sz_other);

				for (auto a = sz - 1, b = sz_other - 1, count = static_cast<decltype(sz)>(0); count < intersect_size; ++count, --a, --b)
				{
					if (value_type current = container_[a], current_other = other.container_[b];
						current not_eq current_other) { return current < current_other; }
				}
				return sz < sz_other;
			}
		}

		/**
		 * @brief is self less_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator<=(const basic_dynamic_bitset& other) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().operator<(std::declval<const basic_dynamic_bitset&>()))) { return this->operator==(other) or this->operator<(other); }

		/**
		 * @brief is self greater than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator>(const basic_dynamic_bitset& other) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<=(std::declval<const basic_dynamic_bitset&>()))) { return not this->operator<=(other); }

		/**
		 * @brief is self greater_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator>=(const basic_dynamic_bitset& other) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<(std::declval<const basic_dynamic_bitset&>()))) { return not this->operator<(other); }

		/**
		 * @brief cast self to string
		 * @tparam Char char
		 * @tparam Trait trait
		 * @tparam Alloc allocator
		 * @param zero zero char
		 * @param one one char
		 * @return str
		*/
		template<typename Char = char, typename Trait = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
		[[nodiscard]] constexpr std::basic_string<Char, Trait, Alloc> to_string(
				Char zero = static_cast<Char>('0'),
				Char one = static_cast<Char>('1')) const
		{
			std::basic_string<Char, Trait, Alloc> str;

			str.reserve(size());
			for (size_type i = size() - 1; i not_eq static_cast<size_type>(-1); --i)
			{
				str.push_back(this->operator[](i)
					              ? one
					              : zero);
			}
			return str;
		}

		/**
		 * @brief cast self to another type value
		 * @tparam To dest type
		 * @return result
		 */
		template<std::integral To = value_type>
		// requires(sizeof(To) >= sizeof(value_type))
		[[nodiscard]] GAL_ASSERT_CONSTEXPR To cast_to() const
		noexcept(
			noexcept(bit_trait::block_index(std::declval<size_type>())))
		{
			if (empty()) { return {}; }

			const auto maximum_size = std::min(total_, static_cast<size_type>(std::numeric_limits<To>::digits));
			const auto last_block = bit_trait::block_index(maximum_size - 1);

			gal_assert((last_block * bits_of_type) < static_cast<size_type>(std::numeric_limits<To>::digits));

			To result{};
			for (size_type i = 0; i <= last_block; ++i)
			{
				auto offset = i * bits_of_type;
				result or_eq static_cast<To>(container_[i]) << offset;
			}
			return result;
		}

		/**
		 * @brief is self is a subset of other ? (all set bits of self also set in other, but `maybe` other has set more bits)
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR bool is_subset_of(const basic_dynamic_bitset& other) const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");

			for (size_type i = 0; i < container_size(); ++i) { if (container_[i] bitand compl other.container_[i]) { return false; } }
			return true;
		}

		/**
		 * @brief is self is a subset of other ? (all set bits of self also set in other, but other has set more bits)
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] GAL_ASSERT_CONSTEXPR bool is_proper_subset_of(const basic_dynamic_bitset& other) const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			gal_assert(size() == other.size(), "the two containers are not the same size");

			bool proper = false;
			for (size_type i = 0; i < container_size(); ++i)
			{
				const value_type a = container_[i];
				const value_type b = other.container_[i];

				if (a bitand compl b)
				{
					// not a subset at all
					return false;
				}
				if (b bitand compl a) { proper = true; }
			}
			return proper;
		}

		/**
		 * @brief is self has is_intersects in every block with other ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool is_intersects(const basic_dynamic_bitset& other) const
		noexcept(
			noexcept(std::declval<container>()[0]))
		{
			const auto intersect_size = std::min(container_size(), other.container_size());

			for (size_type i = 0; i < intersect_size; ++i) { if (container_[i] bitand other.container_[i]) { return true; } }
			return false;
		}

	private:
		container container_;
		size_type total_{0};
	};

	template<typename Char, typename Trait>
	std::basic_ostream<Char, Trait>& operator<<(
			std::basic_ostream<Char, Trait>& os,
			const basic_dynamic_bitset& bitset)
	{
		using size_type = basic_dynamic_bitset::size_type;

		std::underlying_type_t<decltype(std::ios_base::goodbit)> err = std::ios_base::goodbit;

		if (typename std::basic_ostream<Char, Trait>::sentry{os})
		{
			auto& fac = std::use_facet<std::ctype<Char>>(os.getloc());
			auto zero = fac.widen('0');
			auto one = fac.widen('1');

			auto sz = bitset.size();
			auto buf = os.rdbuf();
			auto width = (os.width() <= 0)
				             ? 0
				             : static_cast<size_type>(os.width());
			auto pad = (width <= sz)
				           ? 0
				           : width - sz;

			auto fill = os.fill();

			// if needed fill at left; pad is decreased along the way
			if (auto adjust_filed = os.flags() bitand std::ios_base::adjustfield;
				adjust_filed not_eq std::ios_base::left)
			{
				for (; pad > 0; --pad)
				{
					if (Trait::eq_int_type(Trait::eof(), buf->sputc(fill)))
					{
						err or_eq std::ios_base::failbit;
						break;
					}
				}
			}

			if (err == std::ios::goodbit)
			{
				// output the bitset
				for (size_type i = sz; i > 0; --i)
				{
					if (auto ret = buf->sputc(bitset.test(i - 1)
						                          ? one
						                          : zero);
						Trait::eq_int_type(Trait::eof(), ret))
					{
						err or_eq std::ios_base::failbit;
						break;
					}
				}
			}

			if (err == std::ios_base::goodbit)
			{
				// if needed fill at right
				for (; pad > 0; --pad)
				{
					if (Trait::eq_int_type(Trait::eof(), buf->sputc(fill)))
					{
						err or_eq std::ios_base::failbit;
						break;
					}
				}
			}

			os.width(0);
		}

		if (err not_eq std::ios_base::goodbit) { os.setstate(err); }
		return os;
	}

	template<typename Char, typename Trait>
	std::basic_istream<Char, Trait>& operator>>(
			std::basic_istream<Char, Trait>& is,
			basic_dynamic_bitset& bitset)
	{
		using size_type = basic_dynamic_bitset::size_type;

		auto width = is.width();
		auto limit = (width > 0 and static_cast<size_type>(width) < bitset.max_size())
			             ? static_cast<size_type>(width)
			             : bitset.max_size();

		std::underlying_type_t<decltype(std::ios_base::goodbit)> err = std::ios_base::goodbit;

		if (typename std::basic_istream<Char, Trait>::sentry{is})
		{
			auto& fac = std::use_facet<std::ctype<Char>>(is.getloc());
			auto zero = fac.widen('0');
			auto one = fac.widen('1');

			auto buf = is.rdbuf();
			auto i = buf->sgetc();
			while (bitset.size() < limit)
			{
				if (Trait::eq_int_type(Trait::eof(), i))
				{
					err or_eq std::ios_base::eofbit;
					break;
				}
				auto c = Trait::to_char_type(i);

				auto is_one = Trait::eq(c, one);

				// check valid
				if (not is_one and not Trait::eq(c, zero)) { break; }

				bitset.push_back(is_one);
				i = buf->snextc();
			}

			is.width(0);
		}

		if (bitset.empty()) { err or_eq std::ios_base::failbit; }
		if (err not_eq std::ios_base::goodbit) { is.setstate(err); }
		return is;
	}
}// namespace gal::toolbox::container

template<>
struct std::iterator_traits<gal::toolbox::container::basic_dynamic_bitset::bit_iterator>
		: gal::toolbox::container::basic_dynamic_bitset::bit_iterator { };

#endif
