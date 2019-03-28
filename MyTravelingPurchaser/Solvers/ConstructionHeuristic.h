#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"

typedef std::pair<std::vector<unsigned int>, int> Solution;   /* <路由表，路由开销> */
typedef std::vector<Solution> Solutions;


class ConstructionHeuristic {
public:
	virtual Solutions * construct(
		unsigned int dimension,
		std::vector<int> demands,
		std::vector<std::vector<PriAva>> offer_lists,
		std::vector<std::vector<int>> distance_matrix
	) = 0;
};
