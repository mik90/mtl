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
* `algorithm`
    - [ ] helpers like map, reduce, and fold
        - [ ] possibly add parallelization?
* `maybe`
    - [x] make `mtl::maybe` (similar to `std::optional`) 
    - has the same issues as `result` although less numerous
    - used union and tagging, may not be sufficient
        - this wont handle reference types
    - reading:
        - optional impl `https://github.com/akrzemi1/Optional/blob/master/optional.hpp`
        - `http://www.club.cc.cmu.edu/~ajo/disseminate/2017-02-15-Optional-From-Scratch.pdf`
        - `https://ojdip.net/2013/10/implementing-a-variant-type-in-cpp/`
            - should i avoid dynamic memory allocation or no? I lean towards avoiding it
        - `https://www.bfilipek.com/2018/05/using-optional.html`
            - std::aligned_storage or std::aligned_union
* `containers`
    - static array
      - started on this
      - [x] helper like `std::make_array` that handles the size template parameter
      - std::array doens't actually have a constructor, it's implicitly declared
        - can i do this? Not really, a std::array's size cannot change and is embedded in its type
    - dynamic array
        - impl using a static array
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
    - iterator
      - should be interchangeable with std iterators
* `string`
    - utf8 text
    - no null termination
* benchmarks that compare against performance of std:: and other libraries
* Also add on a tiny neural net lib based on https://github.com/codeplea/genann because why not