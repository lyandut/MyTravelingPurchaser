#pragma once
#include "../ImprovementHeuristic.h"
#include "LKH3Lib/CachedTspSolver.h"
#include "../../MyUtility/LogSwitch.h"

using namespace szx;
using namespace lyan;

namespace lyan {
template <typename keyType, typename valType>
using Pair = std::pair<keyType, valType>;

template <typename keyType, typename valType>
std::vector<Pair<keyType, valType>> mapSortByValue(std::map<keyType, valType>& Map) {
	std::vector<Pair<keyType, valType>> vec(Map.begin(), Map.end());
	sort(vec.begin(), vec.end(), [&](Pair<keyType, valType> x, Pair<keyType, valType> y) {return x.second < y.second; });
	return vec;
}
}

class Perturbation {
public:
#pragma region Method
	void improve();
	bool marketDrop(int dropNode, std::map<int, lyan::Pair<int, int>> &costDelta);
	bool marketAdd(int addNode, std::map<int, lyan::Pair<int, int>> &costDelta);
	void tspHeuristic(std::vector<int> nodeIdMap, Solution &curSln);
	void updatePlanTable(std::vector<int> newTourSet, Solution &curSln);
	void makeMove(std::map<ID, Solution> &results);
#pragma endregion Method

#pragma region Field
	// Solution
	Solution sln;
	// Condition Info
	int dimension;
	int demandNum;
	DemmandList demands;
	OfferList offer_lists;
	OfferSortList offer_sort_lists;
	DisMatrix distance_matrix;
	// Tour Info
	std::vector<int> isTravelled;
	/* `totalQuantity[k]` is the total quantity of `product k` */
	std::vector<int> totalQuantity; 
	/* `totalKinds[i]` is the total number of products purchased from `node i` */
	std::vector<int> totalProKinds;
	lyan::OfferSortList ascList;   // used for dropNode
	lyan::OfferSortList descList;  // used for addNode
	// Tabu Info
	int tabuIter;
	std::vector<int> dropTabuTable;
	std::vector<int> addTabuTable;
	szx::Solver::Environment env;
	szx::Random rand;
	// TspSolver
	szx::CachedTspSolver tspSolver;
#pragma endregion Field

#pragma region Constructor
	Perturbation(
		unsigned int dimension,
		const DemmandList &demands,
		const OfferList& offer_lists,
		const OfferSortList &offer_sort_lists,
		const DisMatrix& distance_matrix,
		const Solution &sln,
		const szx::Solver::Environment &env) 
		: dimension(dimension), demandNum(demands.size()), demands(demands), offer_lists(offer_lists),
		offer_sort_lists(offer_sort_lists), distance_matrix(distance_matrix), sln(sln), env(env),
		tspSolver(dimension, std::string("TspCache/") + env.friendlyInstName() + ".csv")
	{
		isTravelled.resize(dimension, 0);
		for (int tourNode : sln.tour) {
			isTravelled[tourNode] = 1;
		}
		ascList.resize(demandNum);
		descList.resize(demandNum);
		totalQuantity.resize(demandNum, 0);
		totalProKinds.resize(dimension, 0);
		for (int k = 0; k < offer_sort_lists.size(); k++) {
			for (auto offerInfo : offer_sort_lists[k]) {
				if (isTravelled[offerInfo.nodeNo]) {
					ascList[k].emplace_back(offerInfo);
					descList[k].emplace(descList[k].begin(), offerInfo);
					totalQuantity[k] += offerInfo.quantity;
					totalProKinds[offerInfo.nodeNo] += 1;
				}
			}
		}
		tabuIter = 0;
		dropTabuTable = std::vector<int>(dimension, -1);
		addTabuTable = std::vector<int>(dimension, -1);
		rand = szx::Random(env.randSeed);
	}
#pragma endregion Constructor

};

