#pragma once
#include "../ConstructionHeuristic.h"

class CAHSolver : public ConstructionHeuristic {
public:
	Solutions construct(
		unsigned int dimension, 
		const std::vector<int>& demands, 
		const std::vector<std::vector<PriQua>>& offer_lists, 
		const std::vector<std::vector<int>>& distance_matrix,
		szx::Solver::Environment &env
	) override;
	
	int insertTourSaving(
		int index,
		int objective,
		std::vector<int> &tour,
		std::vector<int> &isTravelled,
		std::vector<int> &isSatisfy,
		std::vector<std::vector<int>> &planTable,
		const std::vector<std::vector<int>> &distance_matrix,
		const std::vector<std::vector<PriQua>> &offer_lists
	);
};
