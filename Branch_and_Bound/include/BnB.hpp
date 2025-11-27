#ifndef BNB_HPP
#define BNB_HPP

#include <cstdint>
#include <vector>

#include "data.h"
#include "node.hpp"

class BnB {
private:
    Data m_instance;
    
public:
    enum class BranchingStrategy : uint8_t { DFS, BFS, MLB };
    double **costs;
    int N; // grid dimension

    // Construct a solution with a greedy heuristic and returns its cost
    double construction();

    // Branch and Bound algorithm
    double solve(const BranchingStrategy branching_strategy);

    BnB(Data &instance, double **cost);
};

#endif