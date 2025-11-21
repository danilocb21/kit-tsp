#ifndef MLP_HPP
#define MLP_HPP

#include <vector>
#include <random>

#include "data.h"
#include "solution.hpp"

class MLP {
private:
    Data m_instance;

    std::mt19937 m_generator;

public:
    vector<vector<double>> dist_matrix;
    enum class Neighborhood : uint8_t { Swap, TwoOpt, Reinsertion, OrOpt2, OrOpt3 };

    // Construction

    Solution rand_cheapest_insertion(double alpha);

    // Local Search

    void RVND(Solution &solution);

    // Perturbation

    void perturbation(Solution &solution);

    // Neighborhoods

    bool best_improvement_swap(Solution &solution);
    bool best_improvement_two_opt(Solution &solution);
    bool best_improvement_or_opt(Solution &solution, int block_size);

    // Metaheuristics

    Solution GILS_RNVD(size_t max_iterations, size_t max_ils_iterations, double alpha);


    MLP(Data &instance);
};

#endif