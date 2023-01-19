#include <array>
#include <cstdio>

using Vector = std::array<float, 2>;

using Matrix = std::array<float, 4>;

namespace detail {
Vector multiply(const Matrix& m, const Vector& v) {
    Vector r;
    r[0] = v[0] * m[0] + v[1] * m[2];
    r[1] = v[0] * m[1] + v[1] * m[3];
    return r;
}
}  // namespace detail

__attribute__((target("default"))) Vector multiply(const Matrix& m, const Vector& v) {
    return detail::multiply(m, v);
}

__attribute__((target("avx"))) Vector multiply(const Matrix& m, const Vector& v) {
    return detail::multiply(m, v);
}

int main() {
    Vector v{1, 2};
    Matrix m{3, 4, 5, 6};
    Vector r = multiply(m, v);
    printf("Result: %f %f\n", r[0], r[1]);
}
