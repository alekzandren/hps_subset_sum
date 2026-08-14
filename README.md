# Subset Sum Solver (MITM & FPTAS)

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/20)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg?style=flat-square)](https://github.com/alekzandren/hps_subset_sum)
[![Version](https://img.shields.io/badge/Version-1.0.0-blueviolet.svg?style=flat-square)](https://github.com/alekzandren/hps_subset_sum)
[![Library: GMP](https://img.shields.io/badge/Library-GMP-informational.svg?style=flat-square)](https://gmplib.org/)
[![Database: SQLite3](https://img.shields.io/badge/Database-SQLite3-003B57.svg?style=flat-square&logo=sqlite&logoColor=white)](https://www.sqlite.org/)
[![Complexity](https://img.shields.io/badge/Complexity-O(2%5E(n%2F2))-purple.svg?style=flat-square)](https://github.com/alekzandren/hps_subset_sum)

A high-performance C++ solver for the Subset Sum Problem, designed to handle large integers using the GNU MP (GMP) library. This project implements both an exact solution via the Meet-in-the-Middle (MITM) algorithm for smaller datasets and an Fully Polynomial-Time Approximation Scheme (FPTAS) for larger, more complex inputs.

## Features

* Exact MITM Solver: Uses a divide-and-conquer strategy to find an exact subset sum.

* FPTAS Implementation: Provides an approximation when an exact solution is computationally infeasible.

* SQLite Persistence: Leverages SQLite to cache intermediate sums, optimizing memory management and lookups.

* Arbitrary Precision: Built upon GMP (``mpz_class``), allowing the processing of numbers far exceeding standard 64-bit integer limits.

* Modern C++: Utilizes C++20 standards.

## Prerequisites

To compile and run this project, ensure you have the following dependencies installed:
* Compiler: A C++20 compliant compiler (GCC, Clang, or MSVC).
* GMP: GNU Multiple Precision Arithmetic Library.
* SQLite3: The SQLite development library.
* CMake: Version 3.20 or higher.

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/hps_subset_sum.git
cd hps_subset_sum
```

2. Build using CMake:
```bash
mkdir build && cd build
cmake ..
make
```

## Usage
The project is structured as a library with a demonstration in ``main.cpp``. You can instantiate the SubsetSumSolver with a set of integers and use the provided methods to find a target sum.

```cpp
#include "subset_sum_solver.hpp"

// Define your input set
std::vector<mpz_class> items = { ... };
SubsetSumSolver solver(items);

// Find exact match
auto [found, mask] = solver.solve_exact(target);

// Or get an approximation
mpz_class approx = solver.solve_fptas(target, 0.05);
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request if you find bugs or want to implement additional optimizations.

## License

This project is licensed under the MIT License. See the .LICENSE file for details.
