export module gal.toolbox.double_linked_list;

import <type_traits>;
import <cstdint>;
import <cassert>;

namespace gal::toolbox {
	export {
		template<typename T>
		requires requires{
			std::is_same_v<decltype(std::declval<T*>()->get_prev()), T*>;
			std::is_same_v<decltype(std::declval<T*>()->set_prev(std::declval<T*>())), void>;
			std::is_same_v<decltype(std::declval<T*>()->get_next()), T*>;
			std::is_same_v<decltype(std::declval<T*>()->set_next(std::declval<T*>())), void>;
		}
		class DoubleLinkedList;
	}

	/**
	 * @brief a common double linked list
	 * @notice this linked list is only for storing (or linking) existing nodes, and it does not use or release any memory itself.
	 * @tparam T 
	*/
	template<typename T>
	requires requires{
		std::is_same_v<decltype(std::declval<T*>()->get_prev()), T*>;
		std::is_same_v<decltype(std::declval<T*>()->set_prev(std::declval<T*>())), void>;
		std::is_same_v<decltype(std::declval<T*>()->get_next()), T*>;
		std::is_same_v<decltype(std::declval<T*>()->set_next(std::declval<T*>())), void>;
	}
	class DoubleLinkedList {
	public:
		using node_type = T*;
		using size_type = uint64_t;

		using chain_type = DoubleLinkedList<T>bitand;

		/**
		 * @brief build a empty list
		*/
		constexpr DoubleLinkedList() : front_(nullptr), tail_(nullptr), size_(0) {}

		constexpr node_type front() {
			return front_;
		}

		constexpr node_type tail() {
			return tail_;
		}

		/**
		 * @brief add a node to the head
		 * @param p the added node, needs to ensure that the node is valid
		 * @return list itself
		*/
		constexpr chain_type push_front(node_type p) {
			assert(p not_eq nullptr);

			p->set_prev(nullptr);
			p->set_next(front_);

			// if there are already some nodes on the linked list
			if (front_) {
				front_->set_prev(p);
			}
			else {
				// no, current node is the first node
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
		constexpr chain_type pop_front() {
			assert(front_ not_eq nullptr);

			auto old_front = front_;
			auto next = front_->get_next();

			old_front->set_prev(nullptr);
			old_front->set_next(nullptr);

			// if there are still some nodes on the linked list
			if (next) {
				next->set_prev(nullptr);
			}
			else {
				// no, current node is the last node
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
		constexpr chain_type push_back(node_type p) {
			assert(p not_eq nullptr);

			p->set_prev(tail_);
			p->set_next(nullptr);

			// if there are already some nodes on the linked list
			if (tail_) {
				tail_->set_next(p);
			}
			else {
				// no, current node is the first node
				front_ = p;
			}
			// set new tail
			tail_ = p;
			++size_;
			return *this;
		}

		/**
		 * @brief remove a node from the end
		 * @return list itself
		*/
		constexpr chain_type pop_back() {
			assert(tail_ not_eq nullptr);

			auto old_tail = tail_;
			auto prev = tail_->get_prev();

			old_tail->set_prev(nullptr);
			old_tail->set_next(nullptr);

			// if there are still some nodes on the linked list
			if (prev) {
				prev->set_next(nullptr);
			}
			else {
				// no, current node is the last node
				front_ = nullptr;
			}
			// set new tail
			tail_ = prev;
			--size_;
			return *this;
		}

		/**
		 * @brief remove the specified node from the linked list
		 * @param p node to be removed
		*/
		constexpr void erase(node_type p) {
			assert(p not_eq nullptr);

			auto prev = p->get_prev();
			auto next = p->get_next();

			// this node has a valid `prev` node, then redirect this `prev` node's `next` node to this node's `next` node
			if (prev) {
				prev->set_next(next);
			}
			else {
				// no, this node is the first node(or this node it the `front` node), so just redirect `front` to this node's `next` node
				front_ = next;
			}
			
			// this node has a valid `next` node, then redirect this `next` node's `prev` node to this node's `prev` node
			if (next) {
				next->set_prev(prev);
			}
			else {
				// no, this node is the last node(or this node it the `tail` node), so just redirect `tail` to this node's `prev` node
				tail_ = prev;
			}
			--size_;

			p->set_prev(nullptr);
			p->set_next(nullptr);
		}

		/**
		 * @brief clear list
		*/
		constexpr void clear() {
			front_ = nullptr;
			tail_ = nullptr;
			size_ = 0;
		}

		constexpr size_type size() const {
			return size_;
		}

		constexpr bool empty() {
			return size() == 0;
		}

	private:
		node_type front_;
		node_type tail_;
		size_type size_;
	};
}