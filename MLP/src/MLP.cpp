#include <random>

#include "MLP.hpp"
#include "subsequence.hpp"

MLP::MLP(Data &instance) : m_instance(instance) {
    std::random_device rd;
    m_generator = std::mt19937(rd());

    int n = m_instance.getDimension();
    
    subsequence_matrix.resize(n+1, std::vector<Subsequence>(n+1));

    dist_matrix.resize(n+1, std::vector<double>(n+1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            dist_matrix[i][j] = m_instance.getDistance(i, j);
}