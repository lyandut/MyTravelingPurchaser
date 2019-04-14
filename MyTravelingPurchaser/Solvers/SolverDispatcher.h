#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"
#include "./CAHSolver/CAHSolver.h"
#include "./GurobiSolver/GurobiSolver.h"
#include "./Improver/CAHImprovement.h"
#include "../Checker/MyTPPChecker.h"
#include "../MyUtility/Solver.h"


class SolverDispatcher {
public:
	static void dispatch(std::string filename, std::vector<std::string> methods);
	static void iteratedModel(Solution &sln, TPPLIBInstance *instance, szx::Solver::Environment & env);

	static void instancePrinter(TPPLIBInstance *instance) {
		using std::cout; 
		using std::endl;
		cout << instance->name << endl;
		cout << instance->dimension << endl;
		cout << instance->demands.size() << endl;
		cout << instance->distance_matrix.size() << endl;
		cout << instance->offer_lists.size() << endl;
	}

	static void solutionsPrinter(Solutions solutions) {
		using std::cout;
		using std::endl;
		for (int i = 0; i < solutions.size(); i++) {
			auto slnOpt = solutions[i].opitimization;
			auto slnTour = solutions[i].tour;
			cout << "Opitimization: " << slnOpt << endl;
			cout << "Tour Size(include 0): " << slnTour.size() << endl;
			for (auto n : slnTour)
				cout << n << '\t';
			cout << endl;
		}
		/* Compare Purchase Plan */
		for (int i = 0; i < solutions.size() - 1; i++) {
			auto slnPlan = solutions[i].planTable;
			auto slnPlan2 = solutions[i + 1].planTable;
			cout << "Purchase Plan:" << endl;
			for (int n = 0; n < slnPlan.size(); n++) {
				for (int k = 0; k < slnPlan[n].size(); k++) {
					if (slnPlan[n][k] != slnPlan2[n][k])
						cout << n << '-' << k << ":\t" << slnPlan[n][k] << '\t' << slnPlan2[n][k] << endl;
				}
			}
		}
	}

};
