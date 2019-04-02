#include "CAHImprovement.h"

Solutions CAHImprovement::improve(
	unsigned int dimension, 
	const std::vector<int>& demands, 
	const std::vector<std::vector<PriQua>>& offer_lists,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	const std::vector<std::vector<int>>& distance_matrix,
	Solutions & previous_solutions
) {
	Solutions result;  // result to return
	Solution hisSolution = previous_solutions[0];  // initial solution

	/* Market drop */
	Solution dropSolution = hisSolution;
	for (int i = 1; i < hisSolution.tour.size() - 1; i++) {
		// route cost
		auto newTour = hisSolution.tour;
		double newRouteCost = newTravelRoute(hisSolution.tour[i], "DROP", newTour, distance_matrix);
		// purchase cost
		std::vector<std::vector<int>> newPlanTable(dimension, std::vector<int>(demands.size(), 0));
		double newObjective = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
		if (newObjective < 0) continue;
		// total cost
		newObjective += newRouteCost;
		if (newObjective < dropSolution.opitimization) {
			dropSolution = { newObjective, newTour, newPlanTable };
		}
	}
	
	result.emplace_back(dropSolution);
	hisSolution = dropSolution;
	
	/* Market add */
	Solution addSolution = hisSolution;
	for (int i = 0; i < dimension; i++) {
		auto iter = find(hisSolution.tour.begin(), hisSolution.tour.end(), i);
		if (iter == hisSolution.tour.end()) {
			// route cost: add
			auto newTour = hisSolution.tour;
			double newRouteCost = newTravelRoute(i, "ADD", newTour, distance_matrix);
			// purchase cost
			std::vector<std::vector<int>> newPlanTable(dimension, std::vector<int>(demands.size(), 0));
			double newObjective = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
			// route cost: drop
			// Drop from the solution all markets where no purchase is made.
			std::vector<int> indexToDel;
			auto iter = newTour.begin() + 1;
			while (iter != newTour.end() - 1) {
				int h;
				for (h = 0; h < demands.size(); h++) {
					if (newPlanTable[*iter][h])
						break;
				}
				if (h == demands.size()) {
					indexToDel.emplace_back(*iter);
				}
				iter++;
			}
			for (int j = 0; j < indexToDel.size(); j++) {
				newRouteCost = newTravelRoute(indexToDel[j], "DROP", newTour, distance_matrix);
			}
			// total cost
			newObjective += newRouteCost;
			if (newObjective < addSolution.opitimization) {
				addSolution = { newObjective, newTour, newPlanTable };
			}
		}
	}

	result.emplace_back(addSolution);
	hisSolution = addSolution;

	/* Market exchange */


	/* TSP heuristic */

	
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

double CAHImprovement::newTravelRoute(
	int index, std::string method, 
	std::vector<int>& tour, 
	const std::vector<std::vector<int>>& distance_matrix
) {
	if (method == "DROP") {
		tour.erase(find(tour.begin(), tour.end(), index));
	}
	else if (method == "ADD") {
		int minRouteCost = INT_MAX;
		int indexToInsert = 0;
		for (int i = 0; i < tour.size() - 1; i++) {
			int front = tour[i], behind = tour[i + 1];
			int addRouteCost = distance_matrix[front][index] + distance_matrix[index][behind];
			int subRouteCost = distance_matrix[front][behind];
			if (minRouteCost > (addRouteCost - subRouteCost)) {
				minRouteCost = addRouteCost - subRouteCost;
				indexToInsert = i + 1;
			}
		}
		tour.insert(tour.begin() + indexToInsert, index);
	}

	double routeCost = 0.0;
	for (int j = 0; j < tour.size() - 1; j++) 
		routeCost += 1.0 * distance_matrix[tour[j]][tour[j + 1]];

	return routeCost;
}
