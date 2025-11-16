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

inline int rand_int(int a, int b) {
    return a + rand() % (b - a + 1);
}

Solution ILS(Data &dt, int maxIter, int maxIterIls);
Solution Construction(Data &dt);
void LocalSearch(Data &dt, Solution &s);
void Perturbation(Data &dt, Solution &s);

vector<int> chooseRandomNodes();
set<int> remainingNodes(vector<int> &sequence);
vector<InsertionInfo> getInsertionCosts(Data &dt, vector<int> &seq, set<int> &candidates);

bool bestImprovementSwap(Data &dt, Solution &s);
bool bestImprovement2Opt(Data &dt, Solution &s);
bool bestImprovementOrOpt(Data &dt, Solution &s, int blockSize);

void swap_subranges(vector<int> &v, int s1, int len1, int s2, int len2);

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
            if (s.cost > best.cost)
                s = best;
            Perturbation(dt, s);
            iterIls++;
        }

        if (best.cost < bestOfAll.cost)
            bestOfAll = best;
    }

    return bestOfAll;
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
        int selected = rand_int(0, (int) ceil(alpha * insertionCosts.size()));
        
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


void LocalSearch(Data &dt, Solution &s) {
    vector<int> NL = { 1, 2, 3, 4, 5 };
    bool improved = false;

    while (!NL.empty()) {
        int n = rand_int(0, NL.size());
        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(dt, s);
            break;
        case 2:
            improved = bestImprovement2Opt(dt, s);
            break;
        case 3:
            improved = bestImprovementOrOpt(dt, s, 1); // Reinsertion
            break;
        case 4:
            improved = bestImprovementOrOpt(dt, s, 2); // Or-opt2
            break;
        case 5:
            improved = bestImprovementOrOpt(dt, s, 3); // Or-opt3
            break;
        }

        if (improved)
            NL = { 1, 2, 3, 4, 5 };
        else
            NL.erase(NL.begin() + n);
    }
}

void Perturbation(Data &dt, Solution &s) {
    int lmin = 2;
    int lmax = (N + 9) / 10; // ceil(N/10)
    if (N < 6 || lmax < 2) return;

    auto hasIntersection = [](int a, int b, int c, int d) -> bool {
        return max(a, c) <= min(b, d);
    };

    int s1, s2, len1, len2;
    do {
        s1 = rand_int(1, N-3);
        s2 = rand_int(1, N-3);

        int l1max = min(N - 1 - s1, lmax);
        int l2max = min(N - 1 - s2, lmax);
        len1 = rand_int(lmin, l1max);
        len2 = rand_int(lmin, l2max);
    } while (hasIntersection(s1, s1 + len1 - 1, s2, s2 + len2 - 1));

    if (s1 > s2) {
        swap(s1,s2);
        swap(len1,len2);
    }

    int v1_f = s.sequence[s1];
    int v1_b = s.sequence[s1+len1-1];
    int v1_prev = s.sequence[s1-1];
    int v1_next = s.sequence[s1+len1];

    int v2_f = s.sequence[s2];
    int v2_b = s.sequence[s2+len2-1];
    int v2_prev = s.sequence[s2-1];
    int v2_next = s.sequence[s2+len2];

    double delta;
    if (v1_next == v2_f) {
        delta = dt.getDistance(v1_prev, v2_f) + dt.getDistance(v2_b, v1_f)
            + dt.getDistance(v1_b, v2_next) - dt.getDistance(v1_prev, v1_f)
            - dt.getDistance(v1_b, v2_f) - dt.getDistance(v2_b, v2_next);
    }
    else {
        delta = dt.getDistance(v1_prev, v2_f) + dt.getDistance(v2_b, v1_next)
                + dt.getDistance(v2_prev, v1_f) + dt.getDistance(v1_b, v2_next)
                - dt.getDistance(v1_prev, v1_f) - dt.getDistance(v1_b, v1_next)
                - dt.getDistance(v2_prev, v2_f) - dt.getDistance(v2_b, v2_next);
    }

    s.cost += delta;
    swap_subranges(s.sequence, s1, len1, s2, len2);
}

vector<int> chooseRandomNodes() {
    vector<int> sequence = { 1 };

    set<int> vis;
    vis.insert(1);

    // Escolhe 3 nós aleatórios para
    // compor a sequencia inicial
    while (sequence.size() < 4) {
        int node = rand_int(2, N);

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

bool bestImprovement2Opt(Data &dt, Solution &s) {
    double bestDelta = 0.0;
    int best_i = -1, best_j = -1;

    for (int i = 0; i < s.sequence.size() - 1; i++) {
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];

        for (int j = i + 2; j < s.sequence.size() - 1; j++) {
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            double delta = dt.getDistance(vi, vj) + dt.getDistance(vi_next, vj_next)
                        - dt.getDistance(vi, vi_next) - dt.getDistance(vj, vj_next);

            if (delta < bestDelta) {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0) {
        reverse(s.sequence.begin() + best_i + 1, s.sequence.begin() + best_j + 1);
        s.cost += bestDelta;
        return true;
    }

    return false;
}

bool bestImprovementOrOpt(Data &dt, Solution &s, int blockSize) {
    double bestDelta = 0.0;
    vector<int> best_block;
    int best_i = -1, best_j = -1;

    for (int i = 0; i < s.sequence.size() - blockSize - 1; i++) {
        vector<int> block;
        for (int k = 0; k < blockSize; k++) block.push_back(s.sequence[i+k+1]);

        int vi = s.sequence[i];
        int vi_next = s.sequence[i+blockSize+1];
        int vk_first = block[0];
        int vk_last = block.back();

        for (int j = 0; j < s.sequence.size() - 1; j++) {
            // pula indices dentro do bloco
            if (i == j) {
                j += blockSize;
                continue;
            }
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            double delta = dt.getDistance(vi, vi_next) + dt.getDistance(vj, vk_first)
            + dt.getDistance(vk_last, vj_next) - dt.getDistance(vi, vk_first)
            - dt.getDistance(vk_last, vi_next) - dt.getDistance(vj, vj_next);

            if (delta < bestDelta) {
                best_i = i;
                best_j = j;
                best_block = block;
                bestDelta = delta;
            }
        }
    }

    if (bestDelta < 0) {
        if (best_i < best_j) {
            s.sequence.insert(s.sequence.begin() + best_j + 1, best_block.begin(), best_block.end());
            s.sequence.erase(s.sequence.begin() + best_i + 1, s.sequence.begin() + best_i + blockSize + 1);
        }
        else {
            s.sequence.erase(s.sequence.begin() + best_i + 1, s.sequence.begin() + best_i + blockSize + 1);
            s.sequence.insert(s.sequence.begin() + best_j + 1, best_block.begin(), best_block.end());    
        }
        s.cost += bestDelta;
        return true;
    }

    return false;
}

void swap_subranges(vector<int> &v, int s1, int len1, int s2, int len2) {
    auto first_seg = v.begin() + s1;
    auto mid1 = first_seg + len1;
    auto second_seg = v.begin() + s2;
    auto mid2 = second_seg + len2;

    reverse(first_seg, mid1);
    reverse(mid1, second_seg);
    reverse(second_seg, mid2);
    reverse(first_seg, mid2);
}