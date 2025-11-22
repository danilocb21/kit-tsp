#include <iostream>

#include "solution.hpp"

void Solution::print() const {
    std::cout << "Solution\n";

    for (int x : sequence)
        std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Cost: " << std::fixed << cost << '\n';
}