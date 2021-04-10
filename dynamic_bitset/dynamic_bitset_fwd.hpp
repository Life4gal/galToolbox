#ifndef GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
#define GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP

#if __has_include(<vector>)
#ifndef GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER
#define GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER std::vector
#endif
#endif

namespace gal::toolbox::dynamic_bitset {
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
	class basic_dynamic_bitset;

	template<typename T = unsigned int, typename Allocator = std::allocator<unsigned int>, template<typename, typename> typename Container = GAL_DYNAMIC_BITSET_DEFAULT_CONTAINER>
	using dynamic_bitset = basic_dynamic_bitset<T, Allocator, Container>;
}// namespace gal::toolbox::dynamic_bitset

#endif//GAL_TOOLBOX_DYNAMIC_BITSET_FWD_HPP
