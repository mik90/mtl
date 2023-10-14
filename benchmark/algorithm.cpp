#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <vector>

#include "mtl/dyn_array.hpp"
#include "mtl/static_array.hpp"

/*
static void map_array(benchmark::State& state) {
  for (auto _ : state) {
    std::array<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
    mtl::map(arr, [](auto& val) { return ++val; });
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(map_array);

static void transform_array(benchmark::State& state) {
  for (auto _ : state) {
    std::array<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
    std::transform(arr, [](auto& val) { return ++val; });
    benchmark::DoNotOptimize(arr[3]);
  }
}
BENCHMARK(transform_array);
*/