#ifndef MLP_HPP
#define MLP_HPP

#include <vector>
#include <random>

#include "data.h"
#include "solution.hpp"
#include "subsequence.hpp"

class MLP {
private:
    Data m_instance;

    std::mt19937 m_generator;

public:
    std::vector<std::vector<double>> dist_matrix;
    std::vector<std::vector<Subsequence>> subsequence_matrix;
    enum class Neighborhood : uint8_t { Swap, TwoOpt, Reinsertion, OrOpt2, OrOpt3 };

    // Construction

    Solution rand_cheapest_insertion(const double alpha);

    // Local Search

    void RVND(Solution &solution);

    // Perturbation

    void perturbation(Solution &solution);

    // Neighborhoods

    bool best_improvement_swap(Solution &solution);
    bool best_improvement_two_opt(Solution &solution);
    bool best_improvement_or_opt(Solution &solution, int block_size);

    // Metaheuristics

    Solution GILS_RNVD(const size_t max_iterations, const size_t max_ils_iterations, const double alpha);

    // Subsequences calls

    void update_all_subsequences(Solution &solution);
    void update_subsequences(Solution &solution, size_t start, size_t end);
    inline Subsequence concatenate(Subsequence &sigma_1, Subsequence &sigma_2);

    MLP(Data &instance);
};

#endif