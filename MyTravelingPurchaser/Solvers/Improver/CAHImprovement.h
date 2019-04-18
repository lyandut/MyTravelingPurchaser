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
		Solution & hisSolution,
		szx::Solver::Environment &env
	) override;

	Solution marketDrop(
		unsigned int dimension,
		const lyan::DemmandList& demands,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const lyan::DisMatrix& distance_matrix,
		const Solution & hisSolution
	);

	Solution marketAdd(
		unsigned int dimension,
		const lyan::DemmandList& demands,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const lyan::DisMatrix& distance_matrix,
		const Solution & hisSolution
	);

	std::vector<int> alphaTabuTable;
	std::vector<int> betaTabuTable;
	Solution marketExchange(
		unsigned int dimension,
		const lyan::DemmandList& demands,
		const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
		const lyan::DisMatrix& distance_matrix,
		const Solution & hisSolution,
		int tabuIter,
		szx::Random rand
	);

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

