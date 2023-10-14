#pragma once

namespace mtl {
  /// @brief Apply a function to each of the elements between two iterators
  template <class Iterator, class UnaryFunc>
  void for_each(Iterator begin, Iterator end, UnaryFunc f) {
    if (end < begin) {
      // Skip if iterator ordering is bogus
      return;
    }
    for (auto cur_it = begin; cur_it < end; ++cur_it) {
      // update value with return value from f
      *cur_it = f(*cur_it);
    }
  }

  /// @brief Apply a function to all elements in a container
  template <class Container, class UnaryFunc> void for_each(Container& container, UnaryFunc f) {
    auto begin = container.begin();
    auto end = container.end();

    for_each(begin, end, f);
  }

  /// @tparam Container must be copyable
  /// @tparam UnaryFunc must return a value the same as the type of an element in Container. The
  /// element is const.
  template <class Container, class UnaryFunc>
  Container map(const Container& container, UnaryFunc f) {
    Container new_container;
    for (const auto& elem : container) {
      new_container.emplace_back(f(elem));
    }
    return new_container;
  }

}  // namespace mtl