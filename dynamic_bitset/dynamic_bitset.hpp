/**
 * Inspire from boost/dynamic_bitset (Distributed under the Boost Software License, Version 1.0.)(http://www.boost.org/LICENSE_1_0.txt)
 *
 * If there is no legal license conflict, the file will be distributed under the [see root project license] license.
 */

#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_HPP

#include <dynamic_bitset/dynamic_bitset_fwd.hpp>
#include <iosfwd>
#include <locale>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>
#include <vector>

#if !defined(GAL_ASSERT)
#if __has_include(<cassert>)
#define GAL_ASSERT assert
#else
#define GAL_ASSERT
#endif
#endif

namespace gal::toolbox::dynamic_bitset {
	namespace detail {
		template<std::unsigned_integral T>
		[[nodiscard("If you do not receive the return value of bits_of_type, this call is meaningless overhead")]] constexpr size_t bits_of_type() {
			return std::numeric_limits<T>::digits;
		}

		template<typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
		requires std::random_access_iterator<typename Container<T, Allocator<T>>::iterator> &&
				requires(typename Container<T, Allocator<T>>::iterator it) {
			typename Container<T, Allocator<T>>::iterator::iterator_category;
			typename Container<T, Allocator<T>>::iterator::difference_type;
			it.operator*();
		}
		class bit_it;

		template<typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
		class bit_ref {
		public:
			friend class basic_dynamic_bitset<T, Allocator, Container>;
			friend class bit_it<T, Allocator, Container>;
			using ref_container = basic_dynamic_bitset<T, Allocator, Container>;

			using value_type	= typename ref_container::value_type;
			using size_type		= typename ref_container::size_type;

		private:
			// note offset should less than ref_container::bits_of_type
			[[maybe_unused]] constexpr bit_ref(value_type& value, size_type offset) noexcept
				: value(value),
				  mask(value_type{1} << offset) {
				GAL_ASSERT(offset < ref_container::bits_of_type);
			}

		public:
			auto																														   operator&() = delete; /* NOLINT, let IDE stop prompting us that 'Do not overload unary operator&, it is dangerous' */

			[[nodiscard("If you do not receive the return value of bit_ref::operator bool, this call is meaningless overhead")]] constexpr operator bool() const noexcept /* NOLINT, let IDE stop prompting us to add explicit for implicit conversion */ {
				return (value & mask) != 0;
			}

			[[nodiscard("If you do not receive the return value of bit_ref::operator~, this call is meaningless overhead")]] constexpr bool operator~() const noexcept {
				return !this->operator bool();
			}

			constexpr bit_ref& flip() noexcept {
				value ^= mask;
				return *this;
			}

			constexpr bit_ref& operator=(bool v) noexcept {
				if (v) {
					value |= mask;
				} else {
					value &= ~mask;
				}
				return *this;
			}

			constexpr bit_ref& operator=(const bit_ref& other) noexcept {
				this->operator=(other.operator bool());
				return *this;
			}

			constexpr bit_ref& operator|=(bool v) noexcept {
				if (v) {
					value |= mask;
				}
				return *this;
			}

			constexpr bit_ref& operator&=(bool v) noexcept {
				if (!v) {
					value &= ~mask;
				}
				return *this;
			}

			constexpr bit_ref& operator^=(bool v) noexcept {
				if (v) {
					value ^= mask;
				}
				return *this;
			}

			constexpr bit_ref& operator-=(bool v) noexcept {
				if (v) {
					value &= ~mask;
				}
				return *this;
			}

		private:
			value_type&		 value;
			const value_type mask;
		};

		template<typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
		requires std::random_access_iterator<typename Container<T, Allocator<T>>::iterator> &&
				requires(typename Container<T, Allocator<T>>::iterator it) {
			typename Container<T, Allocator<T>>::iterator::iterator_category;
			typename Container<T, Allocator<T>>::iterator::difference_type;
			it.operator*();
		}
		class bit_it {
		public:
			friend class basic_dynamic_bitset<T, Allocator, Container>;
			using ref_container		= basic_dynamic_bitset<T, Allocator, Container>;

			using real_value_type	= typename ref_container::value_type;
			using size_type			= typename ref_container::size_type;

			using iterator_type		= typename Container<T, Allocator<T>>::iterator;
			using iterator_category = typename iterator_type::iterator_category;
			using difference_type	= typename iterator_type::difference_type;

			using value_type		= bit_ref<T, Allocator, Container>;

		private:
			constexpr bit_it(iterator_type it, size_type offset)
				: iterator(it),
				  index(offset) {}

		public:
			[[nodiscard("If you do not receive the return value of bit_it::operator++(int), this call is meaningless overhead (use operator++() instead)")]] constexpr bit_it operator++(int) const noexcept /* NOLINT */ {
				GAL_ASSERT(index + 1 > index);
				auto copy{*this};
				++index;
				return copy;
			}

			constexpr bit_it& operator++() noexcept {
				GAL_ASSERT(index + 1 > index);
				++index;
				return *this;
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator--(int), this call is meaningless overhead (use operator--() instead)")]] constexpr bit_it operator--(int) const noexcept /* NOLINT */ {
				GAL_ASSERT(index > 0);
				auto copy{*this};
				--index;
				return copy;
			}

			constexpr bit_it& operator--() noexcept {
				GAL_ASSERT(index > 0);
				--index;
				return *this;
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator+, this call is meaningless overhead")]] constexpr bit_it operator+(size_type n) const noexcept {
				GAL_ASSERT(index + n > index);
				return {iterator, index + n};
			}

			constexpr bit_it& operator+=(size_type n) noexcept {
				GAL_ASSERT(index + n > index);
				index += n;
				return *this;
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator-, this call is meaningless overhead")]] constexpr bit_it operator-(size_type n) const noexcept {
				GAL_ASSERT(index >= n);
				return {iterator, index - n};
			}

			constexpr bit_it& operator-=(size_type n) noexcept {
				GAL_ASSERT(index >= n);
				index -= n;
				return *this;
			}

			[[nodiscard("If you do not receive the return value of bit_it::distance, this call is meaningless overhead")]] constexpr size_type distance(const bit_it& other) const noexcept {
				GAL_ASSERT(other.index <= index);
				return index - other.index;
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator==, this call is meaningless overhead")]] constexpr bool operator==(const bit_it& other) const noexcept {
				return index == other.index && iterator == other.iterator;
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator!=, this call is meaningless overhead")]] constexpr bool operator!=(const bit_it& other) const noexcept {
				return !operator==(other);
			}

			[[nodiscard("If you do not receive the return value of bit_it::operator*, this call is meaningless overhead")]] constexpr value_type operator*() const noexcept {
				return {(iterator + ref_container::block_index(index)).operator*(), ref_container::bit_index(index)};
			}

		private:
			iterator_type iterator;
			size_type	  index;
		};
	}// namespace detail

	template<std::integral T, template<typename> typename Allocator, template<typename, typename> typename Container>
	requires requires {
		typename Container<T, Allocator<T>>::value_type;
		typename Container<T, Allocator<T>>::size_type;
		typename Container<T, Allocator<T>>::allocator_type;
	} &&
			requires(Container<T, Allocator<T>> c) {
		c.begin();
		c.end();
		c[0] = {};
		c.get_allocator();
		c.size();
		c.resize(typename Container<T, Allocator<T>>::size_type{}, typename Container<T, Allocator<T>>::value_type{});
		c.clear();
		c.push_back({});
		c.pop_back();
		c.insert(c.begin(), std::initializer_list<T>{}.begin(), std::initializer_list<T>{}.end());
	}
	class basic_dynamic_bitset {
		friend class detail::bit_ref<T, Allocator, Container>;
		friend class detail::bit_it<T, Allocator, Container>;

