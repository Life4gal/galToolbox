#pragma once

#include <array>
#include <iosfwd>
#include <locale>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>
#include <vector>

namespace gal::test
{
	template <std::integral T, typename Alloc = std::allocator<T>>
	class basic_dynamic_bitset;
	template <std::integral T, typename Alloc>
	class bit_ref;
	template <std::integral T, typename Alloc>
	class bit_it;


	template <std::integral T, typename Alloc>
	class bit_ref
	{
	public:
		friend class basic_dynamic_bitset<T, Alloc>;
		friend class bit_it<T, Alloc>;

		using container = basic_dynamic_bitset<T, Alloc>;
		using size_type = typename container::size_type;

		// bit_ref's value_type is real value_type
		using value_type = typename container::value_type;

		using reference = typename container::real_reference;
		using const_reference = typename container::real_const_reference;

		using self_reference = bit_ref<T, Alloc>bitand;
		using const_self_reference = const bit_ref<T, Alloc>bitand;
		using self_rreference = bit_ref<T, Alloc>and;

	private:
		constexpr bit_ref(reference value, size_type offset) noexcept
			: value_(value),
			mask_(value_type{1} << offset)
		{
			// todo: assert
		}

		// constexpr compl bit_ref() = default;

	public:
		// let IDE stop prompting us that 'Do not overload unary operator bitand, it is dangerous'
		auto operator bitand() = delete; /* NOLINT */

		// let IDE stop prompting us to add explicit for implicit conversion
		[[nodiscard]] constexpr operator bool() const noexcept /* NOLINT */
		{
			return (value_ bitand mask_) not_eq 0;
		}

		[[nodiscard]] constexpr bool operator compl() const noexcept
		{
			return not this->operator bool();
		}

		constexpr self_reference flip() noexcept
		{
			value_ xor_eq mask_;
			return *this;
		}

		constexpr self_reference operator=(const bool value) noexcept
		{
			if (value)
			{
				value_ or_eq mask_;
			}
			else
			{
				value_ and_eq compl mask_;
			}
			return *this;
		}

		// constexpr bit_ref(const_self_reference other)
		// 	: value_(other.value_), mask_(other.mask_) {}

		// bit_ref should be implicitly convertible to bool
		// constexpr self_reference operator=(const_self_reference other) noexcept
		// {
		// 	this->operator=(other.operator bool());
		// 	return *this;
		// }

		// move ctor is meaningless
		// bit_ref(self_rreference) = delete;
		// move assign is meaningless
		// self_reference operator=(self_rreference) = delete;

		constexpr self_reference operator or_eq(const bool value) noexcept
		{
			if (value)
			{
				value_ or_eq mask_;
			}
			return *this;
		}

		constexpr self_reference operator and_eq(const bool value) noexcept
		{
			if (not value)
			{
				value_ and_eq compl mask_;
			}
			return *this;
		}

		constexpr self_reference operator xor_eq(const bool value) noexcept
		{
			if (value)
			{
				value_ xor_eq mask_;
			}
			return *this;
		}

		constexpr self_reference operator-=(const bool value) noexcept
		{
			if (value)
			{
				value_ and_eq compl mask_;
			}
			return *this;
		}

	private:
		reference        value_;
		const value_type mask_;
	};

	template <std::integral T, typename Alloc>
	class bit_it
	{
	public:
		friend class basic_dynamic_bitset<T, Alloc>;

		using container = basic_dynamic_bitset<T, Alloc>;
		using size_type = typename container::size_type;

		// bit_it's value_type is bit_ref
		using value_type = bit_ref<T, Alloc>;

		using iterator = typename container::real_iterator;
		using const_iterator = typename container::real_const_iterator;
		using iterator_category = typename iterator::iterator_category;
		using difference_type = typename iterator::difference_type;

		using self_reference = bit_it<T, Alloc>bitand;
		using const_self_reference = const bit_it<T, Alloc>bitand;
		using self_rreference = bit_it<T, Alloc>and;

	private:
		constexpr bit_it(iterator it, size_type offset)
			: iterator_(it),
			index_(offset) {}

	public:
		[[nodiscard]] constexpr bit_it operator++(int) const noexcept
		{
			// GAL_ASSERT(index + 1 > index);
			auto copy{*this};
			++index_;
			return copy;
		}

		constexpr self_reference operator++() noexcept
		{
			// GAL_ASSERT(index + 1 > index);
			++index_;
			return *this;
		}

		[[nodiscard]] constexpr bit_it operator--(int) const noexcept
		{
			// GAL_ASSERT(index > 0);
			auto copy{*this};
			--index_;
			return copy;
		}

		constexpr self_reference operator--() noexcept
		{
			// GAL_ASSERT(index > 0);
			--index_;
			return *this;
		}

		[[nodiscard]] constexpr bit_it operator+(size_type n) const noexcept
		{
			// GAL_ASSERT(index + n > index);
			return {iterator_, index_ + n};
		}

		constexpr self_reference operator+=(size_type n) noexcept
		{
			// GAL_ASSERT(index + n > index);
			index_ += n;
			return *this;
		}

		[[nodiscard]] constexpr bit_it operator-(size_type n) const noexcept
		{
			// GAL_ASSERT(index >= n);
			return {iterator_, index_ - n};
		}

		constexpr self_reference operator-=(size_type n) noexcept
		{
			// GAL_ASSERT(index >= n);
			index_ -= n;
			return *this;
		}

		[[nodiscard]] constexpr size_type distance(const_self_reference other) const noexcept
		{
			// GAL_ASSERT(other.index <= index);
			return index_ - other.index_;
		}

		[[nodiscard]] constexpr bool operator==(const_self_reference other) const noexcept
		{
			return index_ == other.index_ and iterator_ == other.iterator_;
		}

		[[nodiscard]] constexpr bool operator not_eq(const_self_reference other) const noexcept
		{
			return not this->operator==(other);
		}

		[[nodiscard]] constexpr value_type operator*() const noexcept
		{
			return {*std::next(iterator_, container::block_index(index_)), container::bit_index(index_)};
		}

	private:
		iterator  iterator_;
		size_type index_;
	};

	template <std::integral T, typename Alloc>
	class basic_dynamic_bitset
	{
		friend class bit_ref<T, Alloc>;
		friend class bit_it<T, Alloc>;

	public:
		using container = std::vector<T, Alloc>;

		using value_type = typename container::value_type;
		using size_type = typename container::size_type;

		using allocator_type = typename container::allocator_type;
		using const_allocator_reference = const allocator_type bitand;
		using allocator_trait_type = std::allocator_traits<allocator_type>;

