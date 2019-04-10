#pragma once
#include "../ImprovementHeuristic.h"

class CAHImprovement : public ImprovementHeuristic {
public:
	Solutions improve(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriQua>> &offer_lists,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const std::vector<std::vector<int>> &distance_matrix,
		Solutions &previous_solutions
	) override;

	int newPurchasePlan(
		const std::vector<int> &tour,
		const std::vector<int> &demands,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		std::vector<std::vector<int>> &planTable
	);

	int newTravelRoute(
		int index,
		std::string method, 
		std::vector<int> &tour,
		const std::vector<std::vector<int>>& distance_matrix
	);
};

