#include "CAHSolver.h"
#include <cmath>

Solutions CAHSolver::construct(
	unsigned int dimension,
	const std::vector<int> &demands,
	const std::vector<std::vector<PriQua>> &offer_lists,
	const std::vector<std::vector<int>> &distance_matrix
) {
	int objective = 0;
	auto isSatisfy = demands;
	int demandsNum = isSatisfy.size();
	std::vector<int> tour;
	tour.reserve(dimension);
	std::vector<int> isTravelled(dimension, 0);
	std::vector<std::vector<int>> planTable(dimension, std::vector<int>(demandsNum, 0));

	/* 1. Initialization */
	// [TODO] isSatisfy 随机排序
	int h = 0;  // 产品编号 -> 需求量: isSatisfy[h], isSatisfy[h] = 0 已满足需求 
	isTravelled[0] = 1;
	tour.emplace_back(0);

	/* 2. First units of product h=0 */
	int minIndex;
	int minCost = INT_MAX;
	for (int i = 1; i < dimension; i++) {
		int iPrice = offer_lists[i][h].first;
		int iQuantity = offer_lists[i][h].second;
		if (!iQuantity) continue;
		int currCost = std::lround(2.0*distance_matrix[0][i] / 1.0*iQuantity) + iPrice;
		if (currCost < minCost) {
			minCost = currCost;
			minIndex = i;
		}
	}    
	isTravelled[minIndex] = 1;    // 前进一步
	tour.emplace_back(minIndex);
	tour.emplace_back(0);         // 闭环 { 0, minIndex, 0 }
	/* 从 minIndex 节点购买所有种类的产品
	 * 计算目标函数值 objective，更新购买计划表 planTable
	 */
	objective += 2 * distance_matrix[0][minIndex];
	for (int h = 0; h < offer_lists[minIndex].size(); h++) {
		int iPrice = offer_lists[minIndex][h].first;
		int iQuantity = offer_lists[minIndex][h].second;
		if (!iQuantity) continue;
		if (iQuantity >= isSatisfy[h]) {
			objective += isSatisfy[h] * iPrice;
			planTable[minIndex][h] = isSatisfy[h];
			isSatisfy[h] = 0;
		}
		else {
			objective += iQuantity * iPrice;
			planTable[minIndex][h] = iQuantity;
			isSatisfy[h] -= iQuantity;
		}
	}

	/* 3. Remaining units of product h */
	/* 4. Termination test */
	while (h < demandsNum) {
		while (isSatisfy[h]) {
			for (int i = 0; i < dimension; i++) {
				if (isTravelled[i]) continue;
				if (!offer_lists[i][h].second) continue;
				// insert i into tour
				auto tour_tmp = tour;
				auto isTravelled_tmp = isTravelled;
				auto isSatisfy_tmp = isSatisfy;
				auto planTable_tmp = planTable;
				int objective_tmp = insertTourSaving(
					i, objective,
					tour_tmp, isTravelled_tmp,
					isSatisfy_tmp, planTable_tmp,
					distance_matrix, offer_lists
				);
			   /* 
			    * If product h is not yet fully purchased,
				* Or if it is fully purchased and objective_new < objective_old
				*/
				if (isSatisfy[h] || (!isSatisfy[h] && objective_tmp < objective)) {
					objective = objective_tmp;
					tour.swap(tour_tmp);
					isTravelled.swap(isTravelled_tmp);
					isSatisfy.swap(isSatisfy_tmp);
					planTable.swap(planTable_tmp);
				}
			}
		}
		h++;
	}

	Solution sol;
	sol.opitimization = objective;
	sol.tour.swap(tour);
	sol.planTable.swap(planTable);

	/* return */
	Solutions result;
	result.emplace_back(sol);
	return result;
}

int CAHSolver::insertTourSaving(
	int index,
	int objective,
	std::vector<int> &tour,
	std::vector<int> &isTravelled,
	std::vector<int> &isSatisfy,
	std::vector<std::vector<int>> &planTable,
	const std::vector<std::vector<int>> &distance_matrix,
	const std::vector<std::vector<PriQua>> &offer_lists
){
	// purchase cost
	for (int h = 0; h < offer_lists[index].size(); h++) {
		int iPrice = offer_lists[index][h].first;
		int iQuantity = offer_lists[index][h].second;
		if (!iQuantity) continue;
		if (isSatisfy[h]) { // 未满足需求，直接购买
			if (iQuantity >= isSatisfy[h]) {
				objective += isSatisfy[h] * iPrice;
				planTable[index][h] = isSatisfy[h];
				isSatisfy[h] = 0;
			}
			else {
			 	objective += iQuantity * iPrice;
				planTable[index][h] = iQuantity;
				isSatisfy[h] -= iQuantity;
			}		
		}
		else { // 已满足需求，重新分配产品 h 的购买方案
			for (int j = 1; j < tour.size() - 1; j++) {
				int jNode = tour[j];
				int jQuantity = planTable[jNode][h];
				int jPrice = offer_lists[jNode][h].first;
				if (iQuantity && jQuantity && jPrice > iPrice) { // 用iQuantity置换jQuantity
					if (jQuantity > iQuantity) {  // 供不应求
						objective -= iQuantity * jPrice;
						objective += iQuantity * iPrice;
						planTable[jNode][h] -= iQuantity;
						planTable[index][h] += iQuantity;
						iQuantity = 0;
					}
					else { // 供过于求
						objective -= jQuantity * jPrice;
						objective += jQuantity * iPrice;
						planTable[jNode][h] -= jQuantity;
						planTable[index][h] += jQuantity;
						iQuantity -= jQuantity;
					}
				}
			}
		}
	}

	// route cost
	int indexToInsert;
	int minRouteCost = INT_MAX;
	for (int i = 0; i < tour.size() - 1; i++) {
		int front = tour[i], behind = tour[i + 1];
		int addRouteCost = distance_matrix[front][index] + distance_matrix[index][behind];
		int subRouteCost = distance_matrix[front][behind];
		// 算例可能不满足三角形不等式，即 minRouteCost < 0
		if (minRouteCost > (addRouteCost - subRouteCost)) {
			minRouteCost = addRouteCost - subRouteCost;
			indexToInsert = i +	1;
		}
	}
	
	objective += minRouteCost;
	tour.insert(tour.begin() + indexToInsert, index);
	isTravelled[index] = 1;

	return objective;
}
