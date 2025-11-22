#include <cstddef>
#include <algorithm>

#include "MLP.hpp"
#include "solution.hpp"
#include "subsequence.hpp"

bool MLP::best_improvement_swap(Solution &solution) {
    double best_cost = solution.cost;
    size_t best_i, best_j;

    size_t n = solution.sequence.size();

    for (size_t i = 1; i < n - 1; i++) {

        for (size_t j = i + 1; j < n - 1; j++) {

            Subsequence sigma = concatenate(subsequence_matrix[0][i - 1], subsequence_matrix[j][j]);
            if (i + 1 != j) sigma = concatenate(sigma, subsequence_matrix[i + 1][j - 1]);
            sigma = concatenate(sigma, subsequence_matrix[i][i]);
            sigma = concatenate(sigma, subsequence_matrix[j + 1][n - 1]);

            if (sigma.C < best_cost) {
                best_cost = sigma.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_cost < solution.cost) {
        swap(solution.sequence[best_i], solution.sequence[best_j]);
        update_subsequences(solution, best_i, best_j);
        solution.cost = best_cost;
        return true;
    }

    return false;
}

bool MLP::best_improvement_two_opt(Solution &solution) {
    double best_cost = solution.cost;
    size_t best_i, best_j;

    size_t n = solution.sequence.size();

    for (size_t i = 0; i < n - 1; i++) {

        for (size_t j = i + 2; j < n - 1; j++) {

            Subsequence sigma = concatenate(subsequence_matrix[0][i], subsequence_matrix[j][i + 1]);
            sigma = concatenate(sigma, subsequence_matrix[j + 1][n - 1]);

            if (sigma.C < best_cost) {
                best_cost = sigma.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_cost < solution.cost) {
        std::reverse(solution.sequence.begin() + best_i + 1, solution.sequence.begin() + best_j + 1);
        update_all_subsequences(solution);
        solution.cost = best_cost;
        return true;
    }

    return false;
}

bool MLP::best_improvement_or_opt(Solution &solution, int block_size) {
    double best_cost = solution.cost;
    size_t best_i, best_j;

    size_t n = solution.sequence.size();

    for (size_t i = 0; i < n - block_size - 1; i++) {
        for (size_t j = 0; j < n - 1; j++) {
            if (j >= i && j <= i + block_size) continue;

            Subsequence sigma;
            if (i < j) {
                sigma = concatenate(subsequence_matrix[0][i], subsequence_matrix[i + block_size + 1][j]);
                sigma = concatenate(sigma, subsequence_matrix[i + 1][i + block_size]);
                sigma = concatenate(sigma, subsequence_matrix[j + 1][n - 1]);
            }
            else {
                sigma = concatenate(subsequence_matrix[0][j], subsequence_matrix[i + 1][i + block_size]);
                sigma = concatenate(sigma, subsequence_matrix[j + 1][i]);
                sigma = concatenate(sigma, subsequence_matrix[i + block_size + 1][n - 1]);
            }

            if (sigma.C < best_cost) {
                best_cost = sigma.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_cost < solution.cost) {
        auto block_start = solution.sequence.begin() + best_i + 1;
        auto block_end   = solution.sequence.begin() + best_i + 1 + block_size;
        auto segment     = solution.sequence.begin() + best_j + 1;

        if (best_i < best_j) {
            rotate(block_start, block_end, segment);
            update_subsequences(solution, best_i + 1, best_j);
        }
        else {
            rotate(segment, block_start, block_end);
            update_subsequences(solution, best_j + 1, best_i + block_size);
        }

        solution.cost = subsequence_matrix[0][n - 1].C;
        return true;
    }

    return false;
}