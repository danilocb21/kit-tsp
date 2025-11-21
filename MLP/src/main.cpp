#include <cstddef>
#include <iostream>

#include "data.h"
#include "MLP.hpp"
#include "solution.hpp"

int main(int argc, char **argv) {
    srand(time(nullptr));
    
    auto data = Data(argc, argv[1]);
    data.read();

    MLP mlp = MLP(data);

    size_t max_iterations = 10;
    size_t max_ils_iterations = min(mlp.dist_matrix.size(), static_cast<size_t>(100));
    double alpha = 0.1; // random alpha from list

    Solution s = mlp.GILS_RNVD(max_iterations, max_ils_iterations, alpha);
    s.print();

    return 0;
}