		using reference = bit_ref<T, Alloc>;
		using const_reference = const bit_ref<T, Alloc>bitand;
		using iterator = bit_it<T, Alloc>;
		using const_iterator = const bit_it<T, Alloc>bitand;

		using real_reference = typename container::reference;
		using real_const_reference = typename container::const_reference;
		using real_iterator = typename container::iterator;
		using real_const_iterator = typename container::const_iterator;

		using self_reference = basic_dynamic_bitset<T, Alloc>bitand;
		using const_self_reference = const basic_dynamic_bitset<T, Alloc>bitand;
		using self_rreference = basic_dynamic_bitset<T, Alloc>and;

		constexpr static size_type bits_of_type          = std::numeric_limits<value_type>::digits;
		constexpr static size_type bits_of_unsigned_long = std::numeric_limits<unsigned long>::digits;
		constexpr static size_type npos                  = static_cast<size_type>(-1);

		/**
		 * @brief default ctor
		 */
		constexpr basic_dynamic_bitset() noexcept(std::is_nothrow_default_constructible_v<container>)
			: container_(),
			total_(0) {}

		/**
		 * @brief ctor from alloc
		 * @param alloc the allocator of container
		 */
		constexpr explicit basic_dynamic_bitset(const_allocator_reference alloc)
		noexcept(std::is_nothrow_constructible_v<container, const_allocator_reference>)
			: container_(alloc),
			total_(0) {}

		/**
		 * @brief ctor from size, maybe need exchange the arg order of value and alloc
		 * @param size how many bits we want(will) to hold
		 * @param value the value to init
		 * @param alloc the allocator of container
		 */
		constexpr explicit basic_dynamic_bitset(
			size_type                 size,
			value_type                value = {},
			const_allocator_reference alloc = {})
		noexcept(
			std::is_nothrow_constructible_v<container, const_allocator_reference> and
			noexcept(std::declval<basic_dynamic_bitset>().init_from_value_type(size, value)))
			: container_(alloc),
			total_(0)
		{
			init_from_value_type(size, value);
		}

		/**
		 * @brief ctor from str[pos~pos+n]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Allocator allocator_type of str
		 * @param str str
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 * @param alloc the allocator of container
		 */
		template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		constexpr basic_dynamic_bitset(
			const std::basic_string<Char, Traits, Allocator>bitand         str,
			typename std::basic_string<Char, Traits, Allocator>::size_type pos,
			typename std::basic_string<Char, Traits, Allocator>::size_type n,
			size_type                                                      size  = npos,
			const_allocator_reference                                      alloc = {})
		noexcept(std::is_nothrow_constructible_v<container, const_allocator_reference> and
			noexcept(std::declval<basic_dynamic_bitset>().init_from_basic_string(str, pos, n, size)))
			: container_(alloc),
			total_(0)
		{
			init_from_basic_string(
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
		 * @param alloc the allocator of container
		 */
		template <typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		constexpr explicit basic_dynamic_bitset(
			const std::basic_string<Char, Traits, Allocator>bitand         str,
			typename std::basic_string<Char, Traits, Allocator>::size_type pos   = {},
			const_allocator_reference                                      alloc = {})
		noexcept(
			noexcept(basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Allocator>::npos, npos, alloc))
		)
			: basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Allocator>::npos, npos, alloc) {}

		/**
		 * @brief ctor from str_view[pos~pos+n]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str str_view
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 * @param alloc the allocator of container
		 */
		template <typename Char, typename Traits = std::char_traits<Char>>
		constexpr basic_dynamic_bitset(
			std::basic_string_view<Char, Traits>                     str,
			typename std::basic_string_view<Char, Traits>::size_type pos,
			typename std::basic_string_view<Char, Traits>::size_type n,
			size_type                                                size  = npos,
			const_allocator_reference                                alloc = {})
		noexcept(
			std::is_nothrow_constructible_v<container, const_allocator_reference> and
			noexcept(std::declval<basic_dynamic_bitset>().init_from_basic_string_view(str, pos, n, size))
		)
			: container_(alloc),
			total_(0)
		{
			init_from_basic_string_view(
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
		 * @param alloc the allocator of container
		 */
		template <typename Char, typename Traits = std::char_traits<Char>>
		constexpr explicit basic_dynamic_bitset(
			std::basic_string_view<Char, Traits>                     str,
			typename std::basic_string_view<Char, Traits>::size_type pos   = {},
			const_allocator_reference                                alloc = {}
			)
		noexcept(noexcept(basic_dynamic_bitset(str, pos, std::basic_string_view<Char, Traits>::npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, std::basic_string_view<Char, Traits>::npos, npos, alloc) {}

		template <typename Char>
		constexpr explicit basic_dynamic_bitset(
			const Char*               str,
			size_type                 pos,
			size_type                 n,
			size_type                 size  = npos,
			const_allocator_reference alloc = {}
			) noexcept(noexcept(basic_dynamic_bitset(std::basic_string_view<Char>{str}, pos, n, size, alloc)))
			: basic_dynamic_bitset(std::basic_string_view<Char>{str}, pos, n, size, alloc) {}

		template <typename Char>
		constexpr explicit basic_dynamic_bitset(
			const Char*               str,
			size_type                 pos,
			const_allocator_reference alloc = {})
		noexcept(noexcept(basic_dynamic_bitset(str, pos, npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, npos, npos, alloc) {}

		/**
		 * @brief ctor from range (at least input_iterator)
		 * @tparam ContainerInputIterator iterator type of range
		 * @param first begin iterator
		 * @param last end iterator
		 * @param alloc the allocator of container
		 */
		template <std::input_iterator ContainerInputIterator>
			requires requires(ContainerInputIterator it)
			{
				std::is_convertible_v<decltype(*it), value_type>;
			} and
			requires(container c)
			{
				c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
			}
		constexpr basic_dynamic_bitset(
			ContainerInputIterator    first,
			ContainerInputIterator    last,
			const_allocator_reference alloc = {})
		noexcept(
			std::is_nothrow_constructible_v<container, const_allocator_reference> and
			noexcept(std::declval<basic_dynamic_bitset>().container_.
														insert(std::declval<basic_dynamic_bitset>().container_.begin(),
																first,
																last))
		)
			: container_(alloc),
			total_(0)
		{
			container_.insert(container_.begin(), first, last);
			total_ += container_.size() * bits_of_type;
		}

		/**
		 * @brief ctor from std::basic_istream
		 * @tparam Char type of input char
		 * @tparam Trait trait of input char
		 * @param is basic_istream
		 * @param alloc the allocator of container
		 */
		template <typename Char, typename Trait = std::char_traits<Char>>
		explicit basic_dynamic_bitset(
			std::basic_istream<Char, Trait>bitand is,
			const_allocator_reference             alloc = {}
			)
			: container_(alloc),
			total_(0)
		{
			is >> *this;
		}

		/**
		 * @brief ctor from initializer_list
		 * @param args args for init
		 * @param alloc the allocator of container
		 */
		constexpr basic_dynamic_bitset(
			std::initializer_list<value_type>and args,
			const_allocator_reference            alloc = {})
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_.
														insert(std::declval<basic_dynamic_bitset>().container_.begin(),
																args.begin(),
																args.end())) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.size())
		)
			: container_(alloc),
			total_(0)
		{
			container_.insert(container_.begin(), std::forward<std::initializer_list<value_type>>(args));
			total_ += container_.size() * bits_of_type;
		}

		/**
		 * @brief dtor
		 */
		constexpr compl basic_dynamic_bitset()
		{
			// GAL_ASSERT(check_invariants());
		}

		/**
		 * @brief copy ctor
		 * @param other another dynamic_bitset for copy
		 */
		constexpr basic_dynamic_bitset(const_self_reference other)
		noexcept(std::is_nothrow_copy_constructible_v<container>)
			: container_(other.container_),
			total_(other.total_) {}

		/**
		 * @brief copy assign operator
		 * @param other another dynamic_bitset for copy
		 * @return self
		 */
		self_reference operator=(const_self_reference other)
		noexcept(std::is_nothrow_copy_assignable_v<container>)
		{
			container_ = other.container_;
			total_     = other.total_;
			return *this;
		}

		/**
		 * @brief move ctor
		 * @param other another dynamic_bitset for move
		 */
		basic_dynamic_bitset(self_rreference other)
		noexcept(std::is_nothrow_move_constructible_v<container>)
			: container_(std::move(other.container_)),
			total_(std::move(other.total_))
		{
			// Required so that check_invariants() works.
			// GAL_ASSERT((other.container_ = container{}).empty());// NOLINT
			other.total_ = 0;
		}

		/**
		 * @brief move assign operator
		 * @param other another dynamic_bitset for move
		 * @return self
		 */
		self_reference operator=(self_rreference other)
		noexcept(std::is_nothrow_move_assignable_v<container>)
		{
			if (std::addressof(other) == this)
			{
				return *this;
			}
			container_ = std::move(other.container);
			total_     = std::move(other.total_);
			// Required so that check_invariants() works.
			// GAL_ASSERT((other.container = container{}).empty());// NOLINT
			other.total_ = 0;
			return *this;
		}

		/**
		 * @brief swap container and size
		 * @param other another dynamic_bitset
		 */
		constexpr void swap(self_reference other)
		noexcept(std::is_nothrow_swappable_v<container>)
		{
			std::swap(container_, other.container_);
			std::swap(total_, other.total_);
		}

		/**
		 * @brief get container's allocator
		 * @return container's allocator
		 */
		[[nodiscard]] constexpr allocator_type get_allocator() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.get_allocator()))
		{
			return container_.get_allocator();
		}

		/**
		 * @brief get size (bits we hold, not container size)
		 * @return bits we hold
		 */
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return total_;
		}

