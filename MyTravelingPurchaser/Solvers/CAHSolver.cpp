#include "CAHSolver.h"

Solutions CAHSolver::construct(
	unsigned int dimension,
	const std::vector<int> &demands,
	const std::vector<std::vector<PriAva>> &offer_lists,
	const std::vector<std::vector<int>> &distance_matrix
) {
	double objective = 0.0;
	auto isSatisfy = demands;
	std::vector<int> tour;
	std::vector<int> isTravelled(dimension, 0);
	std::vector<std::vector<int>> planTable(dimension, std::vector<int>(isSatisfy.size(), 0));

	/* 1. Initialization */
	// [TODO] isSatisfy 随机排序
	int h = 0;  // 产品编号 -> 需求量: isSatisfy[h], isSatisfy[h] = 0 已满足需求 
	isTravelled[0] = 1;
	tour.emplace_back(0);

	/* 2. First units of product h=0 */
	int minIndex;
	double minCost = -1.0;
	for (int i = 1; i < dimension; i++) {
		int iPrice = offer_lists[i][h].first;
		int iQuantity = offer_lists[i][h].second;
		if (!iQuantity) continue;
		double currCost = 2.0*distance_matrix[0][i] / (double)iQuantity + (double)iPrice;
		if (currCost < minCost || minCost == -1.0) {
			minCost = currCost;
			minIndex = i;
		}
	}    
	isTravelled[minIndex] = 1;    // 前进一步
	tour.emplace_back(minIndex);
	tour.emplace_back(0);         // 闭环 { 0, minIndex, 0 }
	// 计算目标函数值 objective，更新购买计划表 planTable
	objective += 2.0*distance_matrix[0][minIndex];
	for (int i = 0; i < offer_lists[minIndex].size(); i++) {
		int iPrice = offer_lists[minIndex][i].first;
		int iQuantity = offer_lists[minIndex][i].second;
		if (!iQuantity) continue;
		if (iQuantity >= isSatisfy[i]) {
			objective += 1.0*isSatisfy[i] * iPrice;
			planTable[minIndex][i] = isSatisfy[i];
			isSatisfy[i] = 0;
		}
		else {
			objective += 1.0*iQuantity * iPrice;
			planTable[minIndex][i] = iQuantity;
			isSatisfy[i] -= iQuantity;
		}
	}

	/* 3. Remaining units of product h=0 */
	while (isSatisfy[h]) {
		for (int i = 0; i < dimension; i++) {
			if (isTravelled[i]) continue;
			if (!offer_lists[i][h].second) continue;
			// insert i into tour
			auto tour_tmp = tour;
			auto isTravelled_tmp = isTravelled;
			auto isSatisfy_tmp = isSatisfy;
			auto planTable_tmp = planTable;
			double objective_tmp = insertTourSaving(
				i, objective, 
				tour_tmp, isTravelled_tmp, 
				isSatisfy_tmp, planTable_tmp,
				distance_matrix, offer_lists
			);
		   /* 
		    * If product h is not yet fully purchased, 
		    * Or if it is fully purchased and objective_new < objective_old
		 	*/
			if (isSatisfy[h] || (!isSatisfy[h] && objective > objective_tmp)) {
				objective = objective_tmp;
				tour = tour_tmp;
				isTravelled = isTravelled_tmp;
				isSatisfy = isSatisfy_tmp;
				planTable = planTable_tmp;
			}
		}
	}

	/* 4. Termination test */
	while (h < isSatisfy.size()) {
		while (isSatisfy[h]) {
			for (int i = 0; i < dimension; i++) {
				if (isTravelled[i]) continue;
				if (!offer_lists[i][h].second) continue;
				// insert i into tour
				auto tour_tmp = tour;
				auto isTravelled_tmp = isTravelled;
				auto isSatisfy_tmp = isSatisfy;
				auto planTable_tmp = planTable;
				double objective_tmp = insertTourSaving(
					i, objective,
					tour_tmp, isTravelled_tmp,
					isSatisfy_tmp, planTable_tmp,
					distance_matrix, offer_lists
				);
			   /* 
			    * If product h is not yet fully purchased,
				* Or if it is fully purchased and objective_new < objective_old
				*/
				if (isSatisfy[h] || (!isSatisfy[h] && objective > objective_tmp)) {
					objective = objective_tmp;
					tour = tour_tmp;
					isTravelled = isTravelled_tmp;
					isSatisfy = isSatisfy_tmp;
					planTable = planTable_tmp;
				}
			}
		}
		h++;
	}

	std::cout << "Termination" << std::endl;
	Solution sol;
	sol.opitimization = objective;
	sol.tour = tour;
	sol.planTable = planTable;

	/* return */
	Solutions result;
	result.emplace_back(sol);
	return result;
}

double CAHSolver::insertTourSaving(
	int index,
	double objective,
	std::vector<int> &tour,
	std::vector<int> &isTravelled,
	std::vector<int> &isSatisfy,
	std::vector<std::vector<int>> &planTable,
	const std::vector<std::vector<int>> &distance_matrix,
	const std::vector<std::vector<PriAva>> &offer_lists
){
	// purchase cost
	for (int i = 0; i < offer_lists[index].size(); i++) {
		int iPrice = offer_lists[index][i].first;
		int iQuantity = offer_lists[index][i].second;
		if (!iQuantity) continue;
		if (isSatisfy[i]) { // 未满足需求，直接购买
			if (iQuantity >= isSatisfy[i]) {
				objective += 1.0*isSatisfy[i] * iPrice;
				planTable[index][i] = isSatisfy[i];
				isSatisfy[i] = 0;
			}
			else {
			 	objective += 1.0*iQuantity * iPrice;
				planTable[index][i] = iQuantity;
				isSatisfy[i] -= iQuantity;
			}		
		}
		else { // 已满足需求，重新分配购买方案
			for (int j = 0; j < tour.size(); j++) {
				int jQuantity = planTable[tour[j]][i];
				int jPrice = offer_lists[tour[j]][i].first;
				if (jQuantity && jPrice > iPrice) {
					if (jQuantity > iQuantity) {
						objective -= 1.0*iQuantity*jPrice;
						objective += 1.0*iQuantity*iPrice;
						planTable[tour[j]][i] = jQuantity - iQuantity;
						planTable[index][i] = iQuantity;
					}
					else {
						objective -= 1.0*jQuantity*jPrice;
						objective += 1.0*jQuantity*iPrice;
						planTable[tour[j]][i] = 0;
						planTable[index][i] = jQuantity;
					}
				}
			}
		}
	}

	// route cost
	int minRouteCost = INT_MAX;
	int indexToInsert = 0;
	for (int i = 0; i < tour.size() - 1; i++) {
		int front = tour[i], behind = tour[i + 1];
		int addRouteCost = distance_matrix[front][index] + distance_matrix[index][behind];
		int subRouteCost = distance_matrix[front][behind];
		// 实际情况下可能不符合三角形原则，即 minRouteCost < 0
		if (minRouteCost > (addRouteCost - subRouteCost)) {
			minRouteCost = addRouteCost - subRouteCost;
			indexToInsert = i +	1;
		}
	}
	
	objective += (double)minRouteCost;
	tour.insert(tour.begin() + indexToInsert, index);
	isTravelled[index] = 1;

	return objective;
}
