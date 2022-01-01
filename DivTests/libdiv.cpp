#include <cstdint>

bool empty( uint64_t j ) {
    return true;
}

bool is_multiple_of_3( uint64_t j ) {
    return ( uint32_t(j)%3==0 );
}

bool is_multiple_of_3_optimized( uint64_t j ) {
    uint32_t i = (uint32_t)j;
    return  (i * (-1u/3+1) <= -1u/3);
}

uint64_t tsc_start()
{
    uint64_t cycles_high, cycles_low;
    asm volatile ( "xor %%eax,%%eax\n\t"
                   "CPUID\n\t"
                   "RDTSC\n\t"
                   "mov %%rdx, %0\n\t"
                   "mov %%rax, %1\n\t":
                   "=r" ( cycles_high ), "=r" ( cycles_low ):
                   : "%rax", "%rbx", "%rcx", "%rdx" );
    return ( ( ( uint64_t )cycles_high )<<32 )  | ( ( uint64_t )cycles_low );
}

uint64_t tsc_stop()
{
    uint64_t cycles_high, cycles_low;
    asm volatile( "RDTSCP\n\t"
                  "mov %%rdx, %0\n\t"
                  "mov %%rax, %1\n\t"
                  "xor %%eax,%%eax\n\t"
                  "CPUID\n\t": "=r" ( cycles_high ), "=r"
                  ( cycles_low ):: "%rax", "%rbx", "%rcx", "%rdx" );
    return ( ( ( uint64_t )cycles_high )<<32 )  | ( ( uint64_t )cycles_low );
}
