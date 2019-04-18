#pragma once
#include "../Main/TPPCommon.h"
#include "../MyUtility/Solver.h"

class ConstructionHeuristic {
public:
	virtual Solutions construct(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriQua>> &offer_lists,
		const std::vector<std::vector<int>> &distance_matrix,
		szx::Solver::Environment &env
	) = 0;
};
