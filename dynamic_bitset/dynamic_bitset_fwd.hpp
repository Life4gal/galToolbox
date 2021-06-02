#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP

#include <concepts>
#include <vector>

namespace gal::toolbox::dynamic_bitset {
	template<std::integral T, template<typename> typename Allocator, template<typename, typename> typename Container>
	requires
	requires {
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
	class basic_dynamic_bitset;

	template<typename T = std::size_t>
	using dynamic_bitset = basic_dynamic_bitset<T, std::allocator, std::vector>;
}// namespace gal::toolbox::dynamic_bitset

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
