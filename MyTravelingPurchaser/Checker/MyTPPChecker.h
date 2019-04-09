#pragma once
#include "../Main/TPPCommon.h"

class MyTPPChecker {
public:
	bool myTppChecker(
		Solution &sln, 
		unsigned int dimension,
		const std::vector<int>& demands,
		const std::vector<std::vector<PriQua>>& offer_lists,
		const std::vector<std::vector<int>>& distance_matrix
	);
};

