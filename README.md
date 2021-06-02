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
    - [x] Allow for move-only types
    - [x] impl rough minimal variant
* `memoize`
    - [ ] memoization wrapper that stores results of a function
* `maybe`
    - [x] make `mtl::maybe` (similar to `std::optional`) 
    - has the same issues as `result` although less numerous
    - used union and tagging, may not be sufficient
        -this wont handle reference types
    - reading:
        - optional impl `https://github.com/akrzemi1/Optional/blob/master/optional.hpp`
        - `http://www.club.cc.cmu.edu/~ajo/disseminate/2017-02-15-Optional-From-Scratch.pdf`
        - `https://ojdip.net/2013/10/implementing-a-variant-type-in-cpp/`
            - should i avoid dynamic memory allocation or no? I lean towards avoiding it
        - `https://www.bfilipek.com/2018/05/using-optional.html`
            - std::aligned_storage or std::aligned_union
* `containers`
    - static array
    - dynamic array
    - hash map
    - tree-based map
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