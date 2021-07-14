export module gal.toolbox.linked_list;

import <type_traits>;
import <cstdint>;
import gal.toolbox.assert;

namespace gal::toolbox {
	export {
		template<typename T>
		requires requires{
			std::is_same_v<decltype(std::declval<T*>()->get_next()), T*>;
			std::is_same_v<decltype(std::declval<T*>()->set_next(std::declval<T*>())), void>;
		}
		class LinkedList;
	}

	/**
	 * @brief a common linked list
	 * @notice this linked list is only for storing (or linking) existing nodes, and it does not use or release any memory itself.
	 * @tparam T 
	*/
	template<typename T>
	requires requires{
		std::is_same_v<decltype(std::declval<T*>()->get_next()), T*>;
		std::is_same_v<decltype(std::declval<T*>()->set_next(std::declval<T*>())), void>;
	}
	class LinkedList {
	public:
		using node_type = T*;
		using size_type = uint64_t;

		using chain_type = LinkedList<T> bitand;

		/**
		 * @brief build a empty list
		*/
		constexpr LinkedList() noexcept(noexcept(std::is_nothrow_constructible_v<node_type, nullptr_t>)) : front_(nullptr), tail_(nullptr), size_(0) {}

		constexpr node_type front() noexcept(noexcept(std::is_nothrow_copy_constructible_v<node_type>)) {
			return front_;
		}

		constexpr node_type front() const noexcept(noexcept(std::is_nothrow_copy_constructible_v<node_type>)) {
			return front_;
		}

		constexpr node_type tail() noexcept(noexcept(std::is_nothrow_copy_constructible_v<node_type>)) {
			return tail_;
		}

		constexpr node_type tail() const noexcept(noexcept(std::is_nothrow_copy_constructible_v<node_type>)) {
			return tail_;
		}

		/**
		 * @brief add a node to the head
		 * @param p the added node, needs to ensure that the node is valid
		 * @return list itself
		*/
		constexpr chain_type push_front(node_type p) 
			noexcept(
				noexcept(std::declval<node_type>()->set_next(std::declval<node_type>())) and
				noexcept(std::is_nothrow_copy_assignable_v<node_type>)
			) {
			gal::toolbox::assert(p not_eq nullptr);

			p->set_next(front_);

			// no node exist
			if (not tail_) {
				tail_ = p;
			}

			// set new front
			front_ = p;
			++size_;
			return *this;
		}

		/**
		 * @brief remove a node from the head
		 * @return list itself
		*/
		constexpr chain_type pop_front() 
			noexcept(
				noexcept(std::declval<node_type>()->set_next(std::declval<node_type>())) and
				noexcept(std::is_nothrow_copy_assignable_v<node_type>) and
				noexcept(std::declval<node_type>() = std::declval<node_type>()->get_next())
			) {
			gal::toolbox::assert(front_ not_eq nullptr);

			auto old_front = front_;
			auto next = front_->get_next();

			old_front->set_next(nullptr);

			if (not next) {
				// current node is the last node
				tail_ = nullptr;
			}
			// set new front
			front_ = next;
			--size;
			return *this;
		}

		/**
		 * @brief add a node to the end
		 * @param p the added node, needs to ensure that the node is valid
		 * @return list itself
		*/
		constexpr chain_type push_back(node_type p) 
			noexcept(
				noexcept(std::declval<LinkedList<T>>().push_front(std::declval<node_type>())) and
				noexcept(std::declval<node_type>()->set_next(std::declval<node_type>())) and
				noexcept(std::is_nothrow_copy_assignable_v<node_type>)
			) {
			gal::toolbox::assert(p not_eq nullptr);

			// no node exist
			if (not tail_) {
				return push_front(p);
			}

			tail_->set_next(p);
			p->set_next(nullptr);

			// set new tail
			tail_ = p;
			++size_;
			return *this;
		}

		/**
		 * @brief clear list
		*/
		constexpr void clear() noexcept {
			front_ = nullptr;
			tail_ = nullptr;
			size_ = 0;
		}

		constexpr size_type size() const noexcept {
			return size_;
		}

		constexpr bool empty() const noexcept {
			return size() == 0;
		}

	private:
		node_type front_;
		node_type tail_;
		size_type size_;
	};
}