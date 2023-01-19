
# Compiling with cmake

```
$ mkdir build
$ cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_CXX_COMPILER=clang++ ..
$ make
```

# Compiling by hand
```
$ clang++ -O3 -march=x86-64 ../naive.cpp -o naive
$ clang++ -O3 -march=skylake ../naive.cpp -o naive
```

# Disassembling
```
objdump -dC naive
```
