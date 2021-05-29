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
* `either`
    - like std::variant but only for two types
    - it'll make `result` implementation easier
    - [ ] `~either` should only be explicitly defined if either FirstType or SecondType are non-trivially destructable
    - [ ] `either` should be constructable from non-rvalue types, especially if it's a primative
    ```
[build] ../test/either_test.cpp:71:59: error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘CopyableType’ {aka ‘int’}
[build]    71 |     auto either = mtl::either<CopyableType, MoveOnlyType>(dummy_copyable_value);
    ```
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
* `string`
    - utf8 text
    - null terminated, i guess
* benchmarks that compare against performance of std:: and other libraries