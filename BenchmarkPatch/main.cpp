#include <array>
#include <cstdio>
#include <random>
#include "benchmark/benchmark.h"

void Empty(benchmark::State& state) {
    for (auto _ : state) {
    }
}

void ClobberMemory(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::ClobberMemory();
    }
}

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<int64_t> dist(1, 100);

using Vector = std::array<float, 2>;
using Matrix = std::array<float, 4>;

Vector multiply(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}

void VecMultiply(benchmark::State& state) {
    Vector v;
    Matrix m;
    for (float& value : v) value = dist(g);
    for (float& value : m) value = dist(g);
    for (auto _ : state) {
        Vector v = multiply(m, v);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(Empty);
BENCHMARK(ClobberMemory);
BENCHMARK(VecMultiply);

BENCHMARK_MAIN();
