#pragma once
#include "../Main/TPPCommon.h"
#include "../MyUtility/Solver.h"

class ImprovementHeuristic {
public:
	virtual Solutions improve(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriQua>> &offer_lists,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const std::vector<std::vector<int>> &distance_matrix,
		Solution &hisSolution,
		szx::Solver::Environment &env
	) = 0;
};
