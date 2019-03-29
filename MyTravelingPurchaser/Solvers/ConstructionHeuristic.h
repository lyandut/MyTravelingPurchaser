#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"

typedef std::pair<std::vector<unsigned int>, int> Solution;   /* <路由表，路由开销> */
typedef std::vector<Solution> Solutions;


class ConstructionHeuristic {
public:
	virtual Solutions * construct(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriAva>> &offer_lists,
		const std::vector<std::vector<int>> &distance_matrix
	) = 0;
};
