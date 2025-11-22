#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "data.h"
#include "MLP.hpp"
#include "solution.hpp"

int main(int argc, char **argv) {
    srand(time(nullptr));
    
    auto data = Data(argc, argv[1]);
    data.read();

    MLP mlp = MLP(data);

    size_t max_iterations = 10;
    size_t max_ils_iterations = static_cast<size_t>(min(data.getDimension(), 100));;
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, 25);

    size_t runs = 10;
    double total_time = 0.0, total_cost = 0.0;

    for (size_t i = 0; i < runs; i++) {
        double alpha = dist(rng) / 100.0;

        auto start = chrono::high_resolution_clock::now();

        Solution s = mlp.GILS_RNVD(max_iterations, max_ils_iterations, alpha);

        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        total_time += duration.count();
        total_cost += s.cost;
    }
    
    std::cout << std::fixed << total_time / runs << ' ' << total_cost / runs << "\n\n";

    return 0;
}
