#include <iostream>
#include "subset_sum_solver.hpp"

int main() {
    std::vector<mpz_class> items = {
        mpz_class("123456789012345678901234567890"),
        mpz_class("987654321098765432109876543210"),
        mpz_class("555555555555555555555555555555"),
        mpz_class("111111111111111111111111111111")
    };

    mpz_class target("167888888888888888888888888888");

    SubsetSumSolver solver(items);

    std::cout << "Running Exact MITM solver..." << std::endl;
    auto [found, mask] = solver.solve_exact(target);

    if (found) {
        std::cout << "Success! Vector mask: ";
        for (bool b : mask) std::cout << b << " ";
        std::cout << std::endl;
    } else {
        std::cout << "No exact subset found." << std::endl;
    }

    std::cout << "\nRunning FPTAS solver..." << std::endl;
    mpz_class approx_res = solver.solve_fptas(target, 0.05);
    std::cout << "Approximated max sum: " << approx_res.get_str() << std::endl;

    return 0;
}