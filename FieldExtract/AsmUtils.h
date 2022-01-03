#pragma once
#include <cstddef>
#include <cstdint>

static inline uint64_t now() {
    return __builtin_ia32_rdtsc();
}

static inline uint64_t tic() {
    uint64_t ticks;
    asm volatile(
        "lfence\n\t"
        "rdtsc\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %0\n\t"
        "lfence"
        : "=a"(ticks)
        :
        : "rdx", "memory", "cc");
    return ticks;
}

static inline uint64_t toc() {
    uint64_t ticks;
    asm volatile(
        "rdtscp\n\t"
        "shl $32, %%rdx\n\t"
        "or %%rdx, %0\n\t"
        "lfence"
        : "=a"(ticks)
        :
        : "rcx", "rdx", "memory", "cc");
    return ticks;
}

void makeExecutable(void *code, size_t size);
