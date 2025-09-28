#include "functions.h"
#include <cmath>

constexpr double pi = M_PI;
constexpr int64_t Q = 16;

void bitcalc(int64_t* samples, double freq, int N) {
    double cs = std::cos((2 * pi * freq) / N);
    int64_t coeff = (1LL << Q) * 2 * cs;

    int64_t Qn1 = (1LL << Q) * pi * freq;
    int64_t Qn2 = 0;

    for (int j = 0; j < N; ++j) {
        samples[j] = Qn2 / N;
        int64_t Qn = ((coeff * Qn1) >> Q) - Qn2;
        Qn2 = Qn1;
        Qn1 = Qn;
    }
}

template <int N>
void bitcalc(int64_t* samples, double freq) {
    bitcalc(samples, freq, N);
}
template void bitcalc<512>(int64_t* samples, double freq);
template void bitcalc<32768>(int64_t* samples, double freq);
template void bitcalc<262144>(int64_t* samples, double freq);

void trivial(int64_t* samples, double freq, int N) {
    double cs = (2 * pi * freq) / N;
    double A = (1LL << Q) / 2;
    for (int j = 0; j < N; ++j) {
        samples[j] = A * sin(cs * j);
    }
}
template <int N>
void trivial(int64_t* samples, double freq) {
    trivial(samples, freq, N);
}
template void trivial<512>(int64_t* samples, double freq);
template void trivial<32768>(int64_t* samples, double freq);
template void trivial<262144>(int64_t* samples, double freq);

void cordic(int64_t* samples, double freq, int N) {
    double sn = std::sin(2 * pi * freq / N);
    double cs = std::cos(2 * pi * freq / N);
    double A = (1LL << Q) / 2;
    double x = 0;
    double y = 1;
    for (int j = 0; j < N; ++j) {
        double tx = x * cs + y * sn;
        double ty = y * cs - x * sn;
        samples[j] = A * x;
        x = tx;
        y = ty;
    }
}

template <int N>
void cordic(int64_t* samples, double freq) {
    cordic(samples, freq, N);
}
template void cordic<512>(int64_t* samples, double freq);
template void cordic<32768>(int64_t* samples, double freq);
template void cordic<262144>(int64_t* samples, double freq);

void icordic(int64_t* samples, double freq, int N) {
    double sn = std::sin(2 * pi * freq / N);
    double cs = std::cos(2 * pi * freq / N);
    double A = (1LL << Q) / 2;
    double x = 0;
    double y = 1;
    for (int j = 0; j < N; ++j) {
        double tx = x * cs + y * sn;
        double ty = y * cs - x * sn;
        samples[j] = A * x;
        x = tx;
        y = ty;
    }
}
