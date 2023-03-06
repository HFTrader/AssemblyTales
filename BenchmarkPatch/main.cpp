#include <array>
#include <cstdio>
#include "benchmark/benchmark.h"

using Vector = std::array<float, 2>;
using Matrix = std::array<float, 4>;

Vector multiply(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}

void Inlined(benchmark::State& state) {
    Vector v{1, 2};
    Matrix m{3, 4, 5, 6};
    for (auto _ : state) {
        Vector v = multiply(m, v);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(Inlined);

BENCHMARK_MAIN();
