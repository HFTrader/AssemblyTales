#include "functions.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <iostream>

struct TrivialFn {
    void operator()(int64_t* ptr, double freq, int N) {
        trivial(ptr, freq, N);
    }
};

struct CordicFn {
    void operator()(int64_t* ptr, double freq, int N) {
        cordic(ptr, freq, N);
    }
};
struct BitCalcFn {
    void operator()(int64_t* ptr, double freq, int N) {
        bitcalc(ptr, freq, N);
    }
};

template <int N>
struct TrivialT {
    void operator()(int64_t* ptr, double freq, int) {
        trivial<N>(ptr, freq);
    }
};

template <int N>
struct CordicT {
    void operator()(int64_t* ptr, double freq, int) {
        cordic<N>(ptr, freq);
    }
};

template <int N>
struct BitCalcT {
    void operator()(int64_t* ptr, double freq, int) {
        bitcalc<N>(ptr, freq);
    }
};

template <typename Fn, int N>
static void Benchmark(benchmark::State& state) {
    Fn fn;
    std::vector<int64_t> samples1(N);
    for (auto _ : state) {
        fn(samples1.data(), 1, samples1.size());
    }
}

BENCHMARK(Benchmark<TrivialFn, 512>);
BENCHMARK(Benchmark<CordicFn, 512>);
BENCHMARK(Benchmark<BitCalcFn, 512>);
BENCHMARK(Benchmark<TrivialT<512>, 512>);
BENCHMARK(Benchmark<CordicT<512>, 512>);
BENCHMARK(Benchmark<BitCalcT<512>, 512>);

BENCHMARK(Benchmark<TrivialFn, 32768>);
BENCHMARK(Benchmark<CordicFn, 32768>);
BENCHMARK(Benchmark<BitCalcFn, 32768>);
BENCHMARK(Benchmark<TrivialT<32768>, 32768>);
BENCHMARK(Benchmark<BitCalcT<32768>, 32768>);
BENCHMARK(Benchmark<CordicT<32768>, 32768>);

BENCHMARK(Benchmark<TrivialFn, 262144>);
BENCHMARK(Benchmark<CordicFn, 262144>);
BENCHMARK(Benchmark<BitCalcFn, 262144>);
BENCHMARK(Benchmark<TrivialT<252144>, 262144>);
BENCHMARK(Benchmark<BitCalcT<252144>, 262144>);
BENCHMARK(Benchmark<CordicT<252144>, 262144>);

BENCHMARK_MAIN();
