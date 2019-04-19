#include "Perturbation.h"
//#include "../../Checker/MyTPPChecker.h"

void Perturbation::improve() {
	//MyTPPChecker tpp_checker = MyTPPChecker();

	/* tabu search */
	while (tabuIter != STOPCONDITION) {
		//tpp_checker.myTppChecker(sln, dimension, demands, offer_lists, distance_matrix);
		std::map<int, int> deltaMap;
		std::map<int, int> tabuDeltaMap;
		for (int i = 1; i < dimension; i++) {
			/* Market drop */
			if (isTravelled[i]) {
				if (dropTabuTable[i] < tabuIter) {
					marketDrop(i, deltaMap);
				}
				else {
					marketDrop(i, tabuDeltaMap);
				}
			}
			/* Market add */
			else {
				if (addTabuTable[i] < tabuIter) {
					marketAdd(i, deltaMap);
				}
				else {
					marketAdd(i, tabuDeltaMap);
				}
			}
		}
		/* sort the deltaMap */
		std::vector<Pair<int, int>> vec = mapSortByValue(deltaMap);
		std::vector<Pair<int, int>> tabuVec = mapSortByValue(tabuDeltaMap);

		/* TSP heuristic */
		// 探测结果前10%用LKH修复求精确路由
		std::map<ID, Solution> results;
		if (vec.empty()) {
			int len = tabuVec.size() * 0.1;
			for (auto iter = tabuVec.begin(); iter != tabuVec.begin() + len; iter++) {
				Solution curSln; curSln.opitimization = 0;
				int nodeId = (*iter).first;
				auto newTourSet = sln.tour;
				newTourSet.erase(newTourSet.end() - 1);
				if (isTravelled[nodeId]) {
					newTourSet.erase(find(newTourSet.begin(), newTourSet.end(), nodeId));
				}
				else {
					newTourSet.push_back(nodeId);
				}
				tspHeuristic(newTourSet, curSln);
				updatePlanTable(newTourSet, curSln);
				results[nodeId] = curSln;
			}
		}
		else {
			int len = vec.size() * 0.1;
			for (auto iter = vec.begin(); iter != vec.begin() + len; iter++) {
				Solution curSln; curSln.opitimization = 0;
				int nodeId = (*iter).first;
				auto newTourSet = sln.tour;
				newTourSet.erase(newTourSet.end() - 1);
				if (isTravelled[nodeId]) {
					newTourSet.erase(find(newTourSet.begin(), newTourSet.end(), nodeId));
				}
				else {
					newTourSet.push_back(nodeId);
				}
				tspHeuristic(newTourSet, curSln);
				updatePlanTable(newTourSet, curSln);
				results[nodeId] = curSln;
			}
		}
		makeMove(results);
		++tabuIter;
	}
}

bool Perturbation::marketDrop(int dropNode, std::map<int, int> &costDelta) {
	int routeDelta = 0, purDelta = 0;
	// Purchase delta
	std::vector<int> dropProduct;
	for (int k = 0; k < sln.planTable[dropNode].size(); k++) {
		if (!sln.planTable[dropNode][k]) { continue; }
		if (totalQuantity[k] - offer_lists[dropNode][k].second < demands[k]) { return false; }
		dropProduct.push_back(k);
	}
	for (int k : dropProduct) {
		int dropDemand = sln.planTable[dropNode][k];
		int dropPrice = offer_lists[dropNode][k].first;
		purDelta -= dropDemand * dropPrice;
		for (auto nodeInfo : ascList[k]) {
			int iNode = nodeInfo.nodeNo;
			if (iNode == dropNode) { continue; }
			int iQuantity = nodeInfo.quantity - sln.planTable[iNode][k];
			int iPrice = nodeInfo.price;
			if (iQuantity >= dropDemand) {
				purDelta += dropDemand * iPrice;
				dropDemand = 0;
			}
			else {
				purDelta += iQuantity * iPrice;
				dropDemand -= iQuantity;
			}
			if (!dropDemand) { break; }
		}
	}
	// Route delta
	auto dropIter = find(sln.tour.begin(), sln.tour.end(), dropNode);
	int front = *(dropIter - 1);
	int behind = *(dropIter + 1);
	routeDelta = distance_matrix[front][behind] - distance_matrix[front][dropNode] - distance_matrix[dropNode][behind];

	costDelta[dropNode] = routeDelta + purDelta;  // 估算路由代价 + 精确购买开销
	return true;
}

