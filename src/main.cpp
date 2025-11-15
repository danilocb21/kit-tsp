#include "Data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>

using namespace std;

#define INF 2e18

int N;

struct Solution {
    vector<int> sequence;
    double cost;
};

struct InsertionInfo {
    int insertedNode;
    int removedEdge;
    double cost;

    bool operator<(const InsertionInfo &p) const {
        return cost < p.cost;
    }
};

vector<int> chooseRandomNodes();
set<int> remainingNodes(vector<int> &sequence);
vector<InsertionInfo> getInsertionCosts(Data &dt, vector<int> &seq);
Solution Construction(Data &dt);
void LocalSearch(Data &dt, Solution &s);
Solution ILS(Data &dt, int maxIter, int maxIterIls);

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();

    srand(time(NULL));

    N = data.getDimension();

    int maxIter = 50;
    int maxIterIls = N >= 150 ? N / 2 : N;

    Solution tsp = ILS(data, maxIter, maxIterIls);

    cout << "Solução: ";
    for (int x : tsp.sequence) 
        cout << x << ' ';
    cout << endl;
    cout << "Custo: " << tsp.cost << endl;

    return 0;
}

vector<int> chooseRandomNodes() {
    vector<int> sequence = { 1 };

    set<int> vis;
    vis.insert(1);

    // Escolhe 3 nós aleatórios para
    // compor a sequencia inicial
    while (sequence.size() < 4) {
        int node = rand() % N + 1;

        if (vis.count(node)) continue;

        vis.insert(node);
        sequence.push_back(node);
    }

    sequence.push_back(1);

    return sequence;
}

set<int> remainingNodes(vector<int> &sequence) {
    set<int> vis;
    for (int x : sequence) vis.insert(x);

    set<int> nodes;
    for (int i = 1; i <= N; i++) if (!vis.count(i))
        nodes.insert(i);
    
    return nodes;
}

vector<InsertionInfo> getInsertionCosts(Data &dt, vector<int> &seq, set<int> &candidates) {
    vector<InsertionInfo> s;
    s.reserve((seq.size() - 1) * (candidates.size()));
    
    for (int a = 0; a < seq.size() - 1; a++) {
        int i = seq[a];
        int j = seq[a + 1];
        for (int k : candidates) {
            double cost = dt.getDistance(i,k) + dt.getDistance(k,j) - dt.getDistance(i,j);
            s.push_back({k, a, cost});
        }
    }

    return s;
}

// Inserção mais barata
Solution Construction(Data &dt) {
    Solution sol{};
    sol.sequence = chooseRandomNodes();
    auto &seq = sol.sequence;
    auto CL = remainingNodes(seq);
    
    while (!CL.empty()) {
        auto insertionCosts = getInsertionCosts(dt, seq, CL);
        sort(insertionCosts.begin(), insertionCosts.end());

        double alpha = (double) rand() / RAND_MAX;
        int selected = rand() % ((int)ceil(alpha * insertionCosts.size()));
        
        auto &x = insertionCosts[selected];
        CL.erase(x.insertedNode);
        seq.insert(seq.begin() + x.removedEdge + 1, x.insertedNode);
    }

    for (int a = 0; a < seq.size() - 1; a++) {
        int i = seq[a];
        int j = seq[a + 1];
        sol.cost += dt.getDistance(i,j);
    }

    return sol;
}

bool bestImprovementSwap(Data &dt, Solution &s) {
    double bestDelta = 0.0;
    int best_i = -1, best_j = -1;

    for (int i = 1; i < s.sequence.size() - 1; i++) {
        int vi = s.sequence[i];
        int vi_prev = s.sequence[i-1];
        int vi_next = s.sequence[i+1];

        for (int j = i + 1; j < s.sequence.size() - 1; j++) {
            int vj = s.sequence[j];
            int vj_prev = s.sequence[j-1];
            int vj_next = s.sequence[j+1];

            double delta;
            if (i + 1 == j)
                delta = dt.getDistance(vi_prev, vj) + dt.getDistance(vj, vi) 
                    + dt.getDistance(vi, vj_next) - dt.getDistance(vi_prev, vi)
                    - dt.getDistance(vi, vj) - dt.getDistance(vj, vj_next);
            else
                delta = dt.getDistance(vi_prev, vj) + dt.getDistance(vj, vi_next) 
                    + dt.getDistance(vj_prev, vi) + dt.getDistance(vi, vj_next) - dt.getDistance(vi_prev, vi)
                    - dt.getDistance(vi, vi_next) - dt.getDistance(vj_prev, vj) - dt.getDistance(vj, vj_next);

            if (delta < bestDelta) {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0) {
        swap(s.sequence[best_i], s.sequence[best_j]);
        s.cost += bestDelta;
        return true;
    }

    return false;
}

void LocalSearch(Data &dt, Solution &s) {
    vector<int> NL = { 1 };
    bool improved = false;

    while (!NL.empty()) {
        int n = rand() % NL.size();
        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(dt, s);
            break;
        // case 2:
        //     improved = bestImprovement2Opt(dt, s);
        //     break;
        // case 3:
        //     improved = bestImprovementOrOpt(dt, s, 1);
        //     break;
        // case 4:
        //     improved = bestImprovementOrOpt(dt, s, 2);
        //     break;
        // case 5:
        //     improved = bestImprovementOrOpt(dt, s, 3);
        //     break;
        }

        if (improved)
            NL = { 1 };
        else
            NL.erase(NL.begin() + n);
    }
}

Solution ILS(Data &dt, int maxIter, int maxIterIls) {
    Solution bestOfAll;
    bestOfAll.cost = INF;

    for (int i = 0; i < maxIter; i++) {
        Solution s = Construction(dt);
        Solution best = s;
   
        int iterIls = 0;
        
        while (iterIls <= maxIterIls) {
            // Busca uma solução vizinha melhor
            LocalSearch(dt, s);
            if (s.cost < best.cost) {
                best = s;
                iterIls = 0;
            }
            iterIls++;
        }

        if (best.cost < bestOfAll.cost)
            bestOfAll = best;
    }

    return bestOfAll;
}
