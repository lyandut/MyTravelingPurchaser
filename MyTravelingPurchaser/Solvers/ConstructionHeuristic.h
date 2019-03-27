#pragma once
#include <vector>

typedef std::pair<std::vector<unsigned int>, int> Solution;
typedef std::vector<Solution> Solutions;


class ConstructionHeuristic {
public:
	virtual Solutions * construct(std::vector<std::vector<int>> distance_matrix, unsigned int n) = 0;
};
