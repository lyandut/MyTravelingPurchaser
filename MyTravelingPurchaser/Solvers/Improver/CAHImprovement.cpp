#include "CAHImprovement.h"

Solutions CAHImprovement::improve(
	unsigned int dimension, 
	const lyan::DemmandList& demands,
	const lyan::OfferList& offer_lists,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	const lyan::DisMatrix& distance_matrix,
	Solution & hisSolution
) {
	Solutions result;  // result to return
	result.emplace_back(hisSolution);

	/* Market drop */
	Solution dropSolution = hisSolution;
	for (int i = 1; i < hisSolution.tour.size() - 1; i++) {
		int dropNode = hisSolution.tour[i];
		// route cost
		auto newTour = hisSolution.tour;
		int newRouteCost = newTravelRoute(dropNode, "DROP", newTour, distance_matrix);
		// purchase cost
		lyan::PlanTable newPlanTable(dimension, lyan::OfferTable(demands.size(), 0));
		int newPurchaseCost = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
		if (newPurchaseCost < 0) continue;
		// total cost
		int newObjective = newRouteCost + newPurchaseCost;
		if (newObjective < dropSolution.opitimization) {
			dropSolution = { newObjective, newTour, newPlanTable };
		}
	}
	
	result.emplace_back(dropSolution);
	hisSolution = dropSolution;
	
	/* Market add */
	Solution addSolution = hisSolution;
	for (int i = 0; i < dimension; i++) {
		auto hisIter = find(hisSolution.tour.begin(), hisSolution.tour.end(), i);
		if (hisIter != hisSolution.tour.end()) { continue; }
		// route cost: node add
		auto newTour = hisSolution.tour;
		int newRouteCost = newTravelRoute(i, "ADD", newTour, distance_matrix);
		// purchase cost
		lyan::PlanTable newPlanTable(dimension, lyan::OfferTable(demands.size(), 0));
		int newPurchaseCost = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
		// route cost: node drop
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
		int newObjective = newRouteCost + newPurchaseCost;
		if (newObjective < addSolution.opitimization) {
			addSolution = { newObjective, newTour, newPlanTable };
		}
	}

	result.emplace_back(addSolution);
	hisSolution = addSolution;

	/* Market exchange */
	Solution ExcSolution = hisSolution;


	/* TSP heuristic */

	
	return result;
}

int CAHImprovement::newPurchasePlan(
	const lyan::Tour &tour, 
	const lyan::DemmandList &demands,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	lyan::PlanTable &planTable
) {
	int purchaseCost = 0;
	auto isSatisfy = demands;
	int h = 0;
	while (h < demands.size()) {
		for (int i = 0; i < offer_sort_lists[h].size(); i++) {
			int iNode = offer_sort_lists[h][i].nodeNo;
			int iPrice = offer_sort_lists[h][i].price;
			int iQuantity = offer_sort_lists[h][i].quantity;
			// 节点iNode在tour中，且需求h未被满足
			auto iter = find(tour.begin(), tour.end(), iNode);
			if (iter != tour.end() && isSatisfy[h]) { 
				if (iQuantity >= isSatisfy[h]) {  // 供大于求
					purchaseCost += isSatisfy[h] * iPrice;
					planTable[iNode][h] = isSatisfy[h];
					isSatisfy[h] = 0;
				}
				else { // 供不应求
					purchaseCost += iQuantity * iPrice;
					planTable[iNode][h] = iQuantity;
					isSatisfy[h] -= iQuantity;
				}
			}
		}
		// tour中的节点无法满足购买需求
		if (isSatisfy[h]) {  return -1; }
		h++;
	}

	return purchaseCost;
}

int CAHImprovement::newTravelRoute(
	int nodeId, std::string method, 
	lyan::Tour &tour, 
	const lyan::DisMatrix &distance_matrix
) {
	if (method == "DROP") {
		tour.erase(find(tour.begin(), tour.end(), nodeId));
	}
	else if (method == "ADD") {
		int minRouteCost = INT_MAX;
		int indexToInsert = 0;
		for (int i = 0; i < tour.size() - 1; i++) {
			int front = tour[i], behind = tour[i + 1];
			int addRouteCost = distance_matrix[front][nodeId] + distance_matrix[nodeId][behind];
			int subRouteCost = distance_matrix[front][behind];
			if (minRouteCost > (addRouteCost - subRouteCost)) {
				minRouteCost = addRouteCost - subRouteCost;
				indexToInsert = i + 1;
			}
		}
		tour.insert(tour.begin() + indexToInsert, nodeId);
	}

	int routeCost = 0;
	for (int j = 0; j < tour.size() - 1; j++) 
		routeCost += distance_matrix[tour[j]][tour[j + 1]];

	return routeCost;
}
