#include <vector>

#include "MLP.hpp"
#include "solution.hpp"
#include "subsequence.hpp"

void MLP::update_all_subsequences(Solution &solution) {
    int n = solution.sequence.size();

    for (int i = 0; i < n; i++) {
        subsequence_matrix[i][i].W = (i > 0);
        subsequence_matrix[i][i].C = 0.0;
        subsequence_matrix[i][i].T = 0.0;
        subsequence_matrix[i][i].first = solution.sequence[i];
        subsequence_matrix[i][i].last = solution.sequence[i];
    }

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            subsequence_matrix[i][j] = concatenate(subsequence_matrix[i][j - 1], subsequence_matrix[j][j]);
            
    for (int i = n - 1; i >= 0; i--) 
        for (int j = i - 1; j >= 0; j--)
            subsequence_matrix[i][j] = concatenate(subsequence_matrix[i][j + 1], subsequence_matrix[j][j]);
}

void MLP::update_subsequences(Solution &solution, size_t start, size_t end) {
    int n = solution.sequence.size();

    for (int i = start; i <= end; i++) {
        subsequence_matrix[i][i].W = (i > 0);
        subsequence_matrix[i][i].C = 0.0;
        subsequence_matrix[i][i].T = 0.0;
        subsequence_matrix[i][i].first = solution.sequence[i];
        subsequence_matrix[i][i].last = solution.sequence[i];
    }

    for (int i = 0; i <= end; i++)
        for (int j = max(static_cast<int>(start), i + 1); j < n; j++)
            subsequence_matrix[i][j] = concatenate(subsequence_matrix[i][j - 1], subsequence_matrix[j][j]);
            
    for (int i = n - 1; i >= start; i--) 
        for (int j = min(static_cast<int>(end), i - 1); j >= 0; j--)
            subsequence_matrix[i][j] = concatenate(subsequence_matrix[i][j + 1], subsequence_matrix[j][j]);
}

Subsequence MLP::concatenate(Subsequence &sigma_1, Subsequence &sigma_2) {
    Subsequence sigma;

    double edge_cost = dist_matrix[sigma_1.last][sigma_2.first];
    sigma.W = sigma_1.W + sigma_2.W;
    sigma.T = sigma_1.T + edge_cost + sigma_2.T;
    sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + edge_cost) + sigma_2.C;
    sigma.first = sigma_1.first;
    sigma.last = sigma_2.last;

    return sigma;
}