#include <limits>

#include "MLP.hpp"
#include "solution.hpp"

Solution MLP::GILS_RNVD(size_t max_iterations, size_t max_ils_iterations, double alpha) {
    Solution best_found;
    best_found.cost = 0;

    int n = static_cast<int> (dist_matrix.size() - 1);

    for (int i = 1; i <= n; i++) {
        best_found.sequence.push_back(i);
        if (i > 1) best_found.cost += dist_matrix[i - 1][i];
    }
    best_found.sequence.push_back(1);
    best_found.cost += dist_matrix[n][1];

    // for (size_t i = 0; i < max_iterations; i++) {
    //     Solution solution = rand_cheapest_insertion(alpha);
    //     Solution best = solution;

    //     size_t iteration_ils = 0;
        
    //     while (iteration_ils <= max_ils_iterations) {
    //         RVND(solution);

    //         if (solution.cost < best.cost) {
    //             best = solution;
    //             iteration_ils = 0;
    //         }
    //         else if (solution.cost > best.cost)
    //             solution = best;

    //         perturbation(solution);
    //         iteration_ils++;
    //     }

    //     if (best.cost < best_found.cost)
    //         best_found = best;
    // }

    return best_found;
}