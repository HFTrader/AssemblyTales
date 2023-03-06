#include <array>
#include <cstdio>
#include "benchmark/benchmark.h"

using Vector = std::array<float, 2>;
using Matrix = std::array<float, 4>;

namespace detail {
inline Vector multiply(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}
}  // namespace detail

Vector v{1, 2};
Matrix m{3, 4, 5, 6};

void Inlined(benchmark::State& state) {
    for (auto _ : state) {
        v = detail::multiply(m, v);
        benchmark::DoNotOptimize(v);
    }
}

void InlinedReuse(benchmark::State& state) {
    for (auto _ : state) {
        v = detail::multiply(m, v);
    }
}

Vector multiply_standard(const Matrix& m, const Vector& v);

void Standard(benchmark::State& state) {
    for (auto _ : state) {
        v = multiply_standard(m, v);
        benchmark::DoNotOptimize(v);
    }
}

void StandardReuse(benchmark::State& state) {
    for (auto _ : state) {
        v = multiply_standard(m, v);
    }
}

__attribute__((target_clones("default", "arch=core2", "arch=sandybridge", "arch=haswell", "arch=cascadelake",
                             "arch=znver1", "arch=znver2"))) Vector
multiply(const Matrix& m, const Vector& v);

void Encoded(benchmark::State& state) {
    for (auto _ : state) {
        v = multiply(m, v);
        benchmark::DoNotOptimize(v);
    }
}

void EncodedReuse(benchmark::State& state) {
    for (auto _ : state) {
        v = multiply(m, v);
    }
}

BENCHMARK(Inlined);
BENCHMARK(InlinedReuse);
BENCHMARK(Standard);
BENCHMARK(StandardReuse);
BENCHMARK(Encoded);
BENCHMARK(EncodedReuse);

BENCHMARK_MAIN();
