cmake_minimum_required( VERSION 3.0 )
project( cordic )

find_package( benchmark REQUIRED )
add_executable( bit_test bit_test.cpp functions.cpp )

add_executable( benchmark benchmark.cpp functions.cpp )
target_link_libraries( benchmark benchmark::benchmark )
