#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"
#include "./CAHSolver/CAHSolver.h"
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

		/* improvement */
		for (int i = 1; i < methods.size(); ++i) {
			if (methods[i] == "CAH") {
				CAHImprovement cah_improve_sol = CAHImprovement();
				mySolutions = cah_improve_sol.improve(
					instance->dimension,
					instance->demands,
					instance->offer_lists,
					instance->offer_sort_lists,
					instance->distance_matrix,
					mySolutions
				);
			}
		}

		//auto t = solutions->at(0);
		//MatlabVisualizer::visualize(instance, t->first);

		delete instance;
	}

	static void printChecker(TPPLIBInstance *instance) {
		using std::cout; 
		using std::endl;
		cout << instance->name << endl;
		cout << instance->dimension << endl;
		cout << instance->demands.size() << endl;
		cout << instance->distance_matrix.size() << endl;
		cout << instance->offer_lists.size() << endl;
	}

};