		/**
		 * @brief get container's real size we used
		 * @return container's size
		 */
		[[nodiscard]] constexpr size_type container_size() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.size()))
		{
			return container_.size();
		}

		/**
		 * @brief get the max bits we can hold (normally, bits_of_type * max_size_of_container)
		 * @return max bits size
		 */
		[[nodiscard]] constexpr size_type max_size() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container.max_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().get_allocator()) and
			noexcept(allocator_trait_type::max_size(std::declval<basic_dynamic_bitset>().get_allocator()))
		)
		{
			auto container_max = container_.max_size();
			auto alloc_max     = allocator_trait_type::max_size(get_allocator());

			auto max = std::min(container_max, alloc_max);
			return max <= static_cast<size_type>(-1) / bits_of_type
						? max * bits_of_type
						: static_cast<size_type>(-1);
		}

		/**
		 * @brief are we hold zero bits?
		 * @return empty
		 */
		[[nodiscard]] constexpr bool empty() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().size()))
		{
			return size() == 0;
		}

		/**
		 * @brief get the capacity of bits we hold (normally, bits_of_type * capacity_of_container)
		 * @return what capacity is now
		 */
		[[nodiscard]] constexpr size_type capacity() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.capacity()))
		{
			return container_.capacity() * bits_of_type;
		}

		/**
		 * @brief reserve enough memory for use (normally, size / bits_of_type, or container_real_size)
		 * @param size how many size we need reserve
		 */
		constexpr void reserve(size_type size)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.reserve(size)))
		{
			container_.reserve(calc_blocks_needed(size));
		}

		/**
		 * @brief shrink to fit (normally, make container shrink to fit)
		 */
		constexpr void shrink_to_fit()
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_.size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.capacity()) and
			std::is_nothrow_swappable_v<container>
		)
		{
			if (container_.size() < container_.capacity())
			{
				container{container_}.swap(container_);
			}
		}

		/**
		 * @brief set blocks between pos and pos+len (dependent set or not)
		 * @param pos begin pos
		 * @param len length
		 * @param set set or reset
		 * @return self
		 */
		constexpr self_reference set(size_type pos, size_type len, const bool set)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos,
																		len,
																		bitand std::declval<basic_dynamic_bitset>().
																		set_block_partial,
																		bitand std::declval<basic_dynamic_bitset>().
																		set_block_full
																		)
			) and
			noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos,
																		len,
																		std::declval<basic_dynamic_bitset>().
																		reset_block_partial,
																		std::declval<basic_dynamic_bitset>().
																		reset_block_full))
		)
		{
			if (set)
			{
				return range_operation(pos, len, set_block_partial, set_block_full);
			}
			return range_operation(pos, len, reset_block_partial, reset_block_full);
		}

		/**
		 * @brief set a bit's value (dependent set or not)
		 * @param pos pos of bit
		 * @param set set or reset
		 * @return self
		 */
		constexpr self_reference set(size_type pos, const bool set = true) noexcept(
			noexcept(
				std::declval<basic_dynamic_bitset>().bit_mask(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_[0] = {}) and
			noexcept(std::declval<basic_dynamic_bitset>().reset(pos))
		)
		{
			// GAL_ASSERT(pos < total_size);

			if (set)
			{
				container_[block_index(pos)] or_eq bit_mask(pos);
			}
			else
			{
				reset(pos);
			}
			return *this;
		}

		/**
		 * @brief set all block's value
		 * @return self
		 */
		constexpr self_reference set()
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_.begin()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.end()) and
			noexcept(*std::declval<basic_dynamic_bitset>().container_.begin() = {}) and
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())
		)
		{
			for (auto it = container_.begin(); it != container_.end(); ++it)
			{
				*it = static_cast<value_type>(compl 0);
			}
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief reset blocks between pos and pos+len
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		constexpr self_reference reset(size_type pos, size_type len)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos,
																		len,
																		std::declval<basic_dynamic_bitset>().
																		reset_block_partial,
																		std::declval<basic_dynamic_bitset>().
																		reset_block_full)))
		{
			return range_operation(pos, len, reset_block_partial, reset_block_full);
		}

		/**
		 * @brief reset a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		constexpr self_reference reset(size_type pos)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_[0] = {})
		)
		{
			// GAL_ASSERT(pos < total_size);

			container_[block_index(pos)] and_eq compl bit_mask(pos);
			return *this;
		}

		/**
		 * @brief reset all block's value
		 * @return self
		 */
		constexpr self_reference reset()
		noexcept(
			noexcept(*std::declval<basic_dynamic_bitset>().container_.begin() = {})
		)
		{
			for (auto it = container_.begin(); it != container_.end(); ++it)
			{
				*it = static_cast<value_type>(0);
			}
			return *this;
		}

		/**
		 * @brief flip blocks between pos and pos+len (dependent value)
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		constexpr self_reference flip(size_type pos, size_type len)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos,
																		len,
																		std::declval<basic_dynamic_bitset>().
																		flip_block_partial,
																		std::declval<basic_dynamic_bitset>().
																		flip_block_full))
		)
		{
			return range_operation(pos, len, flip_block_partial, flip_block_full);
		}

		/**
		 * @brief flip a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		constexpr self_reference flip(size_type pos)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_[0] = {})
		)
		{
			//GAL_ASSERT(pos < total_size);

			container_[block_index(pos)] xor_eq bit_mask(pos);
			return *this;
		}

		/**
		 * @brief flip all block's value
		 * @return self
		 */
		constexpr self_reference flip()
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())
		)
		{
			for (size_type i = 0; i < container_size(); ++i)
			{
				container_[i] = compl container_[i];
			}
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief test a bit only
		 * @param pos pos of bit
		 * @return result
		 */
		[[nodiscard]] constexpr bool test(size_type pos) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().unchecked_test(pos)))
		{
			//GAL_ASSERT(pos < total_size);
			return unchecked_test(pos);
		}

		/**
		 * @brief test a bit and if it not equal to value, set it
		 * @param pos pos of bit
		 * @param value expect value
		 * @return tested value
		 */
		[[nodiscard]] constexpr bool test_and_set(size_type pos, bool value)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().test(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().set(pos, value))
		)
		{
			auto b = test(pos);
			if (b not_eq value)
			{
				set(pos, value);
			}
			return b;
		}

		/**
		 * @brief is all bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool all() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().empty()) and
			noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			if (empty())
			{
				return true;
			}

			auto extra = count_extra_bits();

			if (auto all_one = static_cast<value_type>(compl 0); extra == 0)
			{
				for (size_type i = 0; i < container_size(); ++i)
				{
					if (container_[i] not_eq all_one)
					{
						return false;
					}
				}
			}
			else
			{
				for (size_type i = 0; i < container_size() - 1; ++i)
				{
					if (container_[i] not_eq all_one)
					{
						return false;
					}
				}
				if (highest_block() not_eq (value_type{1} << extra) - 1)
				{
					return false;
				}
			}
			return true;
		}

		/**
		 * @brief is any bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool any() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0])
		)
		{
			if (empty())
			{
				return false;
			}
			for (size_type i = 0; i < container_size(); ++i)
			{
				if (container_[i])
				{
					return true;
				}
			}
			return false;
		}

		/**
		 * @brief is none bit been set ?
		 * @return result
		 */
		[[nodiscard]] constexpr bool none() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().any()))
		{
			return not any();
		}

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
			for (size_type i = 0; i < container_size(); ++i)
			{
				total += pop_count(container_[i]);
			}
			return total;
		}

		/**
		 * @brief get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard]] constexpr reference operator[](size_type index)
		noexcept(std::is_nothrow_constructible_v<reference, real_reference, size_type>)
		{
			return {container_[block_index(index)], bit_index(index)};
		}

		/**
		 * @brief get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard]] constexpr reference operator[](size_type index) const
		noexcept(std::is_nothrow_constructible_v<reference, real_reference, size_type>)
		{
			return {container_[block_index(index)], bit_index(index)};
		}

		/**
		 * @brief get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard]] constexpr iterator begin()
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>)
		{
			return {container_.begin(), 0};
		}

		/**
		 * @brief get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard]] constexpr const_iterator begin() const
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>)
		{
			return {container_.begin(), 0};
		}

		/**
		 * @brief get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard]] constexpr iterator end()
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>)
		{
			return {container_.begin(), total_};
		}

		/**
		 * @brief get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard]] constexpr const_iterator end() const
		noexcept(std::is_nothrow_constructible_v<iterator, real_iterator, size_type>)
		{
			return {container_.begin(), total_};
		}

		/**
		 * @brief resize the container to required size
		 * @param size required size
		 * @param value value to set
		 */
		constexpr void resize(size_type size, bool value = false)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.resize(size_type{}, value_type{})) and
			noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) and
			noexcept(std::declval<basic_dynamic_bitset>().container.size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0]) and
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())
		)
		{
			auto old_size      = container_size();
			auto required_size = calc_blocks_needed(size);

			auto v = value
						? compl value_type{0}
						: value_type{0};

			if (required_size != old_size)
			{
				container_.resize(required_size, v);
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
			if (value and (size > total_))
			{
				if (auto extra = count_extra_bits(); extra)
				{
					// GAL_ASSERT(old_size >= 1 && old_size <= container.size());

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
		constexpr void clear()
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.clear()))
		{
			container_.clear();
			total_ = 0;
		}

		/**
		 * @brief push a bit from back
		 * @param value bit's value
		 */
		constexpr void push_back(bool value)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().size()) and
			noexcept(std::declval<basic_dynamic_bitset>().resize(0)) and
			noexcept(std::declval<basic_dynamic_bitset>().size(), value)
		)
		{
			auto sz = size();
			resize(sz + 1, value);
		}

		/**
		 * @brief pop a bit from back
		 */
		constexpr void pop_back()
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().
				calc_blocks_needed(std::declval<basic_dynamic_bitset>().total_ - 1)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.pop_back()) and
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())
		)
		{
			auto old_size = container_size();

			if (auto required_size = calc_blocks_needed(total_ - 1); required_size not_eq old_size)
			{
				container_.pop_back();
			}
			--total_;
			zero_unused_bits();
		}

		/**
		 * @brief append a value (not a bit) from back (it means you will append bits_of_type bits from back)
		 * @param value value for append
		 */
		constexpr void append(value_type value)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.push_back(value)) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			if (auto extra = count_extra_bits(); extra == 0)
			{
				// the buffer is empty, or all blocks are filled
				container_.push_back(value);
			}
			else
			{
				container_.push_back(value >> (bits_of_type - extra));
				container_[container_.size() - 2] or_eq (value << extra); // container.size() >= 2
			}

			total_ += bits_of_type;
			//GAL_ASSERT(check_invariants());
		}

		/**
		 * @brief and_eq with another dynamic_bitset, get all bits' intersection set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr self_reference operator and_eq(const_self_reference other)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			// GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i)
			{
				container_[i] &= other.container_[i];
			}
			return *this;
		}

		/**
		 * @brief or_eq with another dynamic_bitset, get all bits' union set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr self_reference operator or_eq(const_self_reference other)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			// GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i)
			{
				container_[i] |= other.container_[i];
			}
			return *this;
		}

		/**
		 * @brief xor_eq with another dynamic_bitset, get all bits' symmetric
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr self_reference operator^=(const_self_reference other)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			//GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i)
			{
				container_[i] ^= other.container_[i];
			}
			return *this;
		}

		/**
		 * @brief -= with another dynamic_bitset, get all bits' subtraction set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr self_reference operator-=(const_self_reference other)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})
		)
		{
			// GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i)
			{
				container_[i] &= ~other.container_[i];
			}
			return *this;
		}

		/**
		 * @brief left shift self
		 * @param n offset
		 * @return self
		 */
		constexpr self_reference operator<<=(size_type n)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().reset()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(n)) and
			noexcept(std::declval<basic_dynamic_bitset>().bit_index(n)) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {}) and
			noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())
		)
		{
			if (n >= total_)
			{
				return reset();
			}

			// note that we store every bit from back to front, it means left shift actually is right shift
			// from blocks_need_discard to total_blocks is the part we need save
			// or container[blocks_need_discard : total_blocks]
			auto total_blocks        = container_size() - 1;
			auto blocks_need_discard = block_index(n);

			if (auto remained_bits = bit_index(n); remained_bits == 0)
			{
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = 0; i <= total_blocks - blocks_need_discard; ++i)
				{
					container_[blocks_need_discard + i] = container_[i];
				}
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

				auto extra_bits_in_block = bits_of_type - remained_bits;

				for (decltype(blocks_need_discard) i = 1; i <= total_blocks - blocks_need_discard; ++i)
				{
					// store this block's remained_bits part and last (actually, is next block, but we store every bit from back to front) block's extra_bits_in_block part
					container_[blocks_need_discard + i] = (container_[i - 1] >> extra_bits_in_block) | (container_[i] <<
						remained_bits);
				}
				container_[blocks_need_discard] = container_[0] << remained_bits;
			}

			// zero out front part
			for (decltype(total_blocks) i = 0; i < blocks_need_discard; ++i)
			{
				container_[i] = static_cast<value_type>(0);
			}

			// zero out any 1 bit that flowed into the unused part
			zero_unused_bits();

			return *this;
		}

		/**
		 * @brief right shift self
		 * @param n offset
		 * @return self
		 */
		constexpr self_reference operator>>=(size_type n)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<<=(n)))
		{
			if (n >= total_)
			{
				return reset();
			}

			// note that we store every bit from back to front, it means right shift actually is left shift
			// from 0 to blocks_need_discard is the part we need save
			// or container[0 : blocks_need_discard]
			auto total_blocks        = container_size() - 1;
			auto blocks_need_discard = block_index(n);

			if (auto remained_bits = bit_index(n); remained_bits == 0)
			{
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = blocks_need_discard; i <= total_blocks; ++i)
				{
					container_[i - blocks_need_discard] = container_[i];
				}
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
				auto extra_bits_in_block = bits_of_type - bit_index(n);

				for (decltype(blocks_need_discard) i = blocks_need_discard; i < total_blocks; ++i)
				{
					// store this block's remained_bits part and next (actually, is last block, but we store every bit from back to front) block's extra_bits_in_block part
					container_[i - blocks_need_discard] = (container_[i + 1] << extra_bits_in_block) | (container_[i] >>
						remained_bits);
				}
				container_[total_blocks - blocks_need_discard] = container_[total_blocks] >> remained_bits;
			}

			// zero out back part
			for (decltype(total_blocks) i = 0, begin = total_blocks - blocks_need_discard; i < blocks_need_discard; ++i)
			{
				container_[begin + i] = static_cast<value_type>(0);
			}

			return *this;
		}

		/**
		 * @brief left shift a copy
		 * @param n offset
		 * @return copy
		 */
		[[nodiscard]] constexpr basic_dynamic_bitset operator<<(size_type n) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<<=(n)))
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
		[[nodiscard]] constexpr basic_dynamic_bitset operator>>(size_type n) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator>>=(n)))
		{
			auto copy{*this};
			copy >>= n;
			return copy;
		}

		/**
		 * @brief flip a copy all bit
		 * @return copy
		 */
		[[nodiscard]] constexpr basic_dynamic_bitset operator compl() const
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
		[[nodiscard]] constexpr bool operator==(const_self_reference other) const
		noexcept
		{
			return total_ == other.total_ and container_ == other.container_;
		}

		/**
		 * @brief is not the same dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator!=(const_self_reference other) const noexcept
		{
			return not operator==(other);
		}

		/**
		 * @brief ss self less than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator<(const_self_reference other) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().empty()) and
			noexcept(std::declval<basic_dynamic_bitset>().size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0])
		)
		{
			if (other.empty())
			{
				return false;
			}
			if (empty())
			{
				return true;
			}
			if (auto sz = size(), sz_other = other.size(); sz == sz_other)
			{
				for (size_type i = container_size() - 1; i >= 0; --i)
				{
					if (value_type current = container_[i], current_other = other.container_[i];
						current not_eq current_other)
					{
						return current < current_other;
					}
				}
				return false;
			}
			else
			{
				size_type intersect_size = std::min(sz, sz_other);

				for (auto a = sz - 1, b = sz_other - 1, count = 0; count < intersect_size; ++count, --a, --b)
				{
					if (value_type current = container_[a], current_other = other.container_[b];
						current not_eq current_other)
					{
						return current < current_other;
					}
				}
				return sz < sz_other;
			}
		}

		/**
		 * @brief is self less_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator<=(const_self_reference other) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().operator==(other)) and
			noexcept(std::declval<basic_dynamic_bitset>().operator<(other))
		)
		{
			return this->operator==(other) or this->operator<(other);
		}

		/**
		 * @brief is self greater than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator>(const_self_reference other) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<=(other)))
		{
			return not this->operator<=(other);
		}

		/**
		 * @brief is self greater_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool operator>=(const_self_reference other) const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<(other)))
		{
			return not this->operator<(other);
		}

		/**
		 * @brief cast self to another type value
		 * @tparam To dest type
		 * @return result
		 */
		template <typename To = unsigned long>
			requires(sizeof(To) >= sizeof(T))
		[[nodiscard]] constexpr To cast_to() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().empty()) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(0))
		)
		{
			if (empty())
			{
				return {};
			}

			auto maximum_size = std::min(total_, static_cast<size_type>(std::numeric_limits<To>::digits));
			auto last_block   = block_index(maximum_size - 1);

			//GAL_ASSERT((last_block * bits_of_type) < static_cast<size_type>(detail::bits_of_type<To>()));

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
		[[nodiscard]] constexpr bool is_subset_of(const_self_reference other) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0])
		)
		{
			//GAL_ASSERT(size() == other.size());

			for (size_type i = 0; i < container_size(); ++i)
			{
				if (container_[i] bitand compl other.container_[i])
				{
					return false;
				}
			}
			return true;
		}

		/**
		 * @brief is self is a subset of other ? (all set bits of self also set in other, but other has set more bits)
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard]] constexpr bool is_proper_subset_of(const_self_reference other) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0])
		)
		{
			//GAL_ASSERT(size() == other.size());
			//GAL_ASSERT(container_size() == other.container_size());

			bool proper = false;
			for (size_type i = 0; i < container_size(); ++i)
			{
				value_type a = container_[i];
				value_type b = other.container_[i];

				if (a bitand compl b)
				{
					// not a subset at all
					return false;
				}
				if (b bitand compl a)
				{
					proper = true;
				}
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
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0])
		)
		{
			auto intersect_size = std::min(container_size(), other.container_size());

			for (size_type i = 0; i < intersect_size; ++i)
			{
				if (container_[i] & other.container_[i])
				{
					return true;
				}
			}
			return false;
		}

	private:
		container container_;
		size_type total_;

		/**
		 * @brief get the block where the current position is
		 * @param pos the pos of bit is
		 * @return index
		 */
		[[nodiscard]] constexpr static size_type block_index(size_type pos) noexcept
		{
			return pos / bits_of_type;
		}

		/**
		 * @brief get the bit of the current position (in the block where the current position is)
		 * @param pos the pos of the bit is
		 * @return index
		 */
		[[nodiscard]] constexpr static size_type bit_index(size_type pos) noexcept
		{
			return pos % bits_of_type;
		}

		/**
		 * @brief get the bit mask of the current position (in the block where the current position is)
		 * @param pos the pos of bit is
		 * @return mask
		 */
		[[nodiscard]] constexpr static value_type bit_mask(size_type pos)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_index(pos)))
		{
			return value_type{1} << bit_index(pos);
		}

		/**
		 * @brief get the bit mask from first to last
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return mask
		 */
		[[nodiscard]] constexpr static value_type bit_mask(size_type first, size_type last) noexcept
		{
			//GAL_ASSERT(first <= last);
			//GAL_ASSERT(last < bits_of_type);
			auto res = (last == bits_of_type - 1)
							? static_cast<value_type>(compl 0)
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
		[[nodiscard]] constexpr static value_type set_block_bits(value_type block, size_type first, size_type last,
																const bool  set)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_mask(first, last)))
		{
			if (set)
			{
				return block bitor bit_mask(first, last);
			}
			return block bitand static_cast<value_type>(compl bit_mask(first, last));
		}

		/**
		 * @brief set the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return block after set
		 */
		[[nodiscard]] constexpr static value_type set_block_partial(value_type block, size_type first, size_type last)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().set_block_bits(block, first, last, true)))
		{
			return set_block_bits(block, first, last, true);
		}

		/**
		 * @brief set the block's value to full (wont change itself)
		 * @return compl 0
		 */
		[[nodiscard]] constexpr static value_type set_block_full(value_type) noexcept
		{
			return static_cast<value_type>(compl 0);
		}

		/**
		 * @brief reset the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last ebd for get mask
		 * @return block after reset
		 */
		[[nodiscard]] constexpr static value_type reset_block_partial(value_type block, size_type first, size_type last)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().set_block_bits(block, first, last, false)))
		{
			return set_block_bits(block, first, last, false);
		}

		/**
		 * @brief reset the block's value to full (wont change itself)
		 * @return 0
		 */
		[[nodiscard]] constexpr static value_type reset_block_full(value_type) noexcept
		{
			return 0;
		}

		/**
		 * @brief flip the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return block after flip
		 */
		[[nodiscard]] constexpr static value_type flip_block_partial(value_type block, size_type first, size_type last)
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_mask(first, last)))
		{
			return block xor bit_mask(first, last);
		}

		/**
		 * @brief flip the block's value to full (wont change itself)
		 * @param block current block
		 * @return block after flip
		 */
		[[nodiscard]] constexpr static value_type flip_block_full(value_type block) noexcept
		{
			return compl block;
		}

		/**
		 * @brief just check a bit
		 * @param pos bit's pos
		 * @return it this bit set
		 */
		[[nodiscard]] constexpr bool unchecked_test(size_type pos) const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_[0])
		)
		{
			return (container_[block_index(pos)] bitand bit_mask(pos)) not_eq 0;
		}

		/**
		 * @brief calc how many blocks we need to store num_bits bits
		 * @param num_bits how many bits we want to hold
		 * @return blocks we need
		 */
		[[nodiscard]] constexpr size_type calc_blocks_needed(size_type num_bits) const noexcept
		{
			return num_bits / bits_of_type + static_cast<size_type>(num_bits % bits_of_type not_eq 0);
		}

		/**
		 * @brief get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard]] constexpr real_reference highest_block()
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.back()))
		{
			//GAL_ASSERT(size() > 0 && container_size() > 0);
			return container_.back();
		}

		/**
		 * @brief get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard]] constexpr real_const_reference highest_block() const
		noexcept(noexcept(std::declval<basic_dynamic_bitset>().container_.back()))
		{
			GAL_ASSERT(size() > 0 && container_size() > 0);
			return container_.back();
		}

		/**
		 * @brief init a dynamic_bitset from value_type
		 * @param size how many bits we want to hold
		 * @param value the value for init
		 */
		constexpr void init_from_value_type(size_type size, value_type value)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.resize(calc_blocks_needed(size)))
		)
		{
			//GAL_ASSERT(container_size() == 0);

			container_.resize(calc_blocks_needed(size));
			total_ = size;

			// zero out all bits at pos >= num_bits, if any;
			// note that: num_bits == 0 implies value == 0
			if (size < static_cast<size_type>(bits_of_unsigned_long))
			{
				value and_eq ((value_type{1} << size) - 1);
			}

			constexpr auto left_shifter = [](real_reference v) constexpr-> void
			{
				if (bits_of_type >= bits_of_unsigned_long)
				{
					v = 0;
				}
				else
				{
					v = static_cast<value_type>(static_cast<decltype(bits_of_unsigned_long)>(v) >> bits_of_type);
				}
			};

			for (auto it = container_.begin();; ++it)
			{
				*it = static_cast<value_type>(value);
				left_shifter(value);

				if (value == 0)
					break;
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
		template <typename Char, typename Traits, typename Allocator>
		constexpr void init_from_basic_string(
			const std::basic_string<Char, Traits, Allocator>bitand         str,
			typename std::basic_string<Char, Traits, Allocator>::size_type pos,
			typename std::basic_string<Char, Traits, Allocator>::size_type n,
			size_type                                                      size)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) and
			noexcept(std::declval<basic_dynamic_bitset>().container.resize(size))
		)
		{
			//GAL_ASSERT(pos <= str.size());
			//GAL_ASSERT(n <= size);

			// make sure the end pos is valid
			auto len      = std::min(n, str.size() - pos);
			auto num_bits = size not_eq npos
								? size
								: len;
			container_.resize(calc_blocks_needed(num_bits));
			total_ = num_bits;

			auto bitand fac  = std::use_facet<std::ctype<Char>>(std::locale());
			auto       one  = fac.widen('1');
			auto       zero = fac.widen('0');

			auto shorter = std::min(num_bits, len);
			using str_size_type = typename std::basic_string<Char, Traits, Allocator>::size_type;
			using str_trait_type = typename std::basic_string<Char, Traits, Allocator>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i)
			{
				auto c = str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (not is_one and not str_trait_type::eq(c, zero))
				{
					break;
				}

				if (is_one)
				{
					set(i);
				}
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
		template <typename Char, typename Traits>
		constexpr void init_from_basic_string_view(
			const std::basic_string_view<Char, Traits>               str,
			typename std::basic_string_view<Char, Traits>::size_type pos,
			typename std::basic_string_view<Char, Traits>::size_type n,
			size_type                                                size)
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.resize(size))
		)
		{
			//GAL_ASSERT(pos <= str.size());
			//GAL_ASSERT(n <= size);

			// make sure the end pos is valid
			auto len      = std::min(n, str.size() - pos);
			auto num_bits = size != npos
								? size
								: len;
			container_.resize(calc_blocks_needed(num_bits));
			total_ = num_bits;

			auto bitand fac  = std::use_facet<std::ctype<Char>>(std::locale());
			auto       one  = fac.widen('1');
			auto       zero = fac.widen('0');

			auto shorter = std::min(num_bits, len);
			using str_size_type = typename std::basic_string_view<Char, Traits>::size_type;
			using str_trait_type = typename std::basic_string_view<Char, Traits>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i)
			{
				auto c = str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (not is_one and not str_trait_type::eq(c, zero))
				{
					break;
				}

				if (is_one)
				{
					set(i);
				}
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
		constexpr self_reference range_operation(
			size_type    pos,
			size_type    len,
			value_type (*partial_block_operation)(value_type, size_type, size_type),
			value_type (*full_block_operation)(value_type))
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().block_index(0)) and
			noexcept(std::declval<basic_dynamic_bitset>().bit_index(0)) and
			noexcept(std::declval<basic_dynamic_bitset>().container[0] = {}) and
			noexcept(partial_block_operation(std::declval<basic_dynamic_bitset>().container[0], 0, 0)) and
			noexcept(full_block_operation(std::declval<basic_dynamic_bitset>().container[0]))
		)
		{
			//GAL_ASSERT(pos + len <= total_size);

			// do nothing in case of zero length
			if (len == 0)
			{
				return *this;
			}

			// use an additional asserts in order to detect size_type overflow
			// for example: pos = 10, len = size_type_limit - 2, pos + len = 7
			// in case of overflow, 'pos + len' is always smaller than 'len'
			//GAL_ASSERT(pos + len >= len);

			// start and end blocks of the [pos : pos + len - 1] sequence
			auto first_block = block_index(pos);
			auto last_block  = block_index(pos + len - 1);
			auto first_bit   = bit_index(pos);

			if (auto last_bit = bit_index(pos + len - 1); first_block == last_block)
			{
				// filling only a sub-block of a block
				container_[first_block] = partial_block_operation(container_[first_block], first_bit, last_bit);
			}
			else
			{
				// check if the corner blocks won't be fully filled with 'value'
				decltype(first_bit) first_block_shift = (first_bit == 0)
															? 0
															: 1;
				decltype(last_bit) last_block_shift = (last_bit == bits_of_type - 1)
														? 0
														: 1;

				// middle
				// blocks that will be filled with compl 0 or 0 at once
				decltype(first_block) first_full_block = first_block + first_block_shift;
				decltype(last_block)  last_full_block  = last_block - last_block_shift;

				for (auto i = first_full_block; i <= last_full_block; ++i)
				{
					container_[i] = full_block_operation(container_[i]);
				}

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
				if (last_block_shift)
				{
					container_[last_block] = partial_block_operation(container_[last_block], 0, last_bit);
				}
			}

			return *this;
		}

		/**
		 * @brief if size() is not a multiple of bits_per_block then not all the bits in the last block are used.
		 * this function resets the unused bits (convenient for the implementation of many member functions)
		 */
		constexpr void zero_unused_bits()
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) and
			noexcept(std::declval<basic_dynamic_bitset>().highest_block())
		)
		{
			//GAL_ASSERT(container_size() == calc_blocks_needed(total_));

			if (auto extra_bits = count_extra_bits(); extra_bits not_eq 0)
			{
				highest_block() and_eq (value_type{1} << extra_bits) - 1;
			}
		}

		/**
		 * @brief check class invariants
		 * @return is invariant
		 */
		[[nodiscard]] constexpr bool check_invariants() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) and
			noexcept(std::declval<basic_dynamic_bitset>().highest_block()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.size()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_.capacity()) and
			noexcept(std::declval<basic_dynamic_bitset>().container_size()) and
			noexcept(std::declval<basic_dynamic_bitset>().size()) and
			noexcept(std::declval<basic_dynamic_bitset>().
				calc_blocks_needed(std::declval<basic_dynamic_bitset>().size()))
		)
		{
			if (auto extra_bits = count_extra_bits(); extra_bits > 0)
			{
				if (auto mask = static_cast<value_type>(compl 0) << extra_bits;
					(highest_block() bitand mask) not_eq 0)
				{
					return false;
				}
			}
			if (container_.size() > container_.capacity() or container_size() not_eq calc_blocks_needed(size()))
			{
				return false;
			}
			return true;
		}

		/**
		 * @brief count extra bits (in last block)
		 * @return extra bits
		 */
		[[nodiscard]] constexpr size_type count_extra_bits() const
		noexcept(
			noexcept(std::declval<basic_dynamic_bitset>().size()) and
			noexcept(std::declval<basic_dynamic_bitset>().bit_index(std::declval<basic_dynamic_bitset>().size()))
		)
		{
			return bit_index(size());
		}
	};

	/**
	  @brief deduction guide
	  todo:
		`2`, `3` make `10` not work
		`2` make `11` not work
		`12` make `2` not work
	 */

	// 1
	basic_dynamic_bitset() -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 2
	template <std::integral T, template<typename> typename Allocator>
	basic_dynamic_bitset(const Allocator<T>bitand) -> basic_dynamic_bitset<std::make_unsigned_t<T>, Allocator<T>>;

	// 3
	template <std::integral T>
	basic_dynamic_bitset(
		typename std::vector<std::make_unsigned_t<T>, std::allocator<std::make_unsigned_t<T>>>::size_type,
		T,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<
		std::make_unsigned_t<T>, std::allocator<T>>;

	// 4
	template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
	basic_dynamic_bitset(
		const std::basic_string<Char, Traits, Alloc>bitand                         ,
		typename std::basic_string<Char, Traits, Alloc>::size_type                 ,
		typename std::basic_string<Char, Traits, Alloc>::size_type                 ,
		typename basic_dynamic_bitset<std::size_t, std::allocator<Char>>::size_type = basic_dynamic_bitset<
			std::size_t, std::allocator<std::size_t>>::npos,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 5
	template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
	basic_dynamic_bitset(
		const std::basic_string<Char, Traits, Alloc>bitand,
		typename std::basic_string<Char, Traits, Alloc>::size_type = {},
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 6
	template <typename Char, typename Traits = std::char_traits<Char>>
	basic_dynamic_bitset(
		std::basic_string_view<Char, Traits>                                     ,
		typename std::basic_string_view<Char, Traits>::size_type                 ,
		typename std::basic_string_view<Char, Traits>::size_type                 ,
		basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>::size_type = basic_dynamic_bitset<
			std::size_t, std::allocator<std::size_t>>::npos,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 7
	template <typename Char, typename Trait = std::char_traits<Char>>
	basic_dynamic_bitset(
		std::basic_string_view<Char, Trait>,
		typename std::basic_string_view<Char, Trait>::size_type = {},
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 8
	template <typename Char>
	basic_dynamic_bitset(
		const Char*                                                              ,
		basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>::size_type,
		basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>::size_type,
		basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>::size_type = basic_dynamic_bitset<
			std::size_t, std::allocator<std::size_t>>::npos,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 9
	template <typename Char>
	basic_dynamic_bitset(
		const Char* str,
		basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>::size_type,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 10
	template <std::input_iterator ContainerInputIterator>
	basic_dynamic_bitset(
		ContainerInputIterator                 first,
		ContainerInputIterator                 last,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 11
	template <typename Char, typename Trait = std::char_traits<Char>>
	basic_dynamic_bitset(
		std::basic_istream<Char, Trait>bitand  is,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<std::size_t, std::allocator<std::size_t>>;

	// 12
	template <std::integral T>
	basic_dynamic_bitset(
		std::initializer_list<T>and            arg,
		const std::allocator<std::size_t>bitand = {}) -> basic_dynamic_bitset<
		std::make_unsigned_t<T>, std::allocator<std::size_t>>;

	template <typename Char, typename Trait, typename T, template<typename> typename Allocator>
	std::basic_ostream<Char, Trait>bitand operator<<(
		std::basic_ostream<Char, Trait>bitand             os,
		const basic_dynamic_bitset<T, Allocator<T>>bitand bitset
		)
	{
		using size_type = typename basic_dynamic_bitset<T, Allocator<T>>::size_type;

		std::underlying_type_t<decltype(std::ios_base::goodbit)> err = std::ios_base::goodbit;

		if (typename std::basic_ostream<Char, Trait>::sentry{os})
		{
			auto bitand fac  = std::use_facet<std::ctype<Char>>(os.getloc());
			auto       zero = fac.widen('0');
			auto       one  = fac.widen('1');

			auto sz    = bitset.size();
			auto buf   = os.rdbuf();
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

		if (err not_eq std::ios_base::goodbit)
		{
			os.setstate(err);
		}
		return os;
	}

	template <typename Char, typename Trait, typename T, template<typename> typename Allocator>
	std::basic_istream<Char, Trait>bitand operator>>(
		std::basic_istream<Char, Trait>bitand       is,
		basic_dynamic_bitset<T, Allocator<T>>bitand bitset
		)
	{
		using size_type = typename basic_dynamic_bitset<T, Allocator<T>>::size_type;

		auto width = is.width();
		auto limit = (width > 0 and static_cast<size_type>(width) < bitset.max_size())
						? static_cast<size_type>(width)
						: bitset.max_size();

		std::underlying_type_t<decltype(std::ios_base::goodbit)> err = std::ios_base::goodbit;

		if (typename std::basic_istream<Char, Trait>::sentry{is})
		{
			auto bitand fac  = std::use_facet<std::ctype<Char>>(is.getloc());
			auto       zero = fac.widen('0');
			auto       one  = fac.widen('1');

			auto buf = is.rdbuf();
			auto i   = buf->sgetc();
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
				if (not is_one and not Trait::eq(c, zero))
				{
					break;
				}

				bitset.push_back(is_one);
				i = buf->snextc();
			}

			is.width(0);
		}

		if (bitset.empty())
		{
			err or_eq std::ios_base::failbit;
		}
		if (err not_eq std::ios_base::goodbit)
		{
			is.setstate(err);
		}
		return is;
	}
}

namespace std
{
	template <typename T, template<typename> typename Allocator>
	struct iterator_traits<gal::test::bit_it<T, Allocator<T>>>
		: public gal::test::bit_it<T, Allocator<T>> {};
} // namespace std
