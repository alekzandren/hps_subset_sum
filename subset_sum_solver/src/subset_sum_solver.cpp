#include "subset_sum_solver.hpp"
#include <algorithm>
#include <cmath>

void SubsetSumSolver::generate_left_space(size_t start, size_t end, mpz_class current_sum, std::vector<bool>& current_mask, size_t index) {
    if (index == end) {
        storage.insert_sum(current_sum, current_mask);
        return;
    }
    current_mask[index] = false;
    generate_left_space(start, end, current_sum, current_mask, index + 1);
    current_mask[index] = true;
    generate_left_space(start, end, current_sum + weights[index], current_mask, index + 1);
}

bool SubsetSumSolver::search_right_space(size_t start, size_t end, mpz_class current_sum, const mpz_class& target,
                                        std::vector<bool>& current_mask, size_t index, std::vector<bool>& result_mask) {
    if (index == end) {
        mpz_class complement = target - current_sum;
        std::vector<bool> left_mask;
        if (storage.find_complement(complement, left_mask)) {
            result_mask = current_mask;
            for(size_t i = 0; i < left_mask.size() && i < start; ++i) {
                result_mask[i] = left_mask[i];
            }
            return true;
        }
        return false;
    }

    current_mask[index] = false;
    if (search_right_space(start, end, current_sum, target, current_mask, index + 1, result_mask)) return true;

    current_mask[index] = true;
    if (search_right_space(start, end, current_sum + weights[index], target, current_mask, index + 1, result_mask)) return true;

    return false;
}

std::pair<bool, std::vector<bool>> SubsetSumSolver::solve_exact(const mpz_class& target) {
    size_t n = weights.size();
    size_t mid = n / 2;

    storage.clear();
    storage.begin_transaction();
    std::vector<bool> current_mask(n, false);
    generate_left_space(0, mid, 0, current_mask, 0);
    storage.end_transaction();

    std::vector<bool> result_mask(n, false);
    bool found = search_right_space(mid, n, 0, target, current_mask, mid, result_mask);
    return {found, result_mask};
}

std::vector<mpz_class> SubsetSumSolver::trim_spectrum(const std::vector<mpz_class>& spectrum, double delta) {
    std::vector<mpz_class> output;
    if (spectrum.empty()) return output;

    output.push_back(spectrum[0]);
    mpz_class last = spectrum[0];

    for (size_t i = 1; i < spectrum.size(); ++i) {
        if (spectrum[i].get_d() > last.get_d() * (1.0 + delta)) {
            output.push_back(spectrum[i]);
            last = spectrum[i];
        }
    }
    return output;
}

mpz_class SubsetSumSolver::solve_fptas(const mpz_class& target, double epsilon) {
    size_t n = weights.size();
    std::vector<mpz_class> spectrum = {0};
    double delta = epsilon / (2.0 * n);

    for (const auto& w : weights) {
        std::vector<mpz_class> next_spectrum;
        next_spectrum.reserve(spectrum.size() * 2);
        size_t i = 0, j = 0;
        std::vector<mpz_class> shifted_spectrum;
        shifted_spectrum.reserve(spectrum.size());
        for (const auto& x : spectrum) {
            if (x + w <= target) shifted_spectrum.push_back(x + w);
        }

        while (i < spectrum.size() || j < shifted_spectrum.size()) {
            if (i == spectrum.size()) {
                next_spectrum.push_back(shifted_spectrum[j++]);
            } else if (j == shifted_spectrum.size()) {
                next_spectrum.push_back(spectrum[i++]);
            } else if (spectrum[i] <= shifted_spectrum[j]) {
                next_spectrum.push_back(spectrum[i++]);
            } else {
                next_spectrum.push_back(shifted_spectrum[j++]);
            }
        }

        spectrum = trim_spectrum(next_spectrum, delta);
    }
    mpz_class max_approx = 0;
    for (const auto& val : spectrum) {
        if (val <= target && val > max_approx) {
            max_approx = val;
        }
    }
    return max_approx;
}