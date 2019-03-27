#pragma once

#include <vector>
#include <string>
#include <map>
#include "./CAHSolver.h"
#include "../TPPLIBInstance/TPPLIBInstance.h"


class SolverDispatcher {
public:
	static void dispatch(std::string filename, std::vector<std::string> methods) {
		auto instance = TPPLIBInstanceBuilder::buildFromFile(filename);
		printChecker(instance);


		//auto construction_method = methods.at(0);
		//Solutions *solutions = nullptr;

		//if (construction_method == "CAH") {
		//	CAHSolver cah_sol = CAHSolver();
		//	solutions = cah_sol.construct(instance->distance_matrix, instance->dimension);
		//}

		//unsigned long method_size = methods.size();

		//for (unsigned int i = 1; i < method_size; ++i) {
		//	if (methods->at(i) == "2opt") {
		//		auto two_opt_solver = new TwoOptSolver;
		//		solutions = two_opt_solver->improve(instance->distance_matrix, instance->dimension, solutions);
		//		delete two_opt_solver;
		//	}
		//}

		//auto t = solutions->at(0);
		//MatlabVisualizer::visualize(instance, t->first);

		delete instance;
	}

	static void printChecker(TPPLIBInstance *instance) {
		using std::cout; 
		using std::endl;
		cout << instance->name << endl;
		cout << instance->comment << endl;
		cout << instance->dimension << endl;
		cout << instance->demands.size() << endl;
		cout << instance->distance_matrix.size() << endl;
		cout << instance->offer_lists.size() << endl;
	}

};
