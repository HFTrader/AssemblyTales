#include <cstdint>
#include <stdio.h>

uint64_t tsc_start();
uint64_t tsc_stop();


template< typename Fn >
void test( const char* msg, const uint64_t count, const Fn&& fn ) {
    uint64_t sum = 0;
    for ( uint64_t j=0; j<count; ++j ) {
        uint64_t t0 = tsc_start();
        fn( j );
        uint64_t t1 = tsc_stop();
        uint64_t elap = t1>=t0 ? t1-t0 : 0;
        sum += elap;
    }
    printf( "%s average: %ld cycles\n", msg, sum/count );
}

bool is_multiple_of_3( uint64_t j );
bool is_multiple_of_3_optimized( uint64_t j );
bool empty( uint64_t j );

constexpr uint64_t numloops = 50000000;

int main()
{
    uint64_t emptycount = 0;
    uint64_t mult3count = 0;
    uint64_t nonmult3count = 0;
    test( "Empty", numloops, [&]( uint64_t j ) {
            if ( empty(j) ) ++mult3count;
            else nonmult3count++;
        });
    test( "Standard", numloops, [&]( uint64_t j ) {
            if ( is_multiple_of_3(j) ) mult3count++;
            else nonmult3count++;
        });
    test( "Optimized", numloops, [&]( uint64_t j ) {
            uint32_t i = (uint32_t)j;
            if ( is_multiple_of_3_optimized(j) ) mult3count++;
            else nonmult3count++;
        });

    printf( "%ld %ld %ld\n", emptycount, mult3count, nonmult3count );
}
