#include <iostream>

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

	double s = tsp.solve(branching_strategy);
	std::cout << "Objective val: " << s << '\n';

	for (int i = 0; i < n; i++) delete [] cost[i];
	delete [] cost;

	return 0;
}
