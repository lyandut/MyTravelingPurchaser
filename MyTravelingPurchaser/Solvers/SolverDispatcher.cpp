#include "SolverDispatcher.h"

szx::Solver::Environment SolverDispatcher::env(
	szx::Solver::Environment::DefaultInstanceDir() + INSTANCESET + "." + INSTANCEID + ".tpp",
	szx::Solver::Environment::DefaultSolutionDir() + INSTANCESET + "." + INSTANCEID + ".json",
	RANDSEED,
	TIMEOUTINSECOND
);

void SolverDispatcher::dispatch(std::string filename, std::vector<std::string> methods) {
	
	auto instance = TPPLIBInstanceBuilder::buildFromFile(filename);

	/* Construction */
	Solution initSln;
	if (methods[0] == "CAH") {
		CAHSolver cah_sol = CAHSolver();
		Solutions initSolutions = cah_sol.construct(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->distance_matrix,
			env
		);
		initSln = initSolutions[0];
	}
	else if (methods[0] == "GRB") {
		GurobiSolver grb_sol = GurobiSolver();
		Solutions initSolutions = grb_sol.construct(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->distance_matrix,
			env
		);
		initSln = initSolutions[0];
	}
	std::cout << "***Construction***" << std::endl;

	/* Improvement */
	Solution iprvSln;
	if (methods[1] == "CAHImprove") {
		CAHImprovement cah_improve_sol = CAHImprovement();
		Solutions iprvSolutions = cah_improve_sol.improve(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->offer_sort_lists,
			instance->distance_matrix,
			initSln,
			env
		);
		iprvSln = iprvSolutions[0];
	}
	else if(methods[1] == "Perturbation"){
		Perturbation per_improve_sol(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->offer_sort_lists,
			instance->distance_matrix,
			initSln,
			env
		);
		per_improve_sol.improve();
		iprvSln = per_improve_sol.sln;
	}
	std::cout << "***Improvement***" << std::endl;

	/* Checker */
	Solution &chkSln = iprvSln;
	MyTPPChecker tpp_checker = MyTPPChecker();
	tpp_checker.myTppChecker(
		chkSln, instance->dimension,
		instance->demands,
		instance->offer_lists,
		instance->distance_matrix
	);


	/* Printer */
	//solutionsPrinter(mySolutions);

	/* Visualizer */
	//auto t = solutions->at(0);
	//MatlabVisualizer::visualize(instance, t->first);

	delete instance;
}

void SolverDispatcher::iteratedModel(Solution & sln, TPPLIBInstance *instance, szx::Solver::Environment &env) {
	
	/* Construction */
	Solution initSln;
	GurobiSolver grb_sol = GurobiSolver();
	Solutions initSolutions = grb_sol.construct(
		instance->dimension,
		instance->demands,
		instance->offer_lists,
		instance->distance_matrix,
		env
	);
	initSln = initSolutions[0];
	
	//CAHImprovement cah_improve_sol = CAHImprovement();
	//Solutions iprvSolutions = cah_improve_sol.improve(
	//	instance->dimension,
	//	instance->demands,
	//	instance->offer_lists,
	//	instance->offer_sort_lists,
	//	instance->distance_matrix,
	//	initSolutions[0],
	//	env
	//);

	Perturbation per_improve_sol(
		instance->dimension,
		instance->demands,
		instance->offer_lists,
		instance->offer_sort_lists,
		instance->distance_matrix,
		initSln,
		env
	);
	per_improve_sol.improve();
	sln = per_improve_sol.sln;
}
