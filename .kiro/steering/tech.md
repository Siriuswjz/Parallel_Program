# Technology Stack

## Core Technologies

- **Language**: C++17 or later
- **Parallelization**: OpenMP for shared-memory parallel computing
- **Build System**: Direct compilation with g++ or clang++
- **Platform**: macOS (ARM64 architecture based on .dSYM structure)

## Dependencies

- OpenMP library (`-fopenmp` flag required)
- Standard C++ libraries: `<iostream>`, `<random>`, `<cmath>`, `<vector>`

## Common Commands

### Compilation
```bash
# Basic compilation with OpenMP
g++ -fopenmp -O3 -o program_name source_file.cpp

# Debug build with symbols
g++ -fopenmp -g -o program_name source_file.cpp

# Example compilations for existing files
g++ -fopenmp -O3 -o matrix_mul_2d matrix_mul_2d.cpp
g++ -fopenmp -O3 -o multiple_matrix multiple_matrix.cpp
g++ -fopenmp -O3 -o sin_taylor sin_taylor.cpp
```

### Execution
```bash
# Run compiled programs
./program_name

# Matrix multiplication with custom dimensions
./multiple_matrix 4000 4000 4000
```

### Cleanup
```bash
# Remove executables and debug symbols
rm -f matrix_mul_2d multiple_matrix sin_taylor
rm -rf *.dSYM
```

## Performance Considerations

- Use `-O3` optimization flag for performance testing
- OpenMP thread count can be controlled via `OMP_NUM_THREADS` environment variable
- Memory allocation uses contiguous arrays for cache efficiency