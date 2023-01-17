#pragma once
#include <cstdint>

void trivial(int64_t* samples, double freq, int N);
void cordic(int64_t* samples, double freq, int N);
void bitcalc(int64_t* samples, double freq, int N);

template <int N>
void bitcalc(int64_t* samples, double freq);
template <int N>
void cordic(int64_t* samples, double freq);
template <int N>
void trivial(int64_t* samples, double freq);
