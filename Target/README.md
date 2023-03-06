# Substack post

This repo refers to the following substack post
https://lucisqr.substack.com/p/clanggcc-target-attributes

# Compiling with cmake

```
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=release \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_RELEASE_FLAGS_INIT="-march=x86-64" \
        ..
$ make
```

# Compiling by hand
```
$ clang++ -O3 -march=x86-64 simple.cpp -o simple
$ clang++ -O3 -march=x86-64 main.cpp functions.cpp -o main
$ clang++ -O3 -march=skylake bench.cpp functions.cpp -o bench
```

# Disassembling
```
objdump -dC naive
```
