#pragma once
#include "../common.h"

class ConstructionHeuristic {
public:
	virtual Solutions construct(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriQua>> &offer_lists,
		const std::vector<std::vector<int>> &distance_matrix
	) = 0;
};
