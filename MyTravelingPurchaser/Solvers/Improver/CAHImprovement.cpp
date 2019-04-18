#include "CAHImprovement.h"

Solutions CAHImprovement::improve(
	unsigned int dimension, 
	const lyan::DemmandList& demands,
	const lyan::OfferList& offer_lists,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	const lyan::DisMatrix& distance_matrix,
	Solution & hisSolution,
	szx::Solver::Environment &env
) {
	Solutions result;  // result to return

	/* Market drop */
	Solution dropSolution = marketDrop(dimension, demands, offer_sort_lists, distance_matrix, hisSolution);
	result.emplace_back(dropSolution);
	hisSolution = dropSolution;

	
	/* Market add */
	Solution addSolution = marketDrop(dimension, demands, offer_sort_lists, distance_matrix, hisSolution);
	result.emplace_back(addSolution);
	hisSolution = addSolution;

	/* Market exchange */
	int tabuIter = 0;
	alphaTabuTable = std::vector<int>(dimension, -1);
	betaTabuTable = std::vector<int>(dimension, -1);
	szx::Random rand(env.randSeed);
	while (tabuIter != STOPCONDITION) {
		hisSolution = marketExchange(
			dimension, demands, offer_sort_lists, distance_matrix, hisSolution, tabuIter, rand
		);
		++tabuIter;
	}

	/* TSP heuristic */

	
	return result;
}

Solution CAHImprovement::marketDrop(
	unsigned int dimension,
	const lyan::DemmandList& demands,
	const std::vector<std::vector<NodePriQua>> &offer_sort_lists,
	const lyan::DisMatrix& distance_matrix,
	const Solution & hisSolution
) {
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
	return dropSolution;
}

Solution CAHImprovement::marketAdd(
	unsigned int dimension, 
	const lyan::DemmandList & demands, 
	const std::vector<std::vector<NodePriQua>>& offer_sort_lists, 
	const lyan::DisMatrix & distance_matrix, 
	const Solution & hisSolution
) {
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
	return addSolution;
}

Solution CAHImprovement::marketExchange(
	unsigned int dimension, 
	const lyan::DemmandList & demands, 
	const std::vector<std::vector<NodePriQua>>& offer_sort_lists, 
	const lyan::DisMatrix & distance_matrix, 
	const Solution & hisSolution,
	int tabuIter,
	szx::Random rand
) {
	// generate all of the exchange pairs.
	lyan::ExcPairList allExcPair;
	for (int i = 1; i < hisSolution.tour.size() - 1; i++) {
		int dropNode = hisSolution.tour[i];
		for (int j = 0; j < dimension; j++) {
			int addNode = j;
			auto hisIter = find(hisSolution.tour.begin(), hisSolution.tour.end(), addNode);
			if (hisIter != hisSolution.tour.end()) { continue; }
			allExcPair.emplace_back(dropNode, addNode);
		}
	}
	// traverse all of the exchange pairs.
	Solutions L, tabuL;
	Solution excSolution = hisSolution, tabuExcSolution = hisSolution;
	lyan::ExcPairList excPair, tabuExcPair;
	for (lyan::ExcPair eachPair : allExcPair) {
		int dropNode = eachPair.first;
		int addNode = eachPair.second;
		auto newTour = hisSolution.tour;
		// route cost: node drop
		int newRouteCost = newTravelRoute(dropNode, "DROP", newTour, distance_matrix);
		// route cost: node add
		newRouteCost = newTravelRoute(addNode, "ADD", newTour, distance_matrix);
		// purchase cost
		lyan::PlanTable newPlanTable(dimension, lyan::OfferTable(demands.size(), 0));
		int newPurchaseCost = newPurchasePlan(newTour, demands, offer_sort_lists, newPlanTable);
		if (newPurchaseCost < 0) continue;
		// route cost: node drop
		// Drop from the solution all markets where no purchase is made.
		std::vector<int> indexToDel;
		for (auto iter = newTour.begin() + 1; iter != newTour.end() - 1; iter++) {
			int h;
			for (h = 0; h < demands.size(); h++) {
				if (newPlanTable[*iter][h])
					break;
			}
			if (h == demands.size()) {
				indexToDel.emplace_back(*iter);
			}
		}
		for (auto each : indexToDel) {
			newRouteCost = newTravelRoute(each, "DROP", newTour, distance_matrix);
		}
		// total cost
		int newObjective = newRouteCost + newPurchaseCost;

		// 禁忌策略
		if (alphaTabuTable[addNode] <= tabuIter && betaTabuTable[dropNode] <= tabuIter) { /* 解禁 */
			if (newObjective < excSolution.opitimization) {
				excSolution = { newObjective, newTour, newPlanTable };
				L.clear();	excPair.clear();
				L.emplace_back(excSolution);
				excPair.emplace_back(eachPair);
			}
			else if (newObjective == excSolution.opitimization) {
				L.emplace_back(newObjective, newTour, newPlanTable);
				excPair.emplace_back(eachPair);
			}
		}
		else { /* 禁忌 */
			if (newObjective < tabuExcSolution.opitimization) {
				tabuExcSolution = { newObjective, newTour, newPlanTable };
				tabuL.clear();	tabuExcPair.clear();
				tabuL.emplace_back(tabuExcSolution);
				tabuExcPair.emplace_back(eachPair);
			}
			else if (newObjective == tabuExcSolution.opitimization) {
				tabuL.emplace_back(newObjective, newTour, newPlanTable);
				tabuExcPair.emplace_back(eachPair);
			}
		}
	}
	// 从最优解中随机返回一个，并将其属性禁忌
	if (!tabuL.size() && !L.size()) { return hisSolution; }
	int randIndex, dropNode, addNode;
	Solution resSolution;
	if (tabuL.size() && tabuExcSolution.opitimization < excSolution.opitimization || !L.size()) { // 特赦规则
		randIndex = rand.pick(tabuL.size());
		dropNode = tabuExcPair[randIndex].first;
		addNode = tabuExcPair[randIndex].second;
		resSolution = tabuL[randIndex];
	}
	else {
		int randIndex = rand.pick(L.size());
		int dropNode = excPair[randIndex].first;
		int addNode = excPair[randIndex].second;
		resSolution = L[randIndex];
	}
	int tourSize = resSolution.tour.size();
	alphaTabuTable[addNode] = ALPHATABUTENURE;
	betaTabuTable[dropNode] = BETATABUTENURE;
	return resSolution;
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
