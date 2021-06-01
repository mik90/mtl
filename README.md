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
    - [x] impl rough minimal variant
        - used union and tagging, may not be sufficient
            -this wont handle reference types
        - `https://ojdip.net/2013/10/implementing-a-variant-type-in-cpp/`
            - should i go for even even lower layer?
* `memoize`
    - [ ] memoization wrapper that stores results of a function
* `maybe`
    - [x] make `mtl::maybe` (similar to `std::optional`) 
    - has the same issues as `result` although less numerous
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
* `string`
    - utf8 text
    - no null termination
* benchmarks that compare against performance of std:: and other libraries