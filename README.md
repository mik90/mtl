# mtl

A basic C++ template library

For usage, all that's needed is the include/mtl directory

I use this as a playground for C++ and development environments.

## Features

### WrappedVar wraps a variable with a mutex

```cpp
mtl::WrappedVar<int> wrapped_int{3};
auto var_with_lock = wrapped_int.get();
ASSERT_EQ(var_with_lock.get_ref(), 3);
```

### DynArray that must be explicitly copied

```cpp
const auto arr = mtl::DynArray<int>({0, 1, 2, 3});
auto new_arr = arr.copy();
```

### MoveOnly which can live on the stack and can only be moved

Useful for when you want the semantics of a `std::unique_ptr` but without the heap allocation

```cpp
mtl::MoveOnly<std::string> myString{"hello"};
ASSERT_EQ(*myString, "hello");
```

## Build and run tests

```bash
bazel test //...
```

## Build and run benchmarks

```bash
bazel run -c opt //benchmark:array
```

## Generate compile_commands.json

```bash
bazel run @hedron_compile_commands//:refresh_all
```

## Tasklist

### General stuff

- [ ] Build/test with sanitizers enabled

### result

- [x] Allow for move-only types
- [x] impl rough minimal variant
- [ ] Find a way to make it easier to return mtl::Error as an mtl::Err

### memoize

- [ ] memoization wrapper that caches results of a function

### algorithm

- algorithmic helpers
- [x] for each
- [x] map
- [ ] reduce
- [ ] fold
- [ ] add parallelization option

### pointers

- [x] `OwningPtr` - similar to `unique_ptr`
- [ ] `SharedPtr` - `shared_ptr`
- [ ] `NonOwningPtr` - `weak_ptr`

### maybe

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
- [x] finish up `remove_at`

### dynamic array

- [x] impl using a mtl::OwningPtr
- [x] deal with segfault in `DynArrayTest.default_capacity`
  - CTest lists out the test segfault
- [x] Allow for copy construction
  - done with `copy()`

### hash map

- [x] start
- [x] Look in here for something to impl `https://en.wikipedia.org/wiki/List_of_hash_functions`
  - probably non-cryptographic hash functions
  - gonna use jenkins's one_at_a_time function
- [ ] read into <https://abseil.io/docs/cpp/guides/container#hash-tables>
- [ ] impl other hashes and test em

### iterator

- [x] start
- Should i merge ConstIterator and Iterator?
- [ ] should be interchangeable with std iterators
- impl with newer concepts <https://en.cppreference.com/w/cpp/iterator/random_access_iterator>

### tree-based map

- [ ] start
- B-tree or Red-Black map

### set

- [ ] start

### hash set

- [ ] start

### linked list

- [ ] start

### tree

- [ ] start

### graph

- [ ] acyclic
- [ ] directed acyclic
- [ ] cyclic

### string

- utf8 text
  - it may be easiest to start with ASCII
- [ ] ConstantString
  - [ ] user defined literal
  - `std::string_view` but null-termintaes

### benchmarks

- [x] cmake setup
- [ ] test mtl algorithm against std algorithms to ensure it isn't worse
- [ ] add third party libraries for comparison
