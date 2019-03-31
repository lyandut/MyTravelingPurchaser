#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"

typedef struct {
	double opitimization;   // 最优解 
	std::vector<int> tour;  // 路由表
	std::vector<std::vector<int>> planTable; // 采购计划
}Solution;
typedef std::vector<Solution> Solutions;


class ConstructionHeuristic {
public:
	virtual Solutions construct(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriAva>> &offer_lists,
		const std::vector<std::vector<int>> &distance_matrix
	) = 0;
};
