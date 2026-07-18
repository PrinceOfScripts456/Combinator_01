# Combinator v1.0

## Release builds

### Standard build

```bash
g++ -std=c++23 -O2 ./src/*.cpp -o ./build/Combinator_v1.exe
```

### Portable (static) build

```bash
g++ -std=c++23 -O2 -static -static-libgcc -static-libstdc++ ./src/*.cpp -o ./build/Combinator_x86-64.exe
```

This build statically links the GCC and C++ runtime libraries, making it easier to run on systems without MinGW installed. Depending on the libraries used, some system libraries may still be dynamically linked.

## Development build

Check for warnings

```bash
g++ -std=c++23 -O2 -g -Wall -Wextra -pedantic -Wshadow -Wconversion -Wuninitialized -Wnull-dereference -Wvla -Wcast-align -Wfloat-equal ./src/*.cpp -o ./build/Combinator_v1.exe
```
