#pragma once

#include "mtl/iterator.hpp"
#include "mtl/maybe.hpp"
#include "mtl/pointers.hpp"
#include "mtl/types.hpp"

namespace mtl {

  template <class ValueType>
  class LinkedList {
    struct Node {
      ValueType value;
      OwningPtr<Node> next = nullptr;
    };

  public:
    void push_back(ValueType&& value) {
      if (head_.is_none()) {
        // Create the head with the given value
        std::swap(head_, {std::forward<ValueType>(value), nullptr});
        return;
      }
      last_element_ref(head_.unsafe_get_some()).next
          = make_owned<Node>({std::forward<ValueType>(value), nullptr});
    }

    void push_back(const ValueType& value) {
      if (head_.is_none()) {
        // Create the head with the given value
        std::swap(head_, {value, nullptr});
        return;
      }

      last_element_ref(head_.unsafe_get_some()).next
          = make_owned<Node>({std::forward<ValueType>(value), nullptr});
    }

    /// @brief linear lookup of size
    usize size() const {
      if (head_.is_none()) {
        return 0;
      }
      usize size = 1;
      auto cur_node = head_;
      while (cur_node.next != nullptr) {
        // keep moving until we get to the end
        cur_node = cur_node.next;
        ++size;
      }
      return size;
    }

  private:
    Maybe<Node> head_ = None{};

    // Used for traversing to the end of the list
    static Node& last_element_ref(Node& start) {
      auto cur_node = start;
      while (cur_node.next != nullptr) {
        // keep moving until we get to the end
        cur_node = cur_node.next;
      }
      return cur_node;
    }
  };
}  // namespace mtl