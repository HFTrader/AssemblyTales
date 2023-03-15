####################################################################################################
# Referencing to
# https://github.com/google/benchmark/commit/27c1d8ace94d23ab5d807801f280faea78836533#r103259540
####################################################################################################
# we dont need all the benchmark internal testing
set( BENCHMARK_ENABLE_TESTING OFF CACHE STRING "")

cmake_minimum_required( VERSION 3.0 )
project( target )
enable_language( CXX )
include( ExternalProject )
enable_testing()

set( RESULT_FILES "" )
macro( add_branch branch_name )
    ExternalProject_Add( benchmark_${branch_name}
        INSTALL_DIR install/${branch_name}
        CMAKE_ARGS 
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_BUILD_TYPE=release 
            -DBENCHMARK_ENABLE_TESTING=OFF 
            -DBENCHMARK_ENABLE_LIBPFM=OFF
        GIT_REPOSITORY https://github.com/google/benchmark.git 
        GIT_TAG ${branch_name} )

    ExternalProject_Get_Property( benchmark_${branch_name} INSTALL_DIR)
    add_executable( main_${branch_name} main.cpp )
    target_include_directories( main_${branch_name} PUBLIC ${INSTALL_DIR}/include )
    target_link_libraries( main_${branch_name} ${INSTALL_DIR}/lib/libbenchmark.a pthread )
    add_dependencies( main_${branch_name} benchmark_${branch_name} )
    add_custom_command( 
        OUTPUT results_${branch_name}.json 
        COMMAND main_${branch_name} --v=0 --benchmark_out=results_${branch_name}.json --benchmark_out_format=json )
    list( APPEND RESULT_FILES results_${branch_name}.json )
endmacro()

add_branch( v1.3.0 )
add_branch( v1.4.0 )
add_branch( v1.4.1 )
add_branch( v1.5.0 )
add_branch( v1.5.1 )
add_branch( v1.5.2 )
add_branch( v1.5.3 )
add_branch( v1.5.4 )
add_branch( v1.5.5 )
add_branch( v1.5.6 )
add_branch( v1.6.0 )
add_branch( v1.6.1 )
add_branch( v1.6.2 )
add_branch( v1.7.0 )
add_branch( v1.7.1 )
add_branch( main )

string( REPLACE ";" "  " FILE_LIST "${RESULT_FILES}" )

find_package (Python3 COMPONENTS Interpreter)
if ( Python3_Interpreter_FOUND )
add_custom_target( 
    graph ALL
    DEPENDS ${RESULT_FILES}
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/compile_benchmark_results.py graph ${RESULT_FILES} )
add_test( NAME regression 
    COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/compile_benchmark_results.py check ${RESULT_FILES} )
endif()


