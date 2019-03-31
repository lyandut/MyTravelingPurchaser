#pragma once
#include <stdio.h>
#include <vector>


typedef std::pair<std::vector<unsigned int> *, int> Solution;
typedef std::vector<Solution *> Solutions;

class ImprovementHeuristic {
public:
	virtual Solutions *improve(int **distance_matrix, unsigned int n, Solutions *previous_solutions) = 0;
};
