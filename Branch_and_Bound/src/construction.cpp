#include <algorithm>
#include <random>
#include <unordered_set>
#include <vector>

#include "BnB.hpp"

struct InsertionInfo {
    int inserted_node;
    double cost;

    bool operator<(const InsertionInfo &p) const {
        return cost < p.cost;
    }

    InsertionInfo(int inserted_node, double cost) : inserted_node(inserted_node), cost(cost) {}
};

double BnB::construction() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<> rd_double(0.0, 0.25);

    double alpha = rd_double(rng);

    std::vector<int> sequence;
    sequence.push_back(1);

    int n = m_instance.getDimension();

    std::unordered_set<int> CL;
    for (int i = 2; i <= static_cast<int>(n); i++)
        CL.insert(i);
    
    while (!CL.empty()) {
        std::vector<InsertionInfo> insertion_costs;
        insertion_costs.reserve((CL.size()));

        int r = sequence.back();

        for (int node : CL) {
            double cost = costs[r - 1][node - 1];
            insertion_costs.emplace_back(node, cost);
        }

        sort(insertion_costs.begin(), insertion_costs.end());

        std::uniform_int_distribution<size_t> dist(0, static_cast<size_t>(alpha * static_cast<double>(insertion_costs.size() - 1)));

        size_t selected = dist(rng);
        
        auto &x = insertion_costs[selected];
        CL.erase(x.inserted_node);
        sequence.push_back(x.inserted_node);
    }
    sequence.push_back(1);

    double cost = 0.0;

    for (size_t i = 0; i < sequence.size() - 1; i++) {
        int vi = sequence[i] - 1;
        int vj = sequence[i + 1] - 1;
        cost += costs[vi][vj];
    }

    return cost;
}