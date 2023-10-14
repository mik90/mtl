#include <benchmark/benchmark.h>

#include <array>
#include <vector>

#include "mtl/dyn_array.hpp"
#include "mtl/static_array.hpp"

static void std_array_init_list(benchmark::State& state) {
  for (auto _ : state) {
    std::array<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(std_array_init_list);

static void std_vector_init_list(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> arr = {0, 1, 2, 3, 4, 5, 6};
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(std_vector_init_list);

static void mtl_static_array_init_list(benchmark::State& state) {
  for (auto _ : state) {
    mtl::StaticArray<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(mtl_static_array_init_list);

static void mtl_dyn_array_init_list(benchmark::State& state) {
  for (auto _ : state) {
    mtl::DynArray<int> arr = {0, 1, 2, 3, 4, 5, 6};
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(mtl_dyn_array_init_list);