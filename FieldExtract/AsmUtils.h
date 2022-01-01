#pragma once
#include <cstddef>
#include <cstdint>

static inline uint64_t now()
{
    return __builtin_ia32_rdtsc();
}

void makeExecutable(void *code, size_t size);
