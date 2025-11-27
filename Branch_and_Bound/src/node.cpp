#include <algorithm>
#include <vector>

#include "BnB.hpp"
#include "node.hpp"
#include "hungarian.h"

#define INFINITE 99999999

void Node::update(double **costs, int n) {
    std::vector<std::pair<std::pair<int, int>, double>> prev_vals;
    for (auto arc : this->forbidden_arcs) {
        int i = arc.first;
        int j = arc.second;
        prev_vals.emplace_back(arc, costs[i][j]);
        costs[i][j] = INFINITE;
    }

    hungarian_problem_t p;
    int mode = HUNGARIAN_MODE_MINIMIZE_COST;
    hungarian_init(&p, costs, n, n, mode);

    this->lower_bound = hungarian_solve(&p);

    this->subtour = get_subtours(p, n);
    this->chosen = get_chosen();
    this->feasible = is_feasible();

    for (auto arc : prev_vals) {
        int i = arc.first.first;
        int j = arc.first.second;
        double c = arc.second;
        costs[i][j] = c;
    }
    
    hungarian_free(&p);
}

std::vector<std::vector<int>> Node::get_subtours(hungarian_problem_t &p, int n) {
    std::vector<std::vector<int>> subtours;
    
    std::vector<bool> visited(n);
    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        int node = i;
        std::vector<int> subtour;

        while (!visited[node]) {
            visited[node] = true;
            subtour.push_back(node);
            
            for (int j = 0; j < n; j++) {
                if (p.assignment[node][j]) {
                    node = j;
                    break;
                }
            }
        }
        subtour.push_back(i);

        subtours.emplace_back(subtour);
    }

    std::sort(subtours.begin(), subtours.end(), [](const std::vector<int> &a, const std::vector<int> &b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a[0] < b[0];
    });

    return subtours;
}

int Node::get_chosen() { return 0; } // a lista de subtours já esta ordenada pelo menor tamanho

bool Node::is_feasible() { return this->subtour.size() == static_cast<size_t> (1); }