	public:
		using container_type						= Container<T, Allocator<T>>;
		using value_type							= typename container_type::value_type;

		using value_type_ref_if_cheaper				= value_type;
		// using value_type_ref_if_cheaper = const value_type&;

		using size_type								= typename container_type::size_type;
		using allocator_type						= typename container_type::allocator_type;

		using reference_type						= detail::bit_ref<T, Allocator, Container>;
		using iterator_type							= detail::bit_it<T, Allocator, Container>;

		constexpr static auto bits_of_type			= detail::bits_of_type<value_type>();
		constexpr static auto bits_of_unsigned_long = detail::bits_of_type<unsigned long>();
		constexpr static auto npos					= static_cast<size_type>(-1);

	public:
		/**
		 * @brief Default ctor
		 */
		constexpr basic_dynamic_bitset() noexcept(std::is_nothrow_default_constructible_v<container_type>)
			: container(),
			  total_size() {}

		/**
		 * @brief Ctor from alloc
		 * @param alloc the allocator of container
		 */
		constexpr explicit basic_dynamic_bitset(const allocator_type& alloc) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>)
			: container(alloc),
			  total_size() {}

		/**
		 * @brief Ctor from size, maybe need exchange the arg order of value and alloc
		 * @param size how many bits we want(will) to hold
		 * @param value the value to init
		 * @param alloc the allocator of container
		 */
		constexpr explicit basic_dynamic_bitset(size_type size, value_type_ref_if_cheaper value = {}, const allocator_type& alloc = {}) noexcept(
				std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(std::declval<basic_dynamic_bitset>().init_from_value_type(size, value)))
			: container(alloc),
			  total_size() {
			init_from_value_type(size, value);
		}

		/**
		 * @brief Ctor from str[pos~pos+n]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Alloc allocator_type of str
		 * @param str str
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 * @param alloc the allocator of container
		 */
		template<typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
		constexpr basic_dynamic_bitset(const std::basic_string<Char, Traits, Alloc>&			  str,
									   typename std::basic_string<Char, Traits, Alloc>::size_type pos,
									   typename std::basic_string<Char, Traits, Alloc>::size_type n,
									   size_type												  size	= npos,
									   const allocator_type&									  alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(std::declval<basic_dynamic_bitset>().init_from_basic_string(str, pos, n, size)))
			: container(alloc),
			  total_size() {
			init_from_basic_string(
					str,
					pos,
					n,
					size);
		}

