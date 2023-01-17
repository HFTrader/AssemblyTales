#pragma once
#include <cstdint>

// Note that it is IMPERATIVE not to bring the
// implementation into the header otherwise
// the benchmark will be skewed with respect to
// inlining

void trivial(int64_t* samples, double freq, int N);
void cordic(int64_t* samples, double freq, int N);
void bitcalc(int64_t* samples, double freq, int N);

// The template versions are explicitly implemented
// for 512, 32768 and 262144 in functions.cpp
template <int N>
void bitcalc(int64_t* samples, double freq);
template <int N>
void cordic(int64_t* samples, double freq);
template <int N>
void trivial(int64_t* samples, double freq);
