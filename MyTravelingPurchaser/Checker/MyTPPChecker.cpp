#include "MyTPPChecker.h"
#include "../MyUtility/LogSwitch.h"

using namespace szx;

bool MyTPPChecker::myTppChecker(Solution & sln, unsigned int dimension, const std::vector<int>& demands, const std::vector<std::vector<PriQua>>& offer_lists, const std::vector<std::vector<int>>& distance_matrix)
{
	int routeCost = 0;
	double purchaseCost = 0.0;

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
		routeCost += distance_matrix[i][i + 1];
	}
	Log(LogSwitch::Szx::Checker) << std::endl;
#pragma endregion TourCheck

	Log(LogSwitch::Szx::Checker) << "===Tour Size Check===" << std::endl;


	
	return false;
}
