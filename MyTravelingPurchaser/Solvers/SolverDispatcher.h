#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"
#include "./CAHSolver/CAHSolver.h"
#include "./GurobiSolver/GurobiSolver.h"
#include "./Improvements/CAHImprovement.h"


class SolverDispatcher {
public:
	static void dispatch(std::string filename, std::vector<std::string> methods) {
		
		auto instance = TPPLIBInstanceBuilder::buildFromFile(filename);
		Solutions mySolutions;

		/* construction */
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
		std::cout << "***CAH-Construction***" << std::endl;
		solutionsPrinter(mySolutions);

		/* improvement */
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
			cout << solutions[i].opitimization << endl;
			cout << solutions[i].tour.size() << endl;
		}
	}

};
