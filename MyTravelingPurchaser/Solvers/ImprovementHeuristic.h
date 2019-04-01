#pragma once
#include "../common.h"

class ImprovementHeuristic {
public:
	virtual Solutions improve(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriQua>> &offer_lists,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const std::vector<std::vector<int>> &distance_matrix,
		Solutions &previous_solutions
	) = 0;
};