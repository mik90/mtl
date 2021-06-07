#include <benchmark/benchmark.h>
#include "mtl/static_array.hpp"
#include <array>

static void std_array_init_list(benchmark::State& state) {
    for (auto _ : state) {
        std::array<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
        benchmark::DoNotOptimize(arr[3]);
    }
}
BENCHMARK(std_array_init_list);

static void mtl_static_array_init_list(benchmark::State& state) {
    for (auto _ : state) {
        mtl::StaticArray<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
        benchmark::DoNotOptimize(arr[3]);
    }
}
BENCHMARK(mtl_static_array_init_list);
