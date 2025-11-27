#ifndef BNB_HPP
#define BNB_HPP

#include <cstdint>
#include <vector>
#include <list>
#include <queue>

#include "data.h"
#include "node.hpp"

class BnB {
private:
    Data m_instance;
    
    std::list<Node> tree;
    
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq_tree;

public:
    enum class BranchingStrategy : uint8_t { DFS, BFS, MLB };
    double **costs;
    int N; // grid dimension

    // Construct a solution with a greedy heuristic and returns its cost
    double construction();

    // Branch and Bound algorithm
    double solve(const BranchingStrategy branching_strategy);
    double solve_pq(Node &root, double upper_bound);
    double solve_list(Node &root, double upper_bound, const BranchingStrategy branching_strategy);

    BnB(Data &instance, double **cost);
};

#endif