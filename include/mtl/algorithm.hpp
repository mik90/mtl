#pragma once

namespace mtl {
    /// @brief Map a function to all elements between begin and end
    template<class Iterator, class FuncReturningUpdatedValue>
    void map(Iterator begin, Iterator end, FuncReturningUpdatedValue f) {
        if (end < begin) {
            // Skip if iterator ordering is bogus
            return;
        }
        for (auto cur_it = begin; cur_it < end; ++cur_it) {
            // update value with return value from f 
            *cur_it = f(*cur_it);
        }
    }
    
    /// @brief Map a function to all elements in a container
    template<class Container, class FuncReturningUpdatedValue>
    void map(Container container, FuncReturningUpdatedValue f) {
        auto begin = container.begin();
        auto end = container.end();

        map(begin, end, f);
    }
}