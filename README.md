# mtl
A C++ template library
Includes fork of https://github.com/mik90/wrapped_var
For usage, all that's needed is the include/mtl directory

## Build and run tests
`mkdir build && cd build`
`cmake .. -DENABLE_TESTING=ON`
`cmake --build UnitTest`
`./bin/UnitTest`

## Build and run benchmarks
`mkdir build && cd build`
`cmake .. -DENABLE_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release`
`cmake --build Benchmark`
`./bin/Benchmark`

# Tasklist

## mtl
### result
- [x] Allow for move-only types
- [x] impl rough minimal variant
### memoize
- [ ] memoization wrapper that stores results of a function
### `algorithm`
- algorithmic helpers
- [x] map,
- [ ] reduce
- [ ] fold
- [ ] add parallelization option
### `pointers`
- [x] `OwnedPtr` - similar to `unique_ptr`
- [ ] `SharedPtr` - `shared_ptr`
- [ ] `NonOwnedPtr` - `weak_ptr`
### `maybe`
- [x] make `mtl::Maybe` (similar to `std::optional`) 
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
### static array
- started on this
- [x] helper like `std::make_array` that handles the size template parameter
- std::array doens't actually have a constructor, it's implicitly declared
- can i do this? Not really, a std::array's size cannot change and is embedded in its type
- [ ] finish up `remove_at`
- [ ] create template specialization that makes it easier to have it as a member func
    - don't include size and allow for resize
### dynamic array
- [ ] impl using a static array
### hash map
- [ ] start
### tree-based map
- [ ] start
### set
- [ ] start
### linked list
- [ ] start
### tree
- [ ] start
### graph
- [ ] acyclic
- [ ] directed acyclic
- [ ] cyclic
### iterator
- [ ] should be interchangeable with std iterators
### string
- utf8 text
- no null termination
### benchmarks that compare against performance of std:: and other libraries
- [x] cmake setup
- [ ] add third party libraries for comparison

## nn
A tiny neural net lib based on https://github.com/codeplea/genann because why not
- [ ] base impl