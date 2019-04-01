#include "CAHImprovement.h"

Solutions CAHImprovement::improve(
	unsigned int dimension, 
	const std::vector<int>& demands, 
	const std::vector<std::vector<PriQua>>& offer_lists,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	const std::vector<std::vector<int>>& distance_matrix,
	Solutions & previous_solutions
) {
	Solution hisSolution = previous_solutions[0];

	/* Market drop */
	Solution newSolution = hisSolution;
	for (int i = 1; i < hisSolution.tour.size() - 1; i++) {
		auto newTour = hisSolution.tour;
		newTour.erase(newTour.begin() + i);
		std::vector<std::vector<int>> newPlanTable(dimension, std::vector<int>(demands.size(), 0));
		double newObjective = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
		// [TODO] 路程开销减少
		if (newObjective > 0 && newObjective < newSolution.opitimization) {
			newSolution.opitimization = newObjective;
			newSolution.planTable = newPlanTable;
			newSolution.tour = newTour;
		}
	}

	/* Market add */


	/* Market exchange */


	/* TSP heuristic */

	Solutions result;
	result.emplace_back(newSolution);
	return result;
}

double CAHImprovement::newPurchasePlan(
	const std::vector<int>& tour, 
	const std::vector<int>& demands, 
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	std::vector<std::vector<int>>& planTable
) {
	double objective = -1.0;
	auto isSatisfy = demands;
	int h = 0;
	while (h < isSatisfy.size()) {
		for (int i = 0; i < offer_sort_lists[h].size(); i++) {
			auto iter = find(tour.begin(), tour.end(), offer_sort_lists[h][i].nodeNo);
			if (iter != tour.end() && isSatisfy[h]) {  // tour中有可能满足需求的节点
				int iNode = offer_sort_lists[h][i].nodeNo;
				int iPrice = offer_sort_lists[h][i].price;
				int iQuantity = offer_sort_lists[h][i].quantity;
				if (iQuantity >= isSatisfy[h]) {
					objective += 1.0*isSatisfy[h] * iPrice;
					planTable[iNode][h] = isSatisfy[h];
					isSatisfy[h] = 0;
				}
				else {
					objective += 1.0*iQuantity * iPrice;
					planTable[iNode][h] = iQuantity;
					isSatisfy[i] -= iQuantity;
				}
			}
		}
		if (isSatisfy[h]) {  // tour中的节点无法满足购买需求
			return -1.0;
		}
		h++;
	}

	return objective;
}
