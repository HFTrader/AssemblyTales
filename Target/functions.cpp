#include <array>
#include <cstdio>

using Vector = std::array<float, 2>;
using Matrix = std::array<float, 4>;

Vector multiply_standard(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}

__attribute__((target_clones("default", "arch=core2", "arch=sandybridge", "arch=haswell", "arch=cascadelake",
                             "arch=znver1", "arch=znver2"))) Vector
multiply(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}
