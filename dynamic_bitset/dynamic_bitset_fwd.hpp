#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP

#include <concepts>

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

#if defined(GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER)
	#if defined(GAL_DYNAMIC_BITSET_DEFAULT_ALLOCATOR)
		template<typename T = GAL_DYNAMIC_BITSET_DEFAULT_TYPE>
		using dynamic_bitset = basic_dynamic_bitset<T, GAL_DYNAMIC_BITSET_DEFAULT_ALLOCATOR, GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER>;
	#else
		template<typename T, template<typename> typename Allocator>
		using dynamic_bitset = basic_dynamic_bitset<T, Allocator, GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER>;
	#endif
#else
	template<typename T, template<typename> typename Allocator, template<typename, typename> typename Container>
	using dynamic_bitset = basic_dynamic_bitset<T, Allocator, Container>;
#endif
}// namespace gal::toolbox::dynamic_bitset

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
