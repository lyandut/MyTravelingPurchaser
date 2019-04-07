#pragma once
#include "../ConstructionHeuristic.h"

class GurobiSolver : public ConstructionHeuristic {
public:
	Solutions construct(
		unsigned int dimension,
		const std::vector<int>& demands,
		const std::vector<std::vector<PriQua>>& offer_lists,
		const std::vector<std::vector<int>>& distance_matrix
	) override;
};