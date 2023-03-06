################################################################################
# Substack post: 
#     https://lucisqr.substack.com/p/clanggcc-target-attributes
################################################################################

cmake_minimum_required( VERSION 3.0 )
project( target )
enable_language( C CXX ASM )

# we dont need all the benchmark internal testing
set( BENCHMARK_ENABLE_TESTING OFF CACHE STRING "")
include( FetchContent )
FetchContent_Declare( benchmark 
    GIT_REPOSITORY https://github.com/google/benchmark.git 
    GIT_TAG main )
FetchContent_MakeAvailable( benchmark )

add_library( functions SHARED functions.cpp )
add_executable( main main.cpp)
target_link_libraries( main functions )

add_executable( bench bench.cpp )
target_link_libraries( bench functions benchmark )

add_executable( simple simple.cpp )

