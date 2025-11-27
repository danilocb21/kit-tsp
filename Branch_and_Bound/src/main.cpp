#include <iostream>
#include <chrono>

#include "data.h"
#include "hungarian.h"
#include "BnB.hpp"

int main(int argc, char** argv) {

	Data data = Data(argc, argv[1]);
	data.read();

	size_t n = data.getDimension();

	double **cost = new double*[n];
	for (int i = 0; i < n; i++){
		cost[i] = new double[n];
		for (int j = 0; j < n; j++){
			cost[i][j] = data.getDistance(i+1,j+1);
		}
	}

	BnB tsp(data, cost);

	std::string strategy = argv[2];
	BnB::BranchingStrategy branching_strategy;
	
	if (strategy == "DFS")
		branching_strategy = BnB::BranchingStrategy::DFS;
	else if (strategy == "BFS")
		branching_strategy = BnB::BranchingStrategy::BFS;
	else
		branching_strategy = BnB::BranchingStrategy::MLB;

	
	uint8_t runs = 10;
    double total_time = 0.0, total_cost = 0.0;

    for (uint8_t i = 0; i < runs; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        double s = tsp.solve(branching_strategy);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        total_time += duration.count();
        total_cost += s;
    }
    
    std::cout << std::fixed << total_time / runs << ' ' << total_cost / runs << "\n\n";	

	for (int i = 0; i < n; i++) delete [] cost[i];
	delete [] cost;

	return 0;
}
