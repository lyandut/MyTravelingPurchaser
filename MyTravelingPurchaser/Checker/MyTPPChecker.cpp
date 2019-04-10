#include "MyTPPChecker.h"
#include "../MyUtility/LogSwitch.h"

using namespace szx;

bool MyTPPChecker::myTppChecker(Solution & sln, unsigned int dimension, const std::vector<int>& demands, const std::vector<std::vector<PriQua>>& offer_lists, const std::vector<std::vector<int>>& distance_matrix)
{
	int routeCost = 0;
	int purchaseCost = 0;

	Log(LogSwitch::Szx::Checker) << "***Check Begin***" << std::endl;

#pragma region TourCheck
	Log(LogSwitch::Szx::Checker) << "===Tour Check===" << std::endl;
	int tourSize = sln.tour.size() - 1;    // ÆðÖ¹µã = 0
	if (tourSize > dimension && tourSize < 2) {
		Log(LogSwitch::Szx::Checker) << "Error: Tour Size=" << tourSize << "\tDimension=" << dimension << std::endl;
		return false;
	}
	auto iterBegin = sln.tour.begin(), iterEnd = sln.tour.end();
	if (*iterBegin != 0 || *(iterEnd - 1) != 0) {
		Log(LogSwitch::Szx::Checker) << "Error: Begin & End should be depot 0." << std::endl;
		return false;
	}
	for (int i = 0; i < tourSize; i++) {
		if (count(iterBegin, iterEnd - 1, sln.tour[i]) != 1) {
			Log(LogSwitch::Szx::Checker) << "Error: Node duplication=" << sln.tour[i] << std::endl;
			return false;
		}
		Log(LogSwitch::Szx::Checker) << sln.tour[i] << '\t';
		routeCost += distance_matrix[sln.tour[i]][sln.tour[i + 1]];
	}
	Log(LogSwitch::Szx::Checker) << sln.tour[tourSize] << std::endl;
#pragma endregion TourCheck

#pragma region PurchaseCheck
	Log(LogSwitch::Szx::Checker) << "===Purchase Plan Check===" << std::endl;
	for (int k = 0; k < demands.size(); k++) {
		int sumPurCost = 0;
		for (int i = 0; i < sln.planTable.size(); i++) {
			int iPrice = offer_lists[i][k].first;
			int iQuantity = offer_lists[i][k].second;
			if (sln.planTable[i][k] > iQuantity) {
				Log(LogSwitch::Szx::Checker) << "Error: Purchases is more than Availability. ";
				Log(LogSwitch::Szx::Checker) << "Node: " << i << " Product: " << k << std::endl;
				Log(LogSwitch::Szx::Checker) << "Purchases=" << sln.planTable[i][k] << " Availability=" << iQuantity << std::endl;
				return false;
			}
			sumPurCost += sln.planTable[i][k];
			purchaseCost += sln.planTable[i][k] * iPrice;
		}
		if (sumPurCost < demands[k]) {
			Log(LogSwitch::Szx::Checker) << "Error: Demand " << k << " is not satisfied." << std::endl;
			Log(LogSwitch::Szx::Checker) << "sumPurCost=" << sumPurCost << " Demand="<< demands[k] << std::endl;
			return false;
		}
	}
#pragma endregion PurchaseCheck

#pragma region OptCheck
	Log(LogSwitch::Szx::Checker) << "===Opitimization Check===" << std::endl;
	if ((purchaseCost + routeCost) != sln.opitimization) {
		Log(LogSwitch::Szx::Checker) << "Calculation Error!" << std::endl;
		return false;
	}
	Log(LogSwitch::Szx::Checker) << "Opitimization=" << sln.opitimization << std::endl;
#pragma endregion OptCheck

	Log(LogSwitch::Szx::Checker) << "***Check Finish***" << std::endl;
	return true;
}
