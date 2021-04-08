#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_HPP

#include <concepts/concepts.hpp>
#include <limits>

namespace gal::toolbox::dynamic_bitset {
	using size_t = std::size_t;
	using namespace gal::toolbox::concepts::container;

	namespace detail {
		/*
		 * normally, we use unsigned int to store data
		 * no sign bit
		 */
		template<typename T>
		requires std::unsigned_integral<T> [[nodiscard("Meaningless overhead")]] constexpr size_t bits_of_type() {
			// sizeof(T) * CHAR_BIT;
			return std::numeric_limits<T>::digits;
		}

		/*
		 * A reference for a single bit
		 */
		template<typename T>
		requires std::unsigned_integral<T> class bit_ref {
		public:
			using value_type = T;

			constexpr bit_ref(value_type& value, size_t offset) noexcept
				: value(value),
				  offset(offset) {}

			/*
			 * cast this bit to a boolean
			 */
			[[nodiscard("Meaningless overhead")]] constexpr explicit operator bool() const noexcept {
				return (value >> offset) & value_type{1};
			}

			/*
			 * set this bit to a new value
			 */
			bit_ref& operator=(bool new_value) noexcept {
				value = (value & ~(value_type{1} << offset)) | (value_type{new_value} << offset);
				return *this;
			}

			/*
			 * set this bit by another bit_ref's boolean value
			 */
			bit_ref& operator=(const bit_ref& new_value) noexcept {
				this->operator=(new_value.operator bool());
				return *this;
			}

			[[nodiscard("Meaningless overhead")]] constexpr bool operator==(const bit_ref& other) const noexcept {
				return value == other.value && offset == other.offset;
			}

			[[nodiscard("Meaningless overhead")]] constexpr bool operator==(bool v) const noexcept {
				return this->operator bool() == v;
			}

			/*
			 * swaps only the referenced bit
			 */
			template<typename U>
			constexpr void swap(bit_ref<U>& other) {
				// can not use std::exchange because 'value' is a 'ref' and 'old_obj' will be changed if 'this' has been changed
				// other = std::exchange(*this, other).operator bool();
				auto t = this->operator bool();
				this->operator=(other);
				other = t;
			}

			//		private:
			// the value to reference
			T& value;
			// current offset
			size_t offset;
		};

		template<typename ContainerIterator>
		requires std::random_access_iterator<ContainerIterator>&& iterator::is_standard_iterator<ContainerIterator>&& iterator::has_mem_func_dereference<ContainerIterator> class bit_it {
			using real_value_type = typename ContainerIterator::value_type;
			constexpr static auto type_bit_size = bits_of_type<real_value_type>();

		public:
			using iterator_type = ContainerIterator;
			using iterator_category = typename iterator_type::iterator_category;
			using difference_type = typename iterator_type::difference_type;

			using value_type = bit_ref<real_value_type>;
			using reference = bit_ref<real_value_type>;

			constexpr bit_it(iterator_type it, size_t offset)
				: iterator(it),
				  index(offset) {}

			[[nodiscard("If you don't care about the return value, you should use prefix ++ like ++bit_it.")]] bit_it operator++(int) noexcept {
				auto copy{*this};
				++index;
				return copy;
			}

			bit_it& operator++() noexcept {
				++index;
				return *this;
			}

			[[nodiscard("If you don't care about the return value, you should use prefix -- like --bit_it.")]] bit_it operator--(int) noexcept {
				auto copy{*this};
				--index;
				return copy;
			}

			bit_it& operator--() noexcept {
				++index;
				return *this;
			}

			[[nodiscard("This does not change the object itself, so you should not discard the return value.")]] bit_it operator+(size_t n) const noexcept {
				return {iterator, index + n};
			}

			[[nodiscard("This does not change the object itself, so you should not discard the return value.")]] bit_it operator-(size_t n) const noexcept {
				return {iterator, index - n};
			}

			bit_it& operator+=(size_t n) noexcept {
				index += n;
				return *this;
			}

			/*
			 * better make sure n <= index
			 */
			bit_it& operator-=(size_t n) noexcept {
				index -= n;
				return *this;
			}

			/*
			 * better make sure the given bit_it is in front of this
			 */
			[[nodiscard("Meaningless overhead")]] size_t distance(const bit_it& other) const noexcept {
				return index - other.index;
			}

			[[nodiscard("Meaningless overhead")]] constexpr bool operator==(const bit_it& other) const noexcept {
				return iterator == other.iterator && index == other.index;
			}

			[[nodiscard("Meaningless overhead")]] constexpr bool operator!=(const bit_it& other) const noexcept {
				return !this->operator==(other);
			}

			[[nodiscard("Meaningless overhead")]] constexpr value_type operator*() const noexcept {
				return {(iterator + index / type_bit_size).operator*(), index % type_bit_size};
			}

		private:
			iterator_type iterator;
			size_t index;
		};
	}// namespace detail

