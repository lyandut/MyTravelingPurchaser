#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"
#include "./CAHSolver/CAHSolver.h"
#include "./GurobiSolver/GurobiSolver.h"
#include "./Improvements/CAHImprovement.h"
#include "../Checker/MyTPPChecker.h"


class SolverDispatcher {
public:
	static void dispatch(std::string filename, std::vector<std::string> methods) {
		
		auto instance = TPPLIBInstanceBuilder::buildFromFile(filename);
		Solutions mySolutions;

		/* Construction */
		if (methods[0] == "CAH") {
			CAHSolver cah_sol = CAHSolver();
			mySolutions = cah_sol.construct(
				instance->dimension,
				instance->demands,
				instance->offer_lists,
				instance->distance_matrix
			);
		}
		else if (methods[0] == "GRB") {
			GurobiSolver grb_sol = GurobiSolver();
			mySolutions = grb_sol.construct(
				instance->dimension,
				instance->demands,
				instance->offer_lists,
				instance->distance_matrix
			);
		}
		std::cout << "***Construction***" << std::endl;
		solutionsPrinter(mySolutions);

		/* Improvement */
		//for (int i = 1; i < methods.size(); ++i) {
		//	if (methods[i] == "CAHImprove") {
		//		CAHImprovement cah_improve_sol = CAHImprovement();
		//		mySolutions = cah_improve_sol.improve(
		//			instance->dimension,
		//			instance->demands,
		//			instance->offer_lists,
		//			instance->offer_sort_lists,
		//			instance->distance_matrix,
		//			mySolutions
		//		);
		//	}
		//	std::cout << "***CAH-Improvement***" <<  std::endl;
		//	solutionsPrinter(mySolutions);
		//}

		/* Checker */
		MyTPPChecker tpp_checker = MyTPPChecker();
		for (auto sln : mySolutions) {
			tpp_checker.myTppChecker(
				sln, instance->dimension,
				instance->demands,
				instance->offer_lists,
				instance->distance_matrix
			);
		}
		
		/* Visualizer */
		//auto t = solutions->at(0);
		//MatlabVisualizer::visualize(instance, t->first);

 		delete instance;
	}

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
