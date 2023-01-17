#include "functions.h"
#include <vector>
#include <iostream>

int main() {
    int N = 512;
    int freq = 1;
    std::vector<int64_t> samples1(N);
    trivial(samples1.data(), freq, samples1.size());
    std::vector<int64_t> samples2(N);
    bitcalc(samples2.data(), freq, samples2.size());
    std::vector<int64_t> samples3(N);
    cordic(samples3.data(), freq, samples3.size());

    for (int j = 0; j < N; ++j) {
        double phase = double(360 * j) / N;
        printf("%-4d,%6.1f,%6ld,%6ld,%6ld\n", j, phase, samples1[j], samples2[j],
               samples3[j]);
    }
}
