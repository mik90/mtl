# mtl
A C++ template library
Includes fork of https://github.com/mik90/wrapped_var

## Build and run tests
`mkdir build && cd build`
`cmake .. -DENABLE_TESTING=ON`
`cmake --build UnitTest`
`./bin/UnitTest`

# Tasklist
* `result`
    - [ ] Allow for move-only types
    - [ ] Impl `mtl::variant` that avoids throwing
* `memoize`
    - [ ] memoization wrapper that stores results of a function
* `maybe`
    - make `mtl::maybe` (similar to `std::optional`) 
* `containers`
    - static array
    - dynamic array
    - hash map
    - set
    - linked list
    - tree
    - graph
        - What kinds?
            - acyclic
            - directed acyclic
            - cyclic