		/**
		 * @brief Ctor from str[str~end]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Alloc allocator_type of str
		 * @param str str
		 * @param pos begin pos
		 * @param alloc the allocator of container
		 */
		template<typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
		constexpr explicit basic_dynamic_bitset(const std::basic_string<Char, Traits, Alloc>&			   str,
												typename std::basic_string<Char, Traits, Alloc>::size_type pos	 = {},
												const allocator_type&									   alloc = {}) noexcept(noexcept(basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Alloc>::npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, std::basic_string<Char, Traits, Alloc>::npos, npos, alloc) {}

		/**
		 * @brief Ctor from str_view[pos~pos+n]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str str_view
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size container size we need (better make sure size >= n)
		 * @param alloc the allocator of container
		 */
		template<typename Char, typename Traits = std::char_traits<Char>>
		constexpr basic_dynamic_bitset(std::basic_string_view<Char, Traits>						str,
									   typename std::basic_string_view<Char, Traits>::size_type pos,
									   typename std::basic_string_view<Char, Traits>::size_type n,
									   size_type												size  = npos,
									   const allocator_type&									alloc = {}) noexcept(std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(std::declval<basic_dynamic_bitset>().init_from_basic_string_view(str, pos, n, size)))
			: container(alloc),
			  total_size() {
			init_from_basic_string_view(
					str,
					pos,
					n,
					size);
		}

		/**
		 * @brief Ctor from str_view[str~end]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str str_view
		 * @param pos begin pos
		 * @param alloc the allocator of container
		 */
		template<typename Char, typename Traits = std::char_traits<Char>>
		constexpr explicit basic_dynamic_bitset(std::basic_string_view<Char, Traits>					 str,
												typename std::basic_string_view<Char, Traits>::size_type pos   = {},
												const allocator_type&									 alloc = {}) noexcept(noexcept(basic_dynamic_bitset(str, pos, std::basic_string_view<Char, Traits>::npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, std::basic_string_view<Char, Traits>::npos, npos, alloc) {}

		template<typename Char>
		constexpr explicit basic_dynamic_bitset(
				const Char*			  str,
				size_type			  pos,
				size_type			  n,
				size_type			  size	= npos,
				const allocator_type& alloc = {}) noexcept(noexcept(basic_dynamic_bitset(std::basic_string_view<Char>{str}, pos, n, size, alloc)))
			: basic_dynamic_bitset(std::basic_string_view<Char>{str}, pos, n, size, alloc) {}

		template<typename Char>
		constexpr explicit basic_dynamic_bitset(
				const Char*			  str,
				size_type			  pos,
				const allocator_type& alloc = {}) noexcept(noexcept(basic_dynamic_bitset(str, pos, npos, npos, alloc)))
			: basic_dynamic_bitset(str, pos, npos, npos, alloc) {}

		/**
		 * @brief Ctor from range (at least input_iterator)
		 * @tparam ContainerInputIterator iterator type of range
		 * @param first begin iterator
		 * @param last end iterator
		 * @param alloc the allocator of container
		 */
		template<std::input_iterator ContainerInputIterator>
		requires requires(ContainerInputIterator it) {
			std::is_convertible_v<decltype(*it), value_type>;
		} &&
				requires(container_type c) {
			c.insert(c.end(), ContainerInputIterator{}, ContainerInputIterator{});
		}
		constexpr basic_dynamic_bitset(ContainerInputIterator first, ContainerInputIterator last, const allocator_type& alloc = {}) noexcept(
				std::is_nothrow_constructible_v<container_type, allocator_type>&& noexcept(std::declval<basic_dynamic_bitset>().container.insert(std::declval<basic_dynamic_bitset>().container.begin(), first, last)))
			: container(alloc),
			  total_size() {
			container.insert(container.begin(), first, last);
			total_size += container.size() * bits_of_type;
		}

		/**
		 * @brief Ctor from std::basic_istream
		 * @tparam Char type of input char
		 * @tparam Trait trait of input char
		 * @param is basic_istream
		 */
		template<typename Char, typename Trait = std::char_traits<Char>>
		explicit basic_dynamic_bitset(std::basic_istream<Char, Trait>& is, const allocator_type& alloc = {})
			: container(alloc),
			  total_size() {
			is >> *this;
		}

		/**
		 * @brief Ctor from initializer_list
		 * @param args args for init
		 * @param alloc the allocator of container
		 */
		constexpr basic_dynamic_bitset(std::initializer_list<value_type>&& args, const allocator_type& alloc = {}) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container.insert(container.begin(), args.begin(), args.end())) && noexcept(std::declval<basic_dynamic_bitset>().container.size()))
			: container(alloc),
			  total_size() {
			container.insert(container.begin(), std::forward<std::initializer_list<value_type>>(args));
			total_size += container.size() * bits_of_type;
		}

		/**
		 * @brief Dtor
		 */
		constexpr ~basic_dynamic_bitset() {
			GAL_ASSERT(check_invariants());
		}

		/**
		 * @brief copy ctor
		 * @param other
		 */
		constexpr basic_dynamic_bitset(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_constructible_v<container_type>)
			: container(other.container),
			  total_size(other.total_size) {}

		/**
		 * @brief Copy assign operator
		 * @param other another dynamic_bitset for copy
		 * @return self
		 */
		basic_dynamic_bitset& operator=(const basic_dynamic_bitset& other) noexcept(std::is_nothrow_copy_assignable_v<container_type>) {
			container  = other.container;
			total_size = other.total_size;
			return *this;
		}

		/**
		 * @brief Move ctor
		 * @param other another dynamic_bitset for move
		 */
		basic_dynamic_bitset(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_constructible_v<container_type>)
			: container(std::move(other.container)),
			  total_size(std::move(other.total_size)) {
			// Required so that check_invariants() works.
			GAL_ASSERT((other.container = container_type{}).empty());// NOLINT
			other.total_size = 0;
		}

		/**
		 * @brief Move assign operator
		 * @param other another dynamic_bitset for move
		 * @return self
		 */
		basic_dynamic_bitset& operator=(basic_dynamic_bitset&& other) noexcept(std::is_nothrow_move_assignable_v<container_type>) {
			if (std::addressof(other) == this) {
				return *this;
			}
			container  = std::move(other.container);
			total_size = std::move(other.total_size);
			// Required so that check_invariants() works.
			GAL_ASSERT((other.container = container_type{}).empty());// NOLINT
			other.total_size = 0;
			return *this;
		}

		/**
		 * @brief Swap --> swap container and size
		 * @param other another dynamic_bitset
		 */
		constexpr void swap(basic_dynamic_bitset& other) noexcept(noexcept(std::swap(container, other.container))) {
			std::swap(container, other.container);
			std::swap(total_size, other.total_size);
		}

		/**
		 * @brief Get allocator --> get container's allocator
		 * @return container's allocator
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::get_allocator, this call is meaningless overhead")]] constexpr allocator_type get_allocator() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.get_allocator())) {
			return container.get_allocator();
		}

		/**
		 * @brief Get size (bits we hold, not container size)
		 * @return bits we hold
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::size, this call is meaningless overhead")]] constexpr size_type size() const noexcept {
			return total_size;
		}

		/**
		 * @brief Get container's real size we used
		 * @return container's size
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::container_size, this call is meaningless overhead")]] constexpr size_type container_size() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.size())) {
			return container.size();
		}

		/**
		 * @brief Get the max bits we can hold (normally, bits_of_type * max_size_of_container)
		 * @return max bits size
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::max_size, this call is meaningless overhead")]] constexpr auto max_size() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container.max_size()) && noexcept(std::declval<basic_dynamic_bitset>().get_allocator()) && noexcept(std::allocator_traits<allocator_type>::max_size(std::declval<basic_dynamic_bitset>().get_allocator()))) {
			auto container_max = container.max_size();
			auto alloc_max	   = std::allocator_traits<allocator_type>::max_size(get_allocator());

			auto max		   = std::min(container_max, alloc_max);
			return max <= static_cast<size_type>(-1) / bits_of_type ? max * bits_of_type : static_cast<size_type>(-1);
		}

		/**
		 * @brief Are we hold zero bits?
		 * @return empty
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::empty, this call is meaningless overhead")]] constexpr bool empty() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().size())) {
			return size() == 0;
		}

		/**
		 * @brief Get the capacity of bits we hold (normally, bits_of_type * capacity_of_container)
		 * @return what capacity is now
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::capacity, this call is meaningless overhead")]] constexpr size_type capacity() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.capacity())) {
			return container.capacity() * bits_of_type;
		}

		/**
		 * @brief Reserve enough memory for use (normally, size / bits_of_type, or container_real_size)
		 * @param size how many size we need reserve
		 */
		constexpr void reserve(size_type size) noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.reserve(size))) {
			container.reserve(calc_blocks_needed(size));
		}

		/**
		 * @brief Shrink to fit (normally, make container shrink to fit)
		 */
		constexpr void shrink_to_fit() noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container.size()) && noexcept(std::declval<basic_dynamic_bitset>().container.capacity()) &&
				std::is_nothrow_swappable_v<container_type>) {
			if (container.size() < container.capacity()) {
				container_type{container}.swap(container);
			}
		}

	public:
		/**
		 * @brief Set blocks between pos and pos+len (dependent set or not)
		 * @param pos begin pos
		 * @param len length
		 * @param set set or reset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& set(size_type pos, size_type len, bool set) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos, len, &std::declval<basic_dynamic_bitset>().set_block_partial, &std::declval<basic_dynamic_bitset>().set_block_full)) && noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos, len, &std::declval<basic_dynamic_bitset>().reset_block_partial, &std::declval<basic_dynamic_bitset>().reset_block_full))) {
			if (set) {
				return range_operation(pos, len, set_block_partial, set_block_full);
			} else {
				return range_operation(pos, len, reset_block_partial, reset_block_full);
			}
		}

		/**
		 * @brief Set a bit's value (dependent set or not)
		 * @param pos pos of bit
		 * @param set set or reset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& set(size_type pos, bool set = true) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) && noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {}) && noexcept(std::declval<basic_dynamic_bitset>().reset(pos))) {
			GAL_ASSERT(pos < total_size);

			if (set) {
				container[block_index(pos)] |= bit_mask(pos);
			} else {
				reset(pos);
			}
			return *this;
		}

		/**
		 * @brief Set all block's value
		 * @return self
		 */
		constexpr basic_dynamic_bitset& set() noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container.begin()) && noexcept(std::declval<basic_dynamic_bitset>().container.end()) && noexcept(*std::declval<basic_dynamic_bitset>().container.begin() = {}) && noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())) {
			for (auto it = container.begin(); it != container.end(); ++it) {
				*it = static_cast<value_type>(~0);
			}
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief Reset blocks between pos and pos+len
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		constexpr basic_dynamic_bitset& reset(size_type pos, size_type len) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos, len, std::declval<basic_dynamic_bitset>().reset_block_partial, std::declval<basic_dynamic_bitset>().reset_block_full))) {
			return range_operation(pos, len, reset_block_partial, reset_block_full);
		}

		/**
		 * @brief Reset a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		constexpr basic_dynamic_bitset& reset(size_type pos) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) && noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(pos < total_size);

			container[block_index(pos)] &= ~bit_mask(pos);
			return *this;
		}

		/**
		 * @brief Reset all block's value
		 * @return self
		 */
		constexpr basic_dynamic_bitset& reset() noexcept(
				noexcept(*std::declval<basic_dynamic_bitset>().container.begin() = {})) {
			for (auto it = container.begin(); it != container.end(); ++it) {
				*it = static_cast<value_type>(0);
			}
			return *this;
		}

		/**
		 * @brief Flip blocks between pos and pos+len (dependent value)
		 * @param pos begin pos
		 * @param len length
		 * @return self
		 */
		constexpr basic_dynamic_bitset& flip(size_type pos, size_type len) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().range_operation(pos, len, std::declval<basic_dynamic_bitset>().flip_block_partial, std::declval<basic_dynamic_bitset>().flip_block_full))) {
			return range_operation(pos, len, flip_block_partial, flip_block_full);
		}

		/**
		 * @brief Flip a bit's value
		 * @param pos pos of bit
		 * @return self
		 */
		constexpr basic_dynamic_bitset& flip(size_type pos) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) && noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(pos < total_size);

			container[block_index(pos)] ^= bit_mask(pos);
			return *this;
		}

		/**
		 * @brief Flip all block's value
		 * @return self
		 */
		constexpr basic_dynamic_bitset& flip() noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())) {
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] = ~container[i];
			}
			zero_unused_bits();
			return *this;
		}

		/**
		 * @brief Test a bit only
		 * @param pos pos of bit
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::test, this call is meaningless overhead")]] constexpr bool test(size_type pos) const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().unchecked_test(pos))) {
			GAL_ASSERT(pos < total_size);
			return unchecked_test(pos);
		}

		/**
		 * @brief Test a bit and if it not equal to value, set it
		 * @param pos pos of bit
		 * @param value expect value
		 * @return tested value
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::test_and_set, this call is meaningless overhead (use dynamic_bitset::set instead)")]] constexpr bool test_and_set(size_type pos, bool value) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().test(pos)) && noexcept(std::declval<basic_dynamic_bitset>().set(pos, value))) {
			auto b = test(pos);
			if (b != value) {
				set(pos, value);
			}
			return b;
		}

		/**
		 * @brief Is all bit been set ?
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::all, this call is meaningless overhead")]] constexpr bool all() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().empty()) && noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) && noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			if (empty()) {
				return true;
			}

			auto extra	 = count_extra_bits();
			auto all_one = static_cast<value_type>(~0);

			if (extra == 0) {
				for (decltype(container_size()) i = 0; i < container_size(); ++i) {
					if (container[i] != all_one) {
						return false;
					}
				}
			} else {
				for (decltype(container_size()) i = 0; i < container_size() - 1; ++i) {
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

		/**
		 * @brief Is any bit been set ?
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::any, this call is meaningless overhead")]] constexpr bool any() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			if (empty()) {
				return false;
			}
			for (decltype(container_size()) i = 0; i < container_size(); ++i) {
				if (container[i]) {
					return true;
				}
			}
			return false;
		}

		/**
		 * @brief Is none bit been set ?
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::none, this call is meaningless overhead")]] constexpr bool none() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().any())) {
			return !any();
		}

		/**
		 * @brief Count how many bit been set
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::count, this call is meaningless overhead")]] constexpr size_type count() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container_size())) {
			constexpr auto pop_count = [](value_type_ref_if_cheaper value) constexpr noexcept {
				static_assert(sizeof(std::byte) == sizeof(unsigned char));
				static_assert(sizeof(unsigned char) == 1);
				constexpr auto			table_width = 8;
				constexpr unsigned char count_table[] =
						{
								0,
								1,
								1,
								2,
								1,
								2,
								2,
								3,
								1,
								2,
								2,
								3,
								2,
								3,
								3,
								4,
								1,
								2,
								2,
								3,
								2,
								3,
								3,
								4,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								1,
								2,
								2,
								3,
								2,
								3,
								3,
								4,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								1,
								2,
								2,
								3,
								2,
								3,
								3,
								4,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								4,
								5,
								5,
								6,
								5,
								6,
								6,
								7,
								1,
								2,
								2,
								3,
								2,
								3,
								3,
								4,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								4,
								5,
								5,
								6,
								5,
								6,
								6,
								7,
								2,
								3,
								3,
								4,
								3,
								4,
								4,
								5,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								4,
								5,
								5,
								6,
								5,
								6,
								6,
								7,
								3,
								4,
								4,
								5,
								4,
								5,
								5,
								6,
								4,
								5,
								5,
								6,
								5,
								6,
								6,
								7,
								4,
								5,
								5,
								6,
								5,
								6,
								6,
								7,
								5,
								6,
								6,
								7,
								6,
								7,
								7,
								8};
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

		/**
		 * @brief Get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator[], this call is meaningless overhead")]] constexpr reference_type operator[](size_type index) noexcept(std::is_nothrow_constructible_v<reference_type, value_type&, size_type>) {
			return {container[block_index(index)], bit_index(index)};
		}

		/**
		 * @brief Get a bit ref
		 * @param index index of bit
		 * @return ref
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator[], this call is meaningless overhead")]] constexpr reference_type operator[](size_type index) const
				noexcept(std::is_nothrow_constructible_v<reference_type, value_type&, size_type>) {
			return {container[block_index(index)], bit_index(index)};
		}

		/**
		 * @brief Get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::begin, this call is meaningless overhead")]] constexpr iterator_type begin() noexcept(std::is_nothrow_constructible_v<iterator_type, decltype(std::declval<basic_dynamic_bitset>().container.begin()), size_type>) {
			return {container.begin(), 0};
		}

		/**
		 * @brief Get a iterator in container.begin()
		 * @return iterator
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::begin, this call is meaningless overhead")]] constexpr iterator_type begin() const noexcept(std::is_nothrow_constructible_v<iterator_type, decltype(std::declval<basic_dynamic_bitset>().container.begin()), size_type>) {
			return {container.begin(), 0};
		}

		/**
		 * @brief Get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::begin, this call is meaningless overhead")]] constexpr iterator_type end() noexcept(std::is_nothrow_constructible_v<iterator_type, decltype(std::declval<basic_dynamic_bitset>().container.end()), size_type>) {
			return {container.begin(), total_size};
		}

		/**
		 * @brief Get a iterator in container.end()
		 * @return iterator
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::begin, this call is meaningless overhead")]] constexpr iterator_type end() const noexcept(std::is_nothrow_constructible_v<iterator_type, decltype(std::declval<basic_dynamic_bitset>().container.end()), size_type>) {
			return {container.begin(), total_size};
		}

		/**
		 * @brief Resize the container to required size
		 * @param size required size
		 * @param value value to set
		 */
		constexpr void resize(size_type size, bool value = false) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) && noexcept(std::declval<basic_dynamic_bitset>().container.resize(size_type{}, value_type{})) && noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) && noexcept(std::declval<basic_dynamic_bitset>().container.size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0]) && noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())) {
			auto old_size	   = container_size();
			auto required_size = calc_blocks_needed(size);

			auto v			   = value ? ~value_type{0} : value_type{0};

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

		/**
		 * @brief Clear container
		 */
		constexpr void clear() noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.clear())) {
			container.clear();
			total_size = 0;
		}

		/**
		 * @brief Push a bit from back
		 * @param value bit's value
		 */
		constexpr void push_back(bool value) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().size()) && noexcept(std::declval<basic_dynamic_bitset>().resize(0)) && noexcept(std::declval<basic_dynamic_bitset>().size(), value)) {
			auto sz = size();
			resize(sz + 1, value);
		}

		/**
		 * @brief Pop a bit from back
		 */
		constexpr void pop_back() noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(total_size - 1)) && noexcept(std::declval<basic_dynamic_bitset>().container.pop_back()) && noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())) {
			auto old_size	   = container_size();
			auto required_size = calc_blocks_needed(total_size - 1);

			if (required_size != old_size) {
				container.pop_back();
			}
			--total_size;
			zero_unused_bits();
		}

		/**
		 * @brief Append a value (not a bit) from back (it means you will append bits_of_type bits from back)
		 * @param value value for append
		 */
		constexpr void append(value_type_ref_if_cheaper value) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) && noexcept(std::declval<basic_dynamic_bitset>().container.push_back(value)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
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

		/**
		 * @brief &= with another dynamic_bitset, get all bits' intersection set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator&=(const basic_dynamic_bitset& other) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] &= other.container[i];
			}
			return *this;
		}

		/**
		 * @brief |= with another dynamic_bitset, get all bits' union set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator|=(const basic_dynamic_bitset& other) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] |= other.container[i];
			}
			return *this;
		}

		/**
		 * @brief ^= with another dynamic_bitset, get all bits' symmetric
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator^=(const basic_dynamic_bitset& other) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] ^= other.container[i];
			}
			return *this;
		}

		/**
		 * @brief -= with another dynamic_bitset, get all bits' subtraction set
		 * @param other another dynamic_bitset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator-=(const basic_dynamic_bitset& other) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {})) {
			GAL_ASSERT(size() == other.size());
			for (size_type i = 0; i < container_size(); ++i) {
				container[i] &= ~other.container[i];
			}
			return *this;
		}

		/**
		 * @brief Left shift self
		 * @param n offset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator<<=(size_type n) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().reset()) && noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().block_index(n)) && noexcept(std::declval<basic_dynamic_bitset>().bit_index(n)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {}) && noexcept(std::declval<basic_dynamic_bitset>().zero_unused_bits())) {
			if (n >= total_size) {
				return reset();
			}

			// note that we store every bit from back to front, it means left shift actually is right shift
			// from blocks_need_discard to total_blocks is the part we need save
			// or container[blocks_need_discard : total_blocks]
			auto total_blocks		 = container_size() - 1;
			auto blocks_need_discard = block_index(n);
			auto remained_bits		 = bit_index(n);

			if (remained_bits == 0) {
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = 0; i <= total_blocks - blocks_need_discard; ++i) {
					container[blocks_need_discard + i] = container[i];
				}
			} else {
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

				for (decltype(blocks_need_discard) i = 1; i <= total_blocks - blocks_need_discard; ++i) {
					// store this block's remained_bits part and last (actually, is next block, but we store every bit from back to front) block's extra_bits_in_block part
					container[blocks_need_discard + i] = (container[i - 1] >> extra_bits_in_block) | (container[i] << remained_bits);
				}
				container[blocks_need_discard] = container[0] << remained_bits;
			}

			// zero out front part
			for (decltype(total_blocks) i = 0; i < blocks_need_discard; ++i) {
				container[i] = static_cast<value_type>(0);
			}

			// zero out any 1 bit that flowed into the unused part
			zero_unused_bits();

			return *this;
		}

		/**
		 * @brief Right shift self
		 * @param n offset
		 * @return self
		 */
		constexpr basic_dynamic_bitset& operator>>=(size_type n) noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<<=(n))) {
			if (n >= total_size) {
				return reset();
			}

			// note that we store every bit from back to front, it means right shift actually is left shift
			// from 0 to blocks_need_discard is the part we need save
			// or container[0 : blocks_need_discard]
			auto total_blocks		 = container_size() - 1;
			auto blocks_need_discard = block_index(n);
			auto remained_bits		 = bit_index(n);

			if (remained_bits == 0) {
				// it is full fit a block
				// copy remained part which we needed
				for (decltype(blocks_need_discard) i = blocks_need_discard; i <= total_blocks; ++i) {
					container[i - blocks_need_discard] = container[i];
				}
			} else {
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

				for (decltype(blocks_need_discard) i = blocks_need_discard; i < total_blocks; ++i) {
					// store this block's remained_bits part and next (actually, is last block, but we store every bit from back to front) block's extra_bits_in_block part
					container[i - blocks_need_discard] = (container[i + 1] << extra_bits_in_block) | (container[i] >> remained_bits);
				}
				container[total_blocks - blocks_need_discard] = container[total_blocks] >> remained_bits;
			}

			// zero out back part
			for (decltype(total_blocks) i = 0, begin = total_blocks - blocks_need_discard; i < blocks_need_discard; ++i) {
				container[begin + i] = static_cast<value_type>(0);
			}

			return *this;
		}

		/**
		 * @brief Left shift a copy
		 * @param n offset
		 * @return copy
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator<<, this call is meaningless overhead")]] constexpr basic_dynamic_bitset operator<<(size_type n) const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<<=(n))) {
			auto copy{*this};
			copy <<= n;
			return copy;
		}

		/**
		 * @brief Right shift a copy
		 * @param n offset
		 * @return copy
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator>>, this call is meaningless overhead")]] constexpr basic_dynamic_bitset operator>>(size_type n) const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator>>=(n))) {
			auto copy{*this};
			copy >>= n;
			return copy;
		}

		/**
		 * @brief Flip a copy all bit
		 * @return copy
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator~, this call is meaningless overhead")]] constexpr basic_dynamic_bitset operator~() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().flip())) {
			auto copy{*this};
			copy.flip();
			return copy;
		}

		/**
		 * @brief Is the same dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator==, this call is meaningless overhead")]] constexpr bool operator==(const basic_dynamic_bitset& other) const noexcept {
			return total_size == other.total_size && container == other.container;
		}

		/**
		 * @brief Is not the same dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator!=, this call is meaningless overhead")]] constexpr bool operator!=(const basic_dynamic_bitset& other) const noexcept {
			return !operator==(other);
		}

		/**
		 * @brief Is self less than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator<, this call is meaningless overhead")]] constexpr bool operator<(const basic_dynamic_bitset& other) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().empty()) && noexcept(std::declval<basic_dynamic_bitset>().size()) && noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			if (other.empty()) {
				return false;
			} else if (empty()) {
				return true;
			} else if (auto sz = size(), sz_other = other.size(); sz == sz_other) {
				for (size_type i = container_size() - 1; i >= 0; --i) {
					value_type_ref_if_cheaper curr		 = container[i];
					value_type_ref_if_cheaper curr_other = other.container[i];

					if (curr != curr_other) {
						return curr < curr_other;
					}
				}
				return false;
			} else {
				size_type intersect_size = std::min(sz, sz_other);

				for (auto a = sz - 1, b = sz_other - 1, count = 0; count < intersect_size; ++count, --a, --b) {
					value_type_ref_if_cheaper curr		 = container[a];
					value_type_ref_if_cheaper curr_other = other.container[b];

					if (curr != curr_other) {
						return curr < curr_other;
					}
				}
				return sz < sz_other;
			}
		}

		/**
		 * @brief Is self less_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator<=, this call is meaningless overhead")]] constexpr bool operator<=(const basic_dynamic_bitset& other) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().operator==(other)) && noexcept(std::declval<basic_dynamic_bitset>().operator<(other))) {
			return operator==(other) || operator<(other);
		}

		/**
		 * @brief Is self greater than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator>, this call is meaningless overhead")]] constexpr bool operator>(const basic_dynamic_bitset& other) const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<=(other))) {
			return !operator<=(other);
		}

		/**
		 * @brief Is self greater_equal than another dynamic_bitset ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::operator>=, this call is meaningless overhead")]] constexpr bool operator>=(const basic_dynamic_bitset& other) const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().operator<(other))) {
			return !operator<(other);
		}

		/**
		 * @brief Cast self to another type value
		 * @tparam To dest type
		 * @return result
		 */
		template<typename To = unsigned long>
		requires(sizeof(To) >= sizeof(T))
				[[nodiscard("If you do not receive the return value of dynamic_bitset::cast_to, this call is meaningless overhead")]] constexpr To cast_to() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().empty()) && noexcept(detail::bits_of_type<To>()) && noexcept(std::declval<basic_dynamic_bitset>().block_index(0))) {
			if (empty()) {
				return {};
			}

			auto maximum_size = std::min(total_size, static_cast<size_type>(detail::bits_of_type<To>()));
			auto last_block	  = block_index(maximum_size - 1);

			GAL_ASSERT((last_block * bits_of_type) < static_cast<size_type>(detail::bits_of_type<To>()));

			To result{};
			for (size_type i = 0; i <= last_block; ++i) {
				auto offset = i * bits_of_type;
				result |= static_cast<To>(container[i]) << offset;
			}
			return result;
		}

		/**
		 * @brief Is self is a subset of other ? (all set bits of self also set in other, but `maybe` other has set more bits)
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::is_subset_of, this call is meaningless overhead")]] constexpr bool is_subset_of(const basic_dynamic_bitset& other) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			GAL_ASSERT(size() == other.size());

			for (decltype(container_size()) i = 0; i < container_size(); ++i) {
				if (container[i] & ~other.container[i]) {
					return false;
				}
			}
			return true;
		}

		/**
		 * @brief Is self is a subset of other ? (all set bits of self also set in other, but other has set more bits)
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::is_proper_subset_of, this call is meaningless overhead")]] constexpr bool is_proper_subset_of(const basic_dynamic_bitset& other) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			GAL_ASSERT(size() == other.size());
			GAL_ASSERT(container_size() == other.container_size());

			bool proper = false;
			for (size_type i = 0; i < container_size(); ++i) {
				value_type_ref_if_cheaper a = container[i];
				value_type_ref_if_cheaper b = other.container[i];

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

		/**
		 * @brief Is self has is_intersects in every block with other ?
		 * @param other another dynamic_bitset
		 * @return result
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::is_intersects, this call is meaningless overhead")]] constexpr bool is_intersects(const basic_dynamic_bitset& other) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			auto intersect_size = std::min(container_size(), other.container_size());

			for (size_type i = 0; i < intersect_size; ++i) {
				if (container[i] & other.container[i]) {
					return true;
				}
			}
			return false;
		}

	private:
		/**
		 * @brief Get the block where the current position is
		 * @param pos the pos of bit is
		 * @return index
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::block_index, this call is meaningless overhead")]] constexpr static size_type block_index(size_type pos) noexcept {
			return pos / bits_of_type;
		}
		/**
		 * @brief Get the bit of the current position (in the block where the current position is)
		 * @param pos the pos of the bit is
		 * @return index
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::bit_index, this call is meaningless overhead")]] constexpr static size_type bit_index(size_type pos) noexcept {
			return pos % bits_of_type;
		}
		/**
		 * @brief Get the bit mask of the current position (in the block where the current position is)
		 * @param pos the pos of bit is
		 * @return mask
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::bit_mask, this call is meaningless overhead")]] constexpr static value_type bit_mask(size_type pos) noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_index(pos))) {
			return value_type{1} << bit_index(pos);
		}
		/**
		 * @brief Get the bit mask from first to last
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return mask
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::bit_mask, this call is meaningless overhead")]] constexpr static value_type bit_mask(size_type first, size_type last) noexcept {
			GAL_ASSERT(first <= last);
			GAL_ASSERT(last < bits_of_type);
			auto res = (last == bits_of_type - 1) ? static_cast<value_type>(~0) : ((value_type{1} << (last + 1)) - 1);
			return res ^ ((value_type{1} << first) - 1);
		}
		/**
		 * @brief Set the block's set by the bit mask from first to last and set (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @param set set or reset
		 * @return block after set
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::set_block_bits, this call is meaningless overhead")]] constexpr static value_type set_block_bits(value_type_ref_if_cheaper block, size_type first, size_type last, bool set) noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_mask(first, last))) {
			if (set) {
				return block | bit_mask(first, last);
			} else {
				return block & static_cast<value_type>(~bit_mask(first, last));
			}
		}

		/**
		 * @brief Set the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return block after set
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::set_block_partial, this call is meaningless overhead")]] constexpr static value_type set_block_partial(value_type_ref_if_cheaper block, size_type first, size_type last) noexcept(noexcept(std::declval<basic_dynamic_bitset>().set_block_bits(block, first, last, true))) {
			return set_block_bits(block, first, last, true);
		}

		/**
		 * @brief Set the block's value to full (wont change itself)
		 * @return ~0
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::set_block_full, this call is meaningless overhead")]] constexpr static value_type set_block_full(value_type_ref_if_cheaper) noexcept {
			return static_cast<value_type>(~0);
		}

		/**
		 * @brief Reset the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last ebd for get mask
		 * @return block after reset
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::reset_block_partial, this call is meaningless overhead")]] constexpr static value_type reset_block_partial(value_type_ref_if_cheaper block, size_type first, size_type last) noexcept(noexcept(std::declval<basic_dynamic_bitset>().set_block_bits(block, first, last, false))) {
			return set_block_bits(block, first, last, false);
		}

		/**
		 * @brief Reset the block's value to full (wont change itself)
		 * @return 0
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::reset_block_full, this call is meaningless overhead")]] constexpr static value_type reset_block_full(value_type_ref_if_cheaper) noexcept {
			return 0;
		}

		/**
		 * @brief Flip the block's value by the bit mask from first to last (wont change itself)
		 * @param block current block
		 * @param first begin for get mask
		 * @param last end for get mask
		 * @return block after flip
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::flip_block_partial, this call is meaningless overhead")]] constexpr static value_type flip_block_partial(value_type_ref_if_cheaper block, size_type first, size_type last) noexcept(noexcept(std::declval<basic_dynamic_bitset>().bit_mask(first, last))) {
			return block ^ bit_mask(first, last);
		}

		/**
		 * @brief Flip the block's value to full (wont change itself)
		 * @param block current block
		 * @return block after flip
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::flip_block_full, this call is meaningless overhead")]] constexpr static value_type flip_block_full(value_type_ref_if_cheaper block) noexcept {
			return ~block;
		}

		/**
		 * @brief Just check a bit
		 * @param pos bit's pos
		 * @return it this bit set
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::unchecked_test, this call is meaningless overhead")]] constexpr bool unchecked_test(size_type pos) const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().bit_mask(pos)) && noexcept(std::declval<basic_dynamic_bitset>().block_index(pos)) && noexcept(std::declval<basic_dynamic_bitset>().container[0])) {
			return (container[block_index(pos)] & bit_mask(pos)) != 0;
		}

		/**
		 * @brief Calc how many blocks we need to store num_bits bits
		 * @param num_bits how many bits we want to hold
		 * @return blocks we need
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::calc_blocks_needed, this call is meaningless overhead")]] constexpr size_type calc_blocks_needed(size_type num_bits) const noexcept {
			return num_bits / bits_of_type + static_cast<size_type>(num_bits % bits_of_type != 0);
		}

		/**
		 * @brief Get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::highest_block, this call is meaningless overhead")]] constexpr value_type& highest_block() noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.back())) {
			GAL_ASSERT(size() > 0 && container_size() > 0);
			return container.back();
		}

		/**
		 * @brief Get the highest block in container (normally, the container.back() element)
		 * @return highest block
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::highest_block, this call is meaningless overhead")]] constexpr const value_type& highest_block() const
				noexcept(noexcept(std::declval<basic_dynamic_bitset>().container.back())) {
			GAL_ASSERT(size() > 0 && container_size() > 0);
			return container.back();
		}

		/**
		 * @brief Init a dynamic_bitset from value_type
		 * @param size how many bits we want to hold
		 * @param value the value for init
		 */
		constexpr void init_from_value_type(size_type size, value_type value) noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) && noexcept(std::declval<basic_dynamic_bitset>().container.resize(calc_blocks_needed(size)))) {
			GAL_ASSERT(container_size() == 0);

			container.resize(calc_blocks_needed(size));
			total_size = size;

			// zero out all bits at pos >= num_bits, if any;
			// note that: num_bits == 0 implies value == 0
			if (size < static_cast<size_type>(bits_of_unsigned_long)) {
				value &= ((value_type{1} << size) - 1);
			}

			constexpr auto left_shifter = [](value_type & value) constexpr->void {
				if (bits_of_type >= bits_of_unsigned_long) {
					value = 0;
				} else {
					value = static_cast<value_type>(static_cast<decltype(bits_of_unsigned_long)>(value) >> bits_of_type);
				}
			};

			for (auto it = container.begin();; ++it) {
				*it = static_cast<value_type>(value);
				left_shifter(value);

				if (value == 0) break;
			}
		}

		/**
		 * @brief Init a dynamic_bitset from str[pos~pos+n]
		 * @tparam Char char_type of str
		 * @tparam Traits trait_type of str
		 * @tparam Alloc alloc_type of str
		 * @param str the str we used
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size how many bits we want(will) to hold, better make sure it greater than n
		 */
		template<typename Char, typename Traits, typename Alloc>
		constexpr void init_from_basic_string(const std::basic_string<Char, Traits, Alloc>&				 str,
											  typename std::basic_string<Char, Traits, Alloc>::size_type pos,
											  typename std::basic_string<Char, Traits, Alloc>::size_type n,
											  size_type													 size) noexcept(noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) && noexcept(std::declval<basic_dynamic_bitset>().container.resize(size))) {
			GAL_ASSERT(pos <= str.size());
			GAL_ASSERT(n <= size);

			// make sure the end pos is valid
			auto len	  = std::min(n, str.size() - pos);
			auto num_bits = size != npos ? size : len;
			container.resize(calc_blocks_needed(num_bits));
			total_size			 = num_bits;

			auto& fac			 = std::use_facet<std::ctype<Char>>(std::locale());
			auto  one			 = fac.widen('1');
			auto  zero			 = fac.widen('0');

			auto  shorter		 = std::min(num_bits, len);
			using str_size_type	 = typename std::basic_string<Char, Traits, Alloc>::size_type;
			using str_trait_type = typename std::basic_string<Char, Traits, Alloc>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i) {
				auto c		= str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (!is_one && !str_trait_type::eq(c, zero)) {
					break;
				}

				if (is_one) {
					set(i);
				}
			}
		}

		/**
		 * @brief Init a dynamic_bitset from str_view[pos~pos+n]
		 * @tparam Char char_type of str_view
		 * @tparam Traits trait_type of str_view
		 * @param str the str_view we used
		 * @param pos begin pos
		 * @param n how many char we used
		 * @param size how many bits we want(will) to hold, better make sure it greater than n
		 */
		template<typename Char, typename Traits>
		constexpr void init_from_basic_string_view(const std::basic_string_view<Char, Traits>				str,
												   typename std::basic_string_view<Char, Traits>::size_type pos,
												   typename std::basic_string_view<Char, Traits>::size_type n,
												   size_type												size) noexcept(noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(size)) && noexcept(std::declval<basic_dynamic_bitset>().container.resize(size))) {
			GAL_ASSERT(pos <= str.size());
			GAL_ASSERT(n <= size);

			// make sure the end pos is valid
			auto len	  = std::min(n, str.size() - pos);
			auto num_bits = size != npos ? size : len;
			container.resize(calc_blocks_needed(num_bits));
			total_size			 = num_bits;

			auto& fac			 = std::use_facet<std::ctype<Char>>(std::locale());
			auto  one			 = fac.widen('1');
			auto  zero			 = fac.widen('0');

			auto  shorter		 = std::min(num_bits, len);
			using str_size_type	 = typename std::basic_string_view<Char, Traits>::size_type;
			using str_trait_type = typename std::basic_string_view<Char, Traits>::traits_type;
			for (str_size_type i = 0, begin = pos + shorter - 1; i < shorter; ++i) {
				auto c		= str[begin - i];

				auto is_one = str_trait_type::eq(c, one);
				if (!is_one && !str_trait_type::eq(c, zero)) {
					break;
				}

				if (is_one) {
					set(i);
				}
			}
		}

		/**
		 * @briefl Process the block between pos and pos+len
		 * @param pos begin pos
		 * @param len length
		 * @param partial_block_operation range processor, process a block (first-arg) from second-arg to third-arg
		 * @param full_block_operation range processor, process a full block
		 * @return self
		 */
		constexpr basic_dynamic_bitset& range_operation(size_type pos,
														size_type len,
														value_type (*partial_block_operation)(value_type_ref_if_cheaper, size_type, size_type),
														value_type (*full_block_operation)(value_type_ref_if_cheaper)) noexcept(noexcept(std::declval<basic_dynamic_bitset>().block_index(0)) && noexcept(std::declval<basic_dynamic_bitset>().bit_index(0)) && noexcept(std::declval<basic_dynamic_bitset>().container[0] = {}) && noexcept(partial_block_operation(std::declval<basic_dynamic_bitset>().container[0], 0, 0)) && noexcept(full_block_operation(std::declval<basic_dynamic_bitset>().container[0]))) {
			GAL_ASSERT(pos + len <= total_size);

			// Do nothing in case of zero length
			if (len == 0) {
				return *this;
			}

			// Use an additional asserts in order to detect size_type overflow
			// For example: pos = 10, len = size_type_limit - 2, pos + len = 7
			// In case of overflow, 'pos + len' is always smaller than 'len'
			GAL_ASSERT(pos + len >= len);

			// Start and end blocks of the [pos : pos + len - 1] sequence
			auto first_block = block_index(pos);
			auto last_block	 = block_index(pos + len - 1);
			auto first_bit	 = bit_index(pos);
			auto last_bit	 = bit_index(pos + len - 1);

			if (first_block == last_block) {
				// Filling only a sub-block of a block
				container[first_block] = partial_block_operation(container[first_block], first_bit, last_bit);
			} else {
				// Check if the corner blocks won't be fully filled with 'value'
				decltype(first_bit)	  first_block_shift = (first_bit == 0) ? 0 : 1;
				decltype(last_bit)	  last_block_shift	= (last_bit == bits_of_type - 1) ? 0 : 1;

				// Middle
				// Blocks that will be filled with ~0 or 0 at once
				decltype(first_block) first_full_block	= first_block + first_block_shift;
				decltype(last_block)  last_full_block	= last_block - last_block_shift;

				for (auto i = first_full_block; i <= last_full_block; ++i) {
					container[i] = full_block_operation(container[i]);
				}

				// Left
				// Fill the first block from the 'first' bit index to the end
				if (first_block_shift) {
					container[first_block] = partial_block_operation(container[first_block], first_bit, bits_of_type - 1);
				}

				// Right
				// Fill the last block from the start to the 'last' bit index
				if (last_block_shift) {
					container[last_block] = partial_block_operation(container[last_block], 0, last_bit);
				}
			}

			return *this;
		}

		/**
		 * @brief If size() is not a multiple of bits_per_block then not all the bits in the last block are used. This function resets the unused bits (convenient for the implementation of many member functions)
		 */
		constexpr void zero_unused_bits() noexcept(
				noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) && noexcept(std::declval<basic_dynamic_bitset>().highest_block())) {
			GAL_ASSERT(container_size() == calc_blocks_needed(total_size));

			// if != 0 this is the number of bits used in the last block
			auto extra_bits = count_extra_bits();
			if (extra_bits != 0) {
				highest_block() &= (value_type{1} << extra_bits) - 1;
			}
		}

		/**
		 * @brief Check class invariants
		 * @return is invariant
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::check_invariants, this call is meaningless overhead")]] constexpr bool check_invariants() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().count_extra_bits()) && noexcept(std::declval<basic_dynamic_bitset>().highest_block()) && noexcept(std::declval<basic_dynamic_bitset>().container.size()) && noexcept(std::declval<basic_dynamic_bitset>().container.capacity()) && noexcept(std::declval<basic_dynamic_bitset>().container_size()) && noexcept(std::declval<basic_dynamic_bitset>().size()) && noexcept(std::declval<basic_dynamic_bitset>().calc_blocks_needed(std::declval<basic_dynamic_bitset>().size()))) {
			auto extra_bits = count_extra_bits();
			if (extra_bits > 0) {
				auto mask = static_cast<value_type>(~0) << extra_bits;
				if ((highest_block() & mask) != 0) {
					return false;
				}
			}
			if (container.size() > container.capacity() || container_size() != calc_blocks_needed(size())) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Count extra bits (in last block)
		 * @return extra bits
		 */
		[[nodiscard("If you do not receive the return value of dynamic_bitset::count_extra_bits, this call is meaningless overhead")]] constexpr size_type count_extra_bits() const
				noexcept(
						noexcept(std::declval<basic_dynamic_bitset>().size()) && noexcept(std::declval<basic_dynamic_bitset>().bit_index(std::declval<basic_dynamic_bitset>().size()))) {
			return bit_index(size());
		}

		container_type container;
		size_type	   total_size;
	};

	/**
	  @brief deduction guide

	  todo:
	    `2`, `3` make `10` not work
	    `2` make `11` not work
	    `12` make `2` not work

	 */

	// 1
	basic_dynamic_bitset() -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 2
	template<std::integral T, template<typename> typename Allocator>
	basic_dynamic_bitset(const Allocator<T>&) -> basic_dynamic_bitset<std::make_unsigned_t<T>, Allocator, std::vector>;

	// 3
	template<std::integral T>
	basic_dynamic_bitset(
			typename std::vector<std::make_unsigned_t<T>, std::allocator<std::make_unsigned_t<T>>>::size_type,
			T,
			const std::allocator<std::size_t>& = {}) -> basic_dynamic_bitset<std::make_unsigned_t<T>, std::allocator, std::vector>;

	// 4
	template<typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
	basic_dynamic_bitset(
			const std::basic_string<Char, Traits, Alloc>&,
			typename std::basic_string<Char, Traits, Alloc>::size_type,
			typename std::basic_string<Char, Traits, Alloc>::size_type,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type = basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::npos,
			const std::allocator<std::size_t>&												   = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 5
	template<typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
	basic_dynamic_bitset(
			const std::basic_string<Char, Traits, Alloc>&,
			typename std::basic_string<Char, Traits, Alloc>::size_type = {},
			const std::allocator<std::size_t>&						   = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 6
	template<typename Char, typename Traits = std::char_traits<Char>>
	basic_dynamic_bitset(
			std::basic_string_view<Char, Traits>,
			typename std::basic_string_view<Char, Traits>::size_type,
			typename std::basic_string_view<Char, Traits>::size_type,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type = basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::npos,
			const std::allocator<std::size_t>&												   = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 7
	template<typename Char, typename Trait = std::char_traits<Char>>
	basic_dynamic_bitset(
			std::basic_string_view<Char, Trait>,
			typename std::basic_string_view<Char, Trait>::size_type = {},
			const std::allocator<std::size_t>&						= {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 8
	template<typename Char>
	basic_dynamic_bitset(
			const Char*,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type = basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::npos,
			const std::allocator<std::size_t>&												   = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 9
	template<typename Char>
	basic_dynamic_bitset(
			const Char* str,
			typename basic_dynamic_bitset<std::size_t, std::allocator, std::vector>::size_type,
			const std::allocator<std::size_t>& = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 10
	template<std::input_iterator ContainerInputIterator>
	basic_dynamic_bitset(
			ContainerInputIterator first,
			ContainerInputIterator last,
			const std::allocator<std::size_t>& = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 11
	template<typename Char, typename Trait = std::char_traits<Char>>
	basic_dynamic_bitset(
			std::basic_istream<Char, Trait>& is,
			const std::allocator<std::size_t>& = {}) -> basic_dynamic_bitset<std::size_t, std::allocator, std::vector>;

	// 12
	template<std::integral T>
	basic_dynamic_bitset(
			std::initializer_list<T>&& arg,
			const std::allocator<std::size_t>& = {}) -> basic_dynamic_bitset<std::make_unsigned_t<T>, std::allocator, std::vector>;

	template<typename Char, typename Trait, typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
	std::basic_ostream<Char, Trait>&
	operator<<(std::basic_ostream<Char, Trait>& os, const basic_dynamic_bitset<T, Allocator, Container>& bitset) {
		using size_type = typename basic_dynamic_bitset<T, Allocator, Container>::size_type;

		auto err		= std::ios_base::goodbit;

		if (typename std::basic_ostream<Char, Trait>::sentry{os}) {
			auto& fac		   = std::use_facet<std::ctype<Char>>(os.getloc());
			auto  zero		   = fac.widen('0');
			auto  one		   = fac.widen('1');

			auto  sz		   = bitset.size();
			auto  buf		   = os.rdbuf();
			auto  width		   = (os.width() <= 0) ? 0 : static_cast<size_type>(os.width());
			auto  pad		   = (width <= sz) ? 0 : width - sz;

			auto  fill		   = os.fill();
			auto  adjust_filed = os.flags() & std::ios_base::adjustfield;

			// if needed fill at left; pad is decreased along the way
			if (adjust_filed != std::ios_base::left) {
				for (; pad > 0; --pad) {
					if (Trait::eq_int_type(Trait::eof(), buf->sputc(fill))) {
						err |= std::ios_base::failbit;
						break;
					}
				}
			}

			if (err == std::ios::goodbit) {
				// output the bitset
				for (size_type i = sz; i > 0; --i) {
					auto ret = buf->sputc(bitset.test(i - 1) ? one : zero);
					if (Trait::eq_int_type(Trait::eof(), ret)) {
						err |= std::ios_base::failbit;
						break;
					}
				}
			}

			if (err == std::ios_base::goodbit) {
				// if needed fill at right
				for (; pad > 0; --pad) {
					if (Trait::eq_int_type(Trait::eof(), buf->sputc(fill))) {
						err |= std::ios_base::failbit;
						break;
					}
				}
			}

			os.width(0);
		}

		if (err != std::ios_base::goodbit) {
			os.setstate(err);
		}
		return os;
	}

	template<typename Char, typename Trait, typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
	std::basic_istream<Char, Trait>&
	operator>>(std::basic_istream<Char, Trait>& is, basic_dynamic_bitset<T, Allocator, Container>& bitset) {
		using size_type = typename basic_dynamic_bitset<T, Allocator, Container>::size_type;

		auto width		= is.width();
		auto limit		= (width > 0 && static_cast<size_type>(width) < bitset.max_size()) ? static_cast<size_type>(width) : bitset.max_size();

		auto err		= std::ios_base::goodbit;

		if (typename std::basic_istream<Char, Trait>::sentry{is}) {
			auto& fac  = std::use_facet<std::ctype<Char>>(is.getloc());
			auto  zero = fac.widen('0');
			auto  one  = fac.widen('1');

			auto  buf  = is.rdbuf();
			auto  i	   = buf->sgetc();
			while (bitset.size() < limit) {
				if (Trait::eq_int_type(Trait::eof(), i)) {
					err |= std::ios_base::eofbit;
					break;
				} else {
					auto c		= Trait::to_char_type(i);

					auto is_one = Trait::eq(c, one);

					// check valid
					if (!is_one && !Trait::eq(c, zero)) {
						break;
					}

					bitset.push_back(is_one);
				}
				i = buf->snextc();
			}

			is.width(0);
		}

		if (bitset.empty()) {
			err |= std::ios_base::failbit;
		}
		if (err != std::ios_base::goodbit) {
			is.setstate(err);
		}
		return is;
	}

}// namespace gal::toolbox::dynamic_bitset

namespace std {
	template<typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
	struct iterator_traits<gal::toolbox::dynamic_bitset::detail::bit_it<T, Allocator, Container>>
		: public gal::toolbox::dynamic_bitset::detail::bit_it<T, Allocator, Container> {};
}// namespace std

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_HPP