bool Perturbation::marketAdd(int addNode, std::map<int, int> &costDelta) {
	int routeDelta = INT_MAX, purDelta = 0;
	// Purchase delta
	auto newTotalKind = totalProKinds;
	for (int k = 0; k < offer_lists[addNode].size(); k++) {
		int addQuantity = offer_lists[addNode][k].second;
		if (!addQuantity) { continue; }
		int addPrice = offer_lists[addNode][k].first;
		for (auto nodeInfo : descList[k]) {
			int iNode = nodeInfo.nodeNo;
			int iPrice = nodeInfo.price;
			if (iPrice <= addPrice) { break; }
			int iQuantity = sln.planTable[iNode][k];
			if (!iQuantity) { continue; }
			// (iPrice > addPrice && iQuantity)
			if (iQuantity <= addQuantity) { // 供大于求
				purDelta += iQuantity * (addPrice - iPrice);
				addQuantity -= iQuantity;
				newTotalKind[addNode] += 1;
				newTotalKind[iNode] -= 1;
			}
			else { // 供不应求
				purDelta += addQuantity * (addPrice - iPrice);
				addQuantity = 0;
				newTotalKind[addNode] += 1;
			}
			if (!addQuantity) { break; }
		}
	}
	if (!newTotalKind[addNode]) { return false; }
	// Route delta
	for (int i = 0; i < sln.tour.size() - 1; i++) {
		int front = sln.tour[i], behind = sln.tour[i + 1];
		int addRouteCost = distance_matrix[front][addNode] + distance_matrix[addNode][behind];
		int subRouteCost = distance_matrix[front][behind];
		if (routeDelta > (addRouteCost - subRouteCost)) {
			routeDelta = addRouteCost - subRouteCost;
		}
	}
	// Drop from the solution all markets where no purchase is made.
	for (int i = 1; i < sln.tour.size() - 1; i++) {
		int front = sln.tour[i - 1], curr = sln.tour[i], behind = sln.tour[i + 1];
		if (!newTotalKind[curr]) {
			routeDelta -= distance_matrix[front][curr];
			routeDelta -= distance_matrix[curr][behind];
			routeDelta += distance_matrix[front][behind];
		}
	}

	costDelta[addNode] = routeDelta + purDelta;  // 估算路由代价 + 精确购买开销
	return true;
}

void Perturbation::tspHeuristic(std::vector<int> nodeIdMap, Solution &curSln) {

	List<bool> containNode(dimension);
	fill(containNode.begin(), containNode.end(), false);
	lkh::Tour lkhTour;

	int mapSize = nodeIdMap.size();
	lkh::AdjMat adjMat(mapSize, mapSize);
	for (int i = 0; i < mapSize; i++) {
		int iNode = nodeIdMap[i];
		containNode[iNode] = true;
		for (int j = 0; j < mapSize; j++) {
			int jNode = nodeIdMap[j];
			adjMat.at(i, j) = distance_matrix[iNode][jNode];
		}
	}
	if (mapSize > 2) { // repair the relaxed solution.
		// [&](ID n) { return nodeIdMap[n]; } 返回第n个节点的ID
		tspSolver.solve(lkhTour, containNode, adjMat, [&](ID n) { return nodeIdMap[n]; });
	}
	else if (mapSize == 2) { // trivial cases.
		lkhTour.nodes.resize(2);
		lkhTour.nodes[0] = nodeIdMap[0];
		lkhTour.nodes[1] = nodeIdMap[1];
	}
	else { return; }

	lkhTour.nodes.push_back(lkhTour.nodes.front());
	curSln.opitimization += lkhTour.distance;
	curSln.tour = lkhTour.nodes;
}

void Perturbation::updatePlanTable(std::vector<int> newTourSet, Solution & curSln) {
	int purchaseCost = 0;
	PlanTable newPlanTable(dimension, OfferTable(demandNum, 0));
	auto isSatisfy = demands;
	int h = 0;
	while (h < demandNum) {
		for (int i = 0; i < offer_sort_lists[h].size(); i++) {
			int iNode = offer_sort_lists[h][i].nodeNo;
			int iPrice = offer_sort_lists[h][i].price;
			int iQuantity = offer_sort_lists[h][i].quantity;
			auto iter = find(newTourSet.begin(), newTourSet.end(), iNode);
			// 节点iNode在tourSet中，且需求h未被满足
			if (iter != newTourSet.end() && isSatisfy[h]) {
				if (iQuantity >= isSatisfy[h]) {  // 供大于求
					purchaseCost += isSatisfy[h] * iPrice;
					newPlanTable[iNode][h] = isSatisfy[h];
					isSatisfy[h] = 0;
				}
				else { // 供不应求
					purchaseCost += iQuantity * iPrice;
					newPlanTable[iNode][h] = iQuantity;
					isSatisfy[h] -= iQuantity;
				}
			}
		}
		h++;
	}
	curSln.opitimization += purchaseCost;
	curSln.planTable = newPlanTable;
}

void Perturbation::makeMove(std::map<ID, Solution> &results) {
	int moveId = 0;
	Solution bestSln = sln;
	for (auto slnIter : results) {
		if (slnIter.second.opitimization <= bestSln.opitimization) {
			moveId = slnIter.first;
			bestSln = slnIter.second;
		}
	}
	if (!moveId) { return; }
	// tabu tenure
	int tourSize = sln.tour.size();
	if (isTravelled[moveId]) {  // dropNode
		addTabuTable[moveId] = ADDTABUTENURE;
	}
	else { // addNode
		dropTabuTable[moveId] = DROPTABUTENURE;
	}
	// make move
	sln = bestSln;
	isTravelled.clear();	isTravelled.resize(dimension, 0);
	for (int tourNode : sln.tour) {
		isTravelled[tourNode] = 1;
	}
	ascList.clear();	ascList.resize(demandNum);
	descList.clear();	descList.resize(demandNum);
	totalQuantity.clear();	totalQuantity.resize(demandNum, 0);
	totalProKinds.clear();	totalProKinds.resize(dimension, 0);
	for (int k = 0; k < offer_sort_lists.size(); k++) {
		for (auto offerInfo : offer_sort_lists[k]) {
			if (isTravelled[offerInfo.nodeNo]) {
				ascList[k].emplace_back(offerInfo);
				descList[k].emplace(descList[k].begin(), offerInfo);
				totalQuantity[k] += offerInfo.quantity;
				totalProKinds[offerInfo.nodeNo] += 1;
			}
		}
	}
}


