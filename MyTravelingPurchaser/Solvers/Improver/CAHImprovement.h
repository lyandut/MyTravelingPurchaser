#pragma once
#include "../ImprovementHeuristic.h"

class CAHImprovement : public ImprovementHeuristic {
public:
	Solutions improve(
		unsigned int dimension,
		const lyan::DemmandList& demands,
		const lyan::OfferList& offer_lists,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const lyan::DisMatrix& distance_matrix,
		Solution & hisSolution
	) override;

	int newPurchasePlan(
		const lyan::Tour &tour,
		const lyan::DemmandList &demands,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		lyan::PlanTable &planTable
	);

	int newTravelRoute(
		int nodeId,
		std::string method, 
		lyan::Tour &tour,
		const lyan::DisMatrix &distance_matrix
	);
};

