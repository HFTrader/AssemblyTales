####################################################################################################
# Referencing to
# https://github.com/google/benchmark/commit/27c1d8ace94d23ab5d807801f280faea78836533#r103259540
####################################################################################################

cmake_minimum_required( VERSION 3.0 )
project( target )
enable_language( C CXX ASM )

# we dont need all the benchmark internal testing
set( BENCHMARK_ENABLE_TESTING OFF CACHE STRING "")
include( ExternalProject )
ExternalProject_Add( benchmark_current
    INSTALL_DIR install/current
    CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=OFF -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>  -DBENCHMARK_ENABLE_LIBPFM=ON 
    GIT_REPOSITORY https://github.com/google/benchmark.git 
    GIT_TAG main )
ExternalProject_Add( benchmark_proposed
    INSTALL_DIR install/proposed
    CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=OFF -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>  -DBENCHMARK_ENABLE_LIBPFM=ON 
    GIT_REPOSITORY https://github.com/HFTrader/benchmark.git 
    GIT_TAG refactoring-perf-counters )

ExternalProject_Get_Property(benchmark_current INSTALL_DIR)
add_executable( main_current main.cpp )
target_include_directories( main_current PUBLIC ${INSTALL_DIR}/include )
target_link_libraries( main_current ${INSTALL_DIR}/lib/libbenchmark.a pfm pthread )
add_dependencies( main_current benchmark_current )

ExternalProject_Get_Property(benchmark_proposed INSTALL_DIR)
add_executable( main_proposed main.cpp )
target_include_directories( main_proposed PUBLIC ${INSTALL_DIR}/include )
target_link_libraries( main_proposed ${INSTALL_DIR}/lib/libbenchmark.a pfm pthread )
add_dependencies( main_proposed benchmark_proposed )
