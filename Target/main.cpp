#include <array>
#include <cstdio>

using Vector = std::array<float, 2>;
using Matrix = std::array<float, 4>;

__attribute__((target_clones("default", "arch=core2", "arch=sandybridge", "arch=haswell", "arch=cascadelake",
                             "arch=znver1", "arch=znver2"))) Vector
multiply(const Matrix& m, const Vector& v);

int main() {
    Vector v{1, 2};
    Matrix m{3, 4, 5, 6};
    Vector r = multiply(m, v);
    printf("%f %f\n", r[0], r[1]);
}