add_library( libdiv SHARED libdiv.cpp )
add_executable( testdiv testdiv.cpp )
target_link_libraries( testdiv libdiv )