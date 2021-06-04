#include <benchmark/benchmark.h>

static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        const int i = 2 + 3;
        benchmark::DoNotOptimize(i);
    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);