#pragma once
#include "../Main/TPPCommon.h"
#include "../TPPLIBInstance/TPPLIBInstance.h"

class MyTPPChecker {
public:
	static bool myTppChecker(
		const Solution &sln, 
		unsigned int dimension,
		const std::vector<int>& demands,
		const std::vector<std::vector<PriQua>>& offer_lists,
		const std::vector<std::vector<int>>& distance_matrix
	);

	static bool simulateCheck(const Solution &sln, TPPLIBInstance *instance) {
		return myTppChecker(
			sln, instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->distance_matrix
		);
	}
};

