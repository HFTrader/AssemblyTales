#include <array>
#include <cstdio>

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

__attribute__((target("default"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("default\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=core2"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("core2\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=sandybridge"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("sandybridge\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=haswell"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("haswell\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=cascadelake"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("cascadelake\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=znver1"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("znver1\n");
    return detail::multiply(m, v);
}

__attribute__((target("arch=znver2"))) Vector multiply(const Matrix& m, const Vector& v) {
    printf("znver2\n");
    return detail::multiply(m, v);
}

int main() {
    Vector v{1, 2};
    Matrix m{3, 4, 5, 6};
    Vector r = multiply(m, v);
    printf("%f %f\n", r[0], r[1]);
}