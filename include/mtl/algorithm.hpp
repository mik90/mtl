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
template <class Container, class UnaryFunc>
void for_each(Container container, UnaryFunc f) {
    auto begin = container.begin();
    auto end = container.end();

    for_each(begin, end, f);
}
} // namespace mtl