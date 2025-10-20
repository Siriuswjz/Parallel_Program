# Project Structure

## Root Directory Layout

```
.
├── .git/                    # Git version control
├── .gitignore              # Build artifacts and IDE files exclusion
├── .kiro/                  # Kiro AI assistant configuration
├── .vscode/                # VS Code workspace settings
├── *.cpp                   # C++ source files
├── *                       # Compiled executables (no extension)
└── *.dSYM/                 # Debug symbol directories (macOS)
```

## File Organization

### Source Files
- **Single-purpose programs**: Each `.cpp` file contains a complete, standalone program
- **Naming convention**: Descriptive names using underscores (e.g., `matrix_mul_2d.cpp`)
- **No header files**: All functionality is contained within individual source files

### Build Artifacts
- **Executables**: Same name as source file without `.cpp` extension
- **Debug symbols**: `.dSYM` directories for debugging information
- **Ignored files**: All build artifacts are excluded via `.gitignore`

## Code Organization Patterns

### Function Structure
- Memory management functions (`allocate_matrix`, `free_matrix`)
- Initialization functions (`init_matrix`)
- Core algorithm implementations (both serial and parallel versions)
- Main function with timing and benchmarking

### Parallel Implementation Pattern
- Serial version for baseline comparison
- OpenMP parallel version with `#pragma omp` directives
- Performance timing using `omp_get_wtime()`
- Thread count reporting in parallel sections

## Memory Management
- Manual memory allocation using `new`/`delete`
- Contiguous memory layout for cache efficiency
- Proper cleanup in main function before exit