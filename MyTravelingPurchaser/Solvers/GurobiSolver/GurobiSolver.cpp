/* 
 * Construct Solution by Gurobi 
 */
#include "GurobiSolver.h"
#include "../../MyUtility/MpSolver/MpSolver.h"
#include "../../TPPLIBInstance/TPPLIBInstance.h"

const double timeoutInSecond = 180.0;

using namespace szx;

using Dvar = MpSolver::DecisionVar;
using Expr = MpSolver::LinearExpr;

Solutions GurobiSolver::construct(
	unsigned int dimension,
	const std::vector<int> &demands,
	const std::vector<std::vector<PriQua>> &offer_lists,
	const std::vector<std::vector<int>> &distance_matrix
) {
	int demandNum = demands.size();

	MpSolver::Configuration mpCfg(MpSolver::InternalSolver::GurobiMip, timeoutInSecond, true, true);
	MpSolver mp(mpCfg);

	/* Decision Variables
	 * 1. Bool: x[i, j] is true if the edge from node i to node j is visited.
	 * 2. Bool: y[i] is true if node i is selected.
	 * 3. Interger: planTable[i, k] is the quantity of k purchased from node i.
	 */
	Arr2D<Dvar> planTable(dimension, demandNum);
	Arr2D<Dvar> x(dimension, dimension);
	Arr<Dvar> y(dimension);
	for (int i = 0; i < dimension; i++) {
		for (int k = 0; k < demandNum; k++) {
			int availability = offer_lists[i][k].second;
			planTable[i][k] = mp.addVar(MpSolver::VariableType::Integer, 0, availability);
		}
		for (int j = 0; j < dimension; j++) {
			if (i == j) continue;
			x[i][j] = mp.addVar(MpSolver::VariableType::Bool, 0, 1);
		}
		y[i] = mp.addVar(MpSolver::VariableType::Bool, 0, 1);
	}

	/* Constraint
	 * 1. Sum(planTable[i][k]) == demands[k]
	 * 2. node 0 must be involed in tour
	 * 3. 0 <= inDegree == outDegree <= 1
	 * 4. 0 <= planTable[i][k] <= offer_lists[i][k].second * inDegree
	 */
	for (int k = 0; k < demandNum; k++) {
		Expr sumDemand = 0;
		for (int i = 0; i < dimension; i++) {
			sumDemand += planTable[i][k];
		}
		mp.addConstraint(sumDemand == demands[k]);
	}
	for (int i = 0; i < dimension; i++) {
		Expr inDegree = 0, outDegree = 0;
		for (int j = 0; j < dimension; j++) {
			if (i == j) continue;
			inDegree += x[j][i];
			outDegree += x[i][j];
		}
		mp.addConstraint(y[i] == inDegree);
		mp.addConstraint(y[i] == outDegree);
		if (i == 0)
			mp.addConstraint(y[i] == 1);
		else {
			mp.addConstraint(y[i] <= 1);
			mp.addConstraint(y[i] >= 0);
		}
		for (int k = 0; k < demandNum; k++) {
			mp.addConstraint(planTable[i][k] >= 0);
			mp.addConstraint(planTable[i][k] <= offer_lists[i][k].second * y[i]);
		}
	}

	/* 
	 * Objective
	 */
	Expr obj = 0;
	Expr routeCost = 0, purchaseCost = 0;
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			if (i == j) continue;
			routeCost += x[i][j] * distance_matrix[i][j];
		}
		for (int k = 0; k < demandNum; k++) {
			purchaseCost += planTable[i][k] * offer_lists[i][k].first;
		}
	}
	obj = routeCost + purchaseCost;
	mp.addObjective(obj, MpSolver::OptimaOrientation::Minimize, 0, 0, 0, timeoutInSecond);

	/*
	 * CallBack
	 */
	auto subTourHandler = [&](MpSolver::MpEvent &e) {
		enum EliminationPolicy { // OPTIMIZE[szx][0]: first sub-tour, best sub-tour or all sub-tours?
			NoSubTour = 0x0,
			AllSubTours = 0x1,
			FirstSubTour = 0x2,
			BestSubTour = 0x4
		};
		EliminationPolicy policy = EliminationPolicy::BestSubTour;

		List<ID> bestTour; // tour with least nodes/hops.
		List<ID> tour;
		tour.reserve(dimension);
		Arr<bool> visited(dimension);
		visited.reset(Arr<bool>::ResetOption::AllBits0);

		// check if there is a route start from each node.
		for (ID s = 0; s < dimension; ++s) {
			if (visited[s]) { continue; }
			ID prev = s;		// s:tour起点；prev:指针,向前移动
			do {
				for (ID n = 0; n < dimension; ++n) {
					if (prev == n) { continue; }
					if (!e.isTrue(x.at(prev, n))) { continue; }
					if (s != 0) { tour.push_back(n); } // the sub-tour containing depot should not be eliminated.
					prev = n;
					visited[n] = true;
					break;
				}
			} while (prev != s);
			if (tour.empty()) { continue; }
			
			if (policy & (EliminationPolicy::AllSubTours | EliminationPolicy::FirstSubTour)) {
				Expr edges;
				for (auto n = tour.begin(); n != tour.end(); prev = *n, ++n) {
					edges += x.at(prev, *n);
				}
				e.addLazy(edges <= static_cast<double>(tour.size() - 1));
				if (policy & EliminationPolicy::FirstSubTour) { break; }
			}

			if (bestTour.empty() || (tour.size() < bestTour.size())) { swap(bestTour, tour); }
		}

		if ((policy & EliminationPolicy::BestSubTour) && !bestTour.empty()) {
			Expr edges;
			ID prev = bestTour.back();
			for (auto n = bestTour.begin(); n != bestTour.end(); prev = *n, ++n) {
				edges += x.at(prev, *n);
			}
			e.addLazy(edges <= static_cast<double>(bestTour.size() - 1));
		}
	};
	mp.setMipSlnEvent(subTourHandler);

	Solution sln;
	List<ID> slnTour;
	List<List<ID>> slnPlanTable(dimension, List<ID>(demandNum, 0));
	if (mp.optimize()) {
		ID prev = 0;
		slnTour.push_back(0);
		do {
			for (ID n = 0; n < dimension; ++n) {
				if (prev == n) { continue; }
				if (!mp.isTrue(x.at(prev, n))) { continue; }
				slnTour.push_back(n);
				for (int k = 0; k < demandNum; ++k) {
					slnPlanTable[n][k] = mp.getValue(planTable[n][k]);
				}
				prev = n;
				break;
			}
		} while (prev != 0);

		sln.opitimization = obj.getValue();
		sln.tour = slnTour;
		sln.planTable = slnPlanTable;
	}

	Solutions result;
	result.emplace_back(sln);
	return result;
}