#pragma once

#include "./ConstructionHeuristic.h"

class CAHSolver : public ConstructionHeuristic {
public:
	Solutions construct(
		unsigned int dimension, 
		const std::vector<int>& demands, 
		const std::vector<std::vector<PriAva>>& offer_lists, 
		const std::vector<std::vector<int>>& distance_matrix
	) override;
	double insertTourSaving(
		int index,
		double objective,
		std::vector<int> &tour,
		std::vector<int> &isTravelled,
		std::vector<int> &isSatisfy,
		std::vector<std::vector<int>> &planTable,
		const std::vector<std::vector<int>> &distance_matrix,
		const std::vector<std::vector<PriAva>> &offer_lists
	);
};

//Solutions * CAHSolver::construct(std::vector<std::vector<int>> distance_matrix, unsigned int n)
//{
//	int *travelled = new int[n];
//	for (int i = 1; i < n; ++i)
//		travelled[i] = 0;
//	travelled[0] = 1;
//
//	auto tour = std::vector<unsigned int>(); // 路由节点顺序表
//	tour.emplace_back(0);
//
//	int total = 1;  // 已访问节点个数
//	int objective = 0;  // 最优解
//	int node = 0;    // 当前到达的节点
//	int min_index = -1;
//	while (total < n) {
//		int min = -1;     // node 到 min_index 的距离为 min
//		min_index = -1;   //
//		for (int i = 0; i < n; ++i) {
//			if (i == node || travelled[i] == 1)continue;
//			if (min == -1) {
//				min = distance_matrix[node][i];
//				min_index = i;
//			}
//			else if (min > distance_matrix[node][i]) {
//				min = distance_matrix[node][i];
//				min_index = i;
//			}
//		}
//		objective += distance_matrix[node][min_index];
//		tour.emplace_back(min_index);
//		node = min_index;   // 前进一步到达 min_index 节点
//		total++;
//		travelled[node] = 1;
//	}
//
//	objective += distance_matrix[min_index][0];
//
//	Solution sol = Solution();
//
//	sol.first = tour;
//	sol.second = objective;
//
//	auto *result = new Solutions();
//	result->emplace_back(sol);
//	return result;
//}