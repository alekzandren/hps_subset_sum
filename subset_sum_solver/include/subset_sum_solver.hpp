#pragma once
#include <vector>
#include <gmpxx.h>
#include "storage_engine.hpp"

class SubsetSumSolver {
private:
    std::vector<mpz_class> weights;
    StorageEngine storage;
    size_t fptas_threshold;
    void generate_left_space(size_t start, size_t end, mpz_class current_sum, std::vector<bool>& current_mask, size_t index);
    bool search_right_space(size_t start, size_t end, mpz_class current_sum, const mpz_class& target, std::vector<bool>& current_mask, size_t index, std::vector<bool>& result_mask);

    std::vector<mpz_class> trim_spectrum(const std::vector<mpz_class>& spectrum, double delta);

public:
    explicit SubsetSumSolver(std::vector<mpz_class> input_weights, size_t threshold = 24)
        : weights(std::move(input_weights)), fptas_threshold(threshold) {}

    std::pair<bool, std::vector<bool>> solve_exact(const mpz_class& target);
    mpz_class solve_fptas(const mpz_class& target, double epsilon);
};