	/*
	 * hold a real container and manage it's operations
	 */
	template<typename T, template<typename> typename Container>
	requires std::is_default_constructible_v<Container<T>>&& std::is_copy_constructible_v<Container<T>>&& std::is_move_constructible_v<Container<T>>&&
			is_indexable<Container<T>>&& has_mem_func_size<Container<T>>&& is_resizable_and_settable<T, Container>&& is_iterable<Container<T>> class dynamic_bitset {
		using real_value_type = T;
		constexpr static auto type_bit_size = detail::bits_of_type<real_value_type>();

	public:
		using container_type = Container<T>;
		using iterator_type = typename detail::bit_it<decltype(std::declval<Container<T>>().begin())>;

		using value_type = typename iterator_type::value_type;
		using reference = typename iterator_type::reference;

	public:
		constexpr dynamic_bitset() noexcept(std::is_nothrow_default_constructible_v<container_type>)
			: container() {}

		constexpr dynamic_bitset(const dynamic_bitset& other) noexcept(std::is_nothrow_copy_constructible_v<container_type>)
			: container(other.data()) {}

		constexpr dynamic_bitset(dynamic_bitset&& other) noexcept(std::is_nothrow_move_constructible_v<container_type>)
			: container(std::move(other.data())) {}

		constexpr dynamic_bitset(std::initializer_list<real_value_type>&& args) noexcept(std::is_nothrow_constructible_v<container_type, std::initializer_list<real_value_type>>) : container(args) {}

		constexpr explicit dynamic_bitset(size_t n) noexcept(std::is_nothrow_constructible_v<container_type, size_t>) : container(n) {}

		constexpr dynamic_bitset(size_t n, real_value_type value) noexcept(std::is_nothrow_constructible_v<container_type, size_t, real_value_type>) : container(n, value) {}

		/*
		 * maybe this won't work :)
		 */
		template<typename... Args>
		requires std::is_constructible_v<container_type, Args...> constexpr explicit dynamic_bitset(Args&&... args) noexcept(std::is_nothrow_constructible_v<container_type, Args...>)
			: container(std::forward<Args...>(args...)) {}

		/*
		* returns the number of bits that fit in the container
		*/
		[[nodiscard("Meaningless overhead")]] size_t size() const noexcept(noexcept(container.size())) {
			return container.size() * type_bit_size;
		}

		/*
		* resizes the container to hold at least `N` bits.
		* Note that the actual size will be a multiple of the bits in a word.
		*/
		void resize(size_t n, bool value = false) noexcept(noexcept(container.resize(0, 0))) {
			real_value_type offset = n % type_bit_size == 0 ? 0 : 1;
			container.resize(n / type_bit_size + offset, value ? ~0 : 0);
		}

		/*
		* returns a BitReference to the bit at position `i`.
		*/
		[[nodiscard("Meaningless overhead")]] constexpr value_type operator[](size_t i) noexcept(noexcept(container[0])) {
			return {container[i / type_bit_size], i % type_bit_size};
		}

		[[nodiscard("Meaningless overhead")]] constexpr value_type operator[](size_t i) const noexcept(noexcept(container[0])) {
			return {container[i / type_bit_size], i % type_bit_size};
		}

		[[nodiscard("Meaningless overhead")]] constexpr iterator_type begin() noexcept(noexcept(container.begin())) {
			return {container.begin(), 0};
		}

		[[nodiscard("Meaningless overhead")]] constexpr iterator_type begin() const noexcept(noexcept(container.begin())) {
			return iterator_type{container.begin(), 0};
		}

		[[nodiscard("Meaningless overhead")]] constexpr iterator_type end() noexcept(noexcept(container.end())) {
			return {container.begin(), size()};
		}

		[[nodiscard("Meaningless overhead")]] constexpr iterator_type end() const noexcept(noexcept(container.end())) {
			return {container.begin(), size()};
		}

		/*
		* get the underlying data container
		*/
		[[nodiscard("Meaningless overhead")]] container_type& data() noexcept {
			return container;
		}

		[[nodiscard("Meaningless overhead")]] container_type& data() const noexcept {
			return container;
		}

	private:
		container_type container;
	};

}// namespace gal::toolbox::dynamic_bitset

namespace std {
	template<typename T>
	struct iterator_traits<gal::toolbox::dynamic_bitset::detail::bit_it<T>>
		: public gal::toolbox::dynamic_bitset::detail::bit_it<T> {};
}// namespace std

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_HPP
