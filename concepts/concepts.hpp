#ifndef GAL_TOOLBOX_CONCEPT_HPP
#define GAL_TOOLBOX_CONCEPT_HPP

#include <concepts>
#include <type_traits>

namespace gal::toolbox::concepts {

	namespace container {
		template<typename Container>
		concept is_standard_container = requires {
			typename Container::value_type;
			typename Container::pointer;
			typename Container::reference;
			typename Container::iterator;
			typename Container::reverse_iterator;
			typename Container::size_type;
			typename Container::difference_type;
		};

		template<typename Container>
		concept has_mem_func_size = requires(Container c) {
			c.size();
		};

		template<typename Container>
		concept is_iterable = requires(Container c) {
			c.begin();
			c.end();
		};

		template<typename Container>
		concept is_reverse_iterable = requires(Container c) {
			c.rbegin();
			c.rend();
		};

		template<typename Container>
		concept is_indexable = requires(Container c) {
			c[0];
		};

		template<typename Container>
		concept is_resizable = requires(Container c) {
			c.resize(0);
		};

		template<typename T, template<typename...> typename Container>
		concept is_resizable_and_settable = requires(Container<T> c) {
			c.resize(0, T{});
		};

		namespace iterator {
			template<typename ContainerIterator>
			concept is_standard_iterator = requires {
				typename ContainerIterator::iterator_type;
				typename ContainerIterator::iterator_category;
				typename ContainerIterator::value_type;
				typename ContainerIterator::difference_type;
				typename ContainerIterator::reference;
				typename ContainerIterator::pointer;
				typename ContainerIterator::iterator_concept;
			};

			template<typename ContainerIterator>
			concept has_mem_func_dereference = requires(ContainerIterator c) {
				c.operator*();
			};
		}// namespace iterator

	}// namespace container

	namespace reserve {

	}

};// namespace gal::toolbox::concepts

#endif//GAL_TOOLBOX_CONCEPT_HPP
