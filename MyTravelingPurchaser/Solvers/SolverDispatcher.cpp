#include "SolverDispatcher.h"

void SolverDispatcher::dispatch(std::string filename, std::vector<std::string> methods) {

	auto instance = TPPLIBInstanceBuilder::buildFromFile(filename);

	/* Construction */
	Solutions initSolutions;
	if (methods[0] == "CAH") {
		CAHSolver cah_sol = CAHSolver();
		initSolutions = cah_sol.construct(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->distance_matrix
		);
	}
	else if (methods[0] == "GRB") {
		szx::Solver::Environment env;
		env.instPath = szx::Solver::Environment::DefaultInstanceDir() + INSTANCENAME;
		env.slnPath = "";
		env.msTimeout = TIMEOUTINSECOND * szx::Timer::MillisecondsPerSecond;
		GurobiSolver grb_sol = GurobiSolver();
		initSolutions = grb_sol.construct(
			instance->dimension,
			instance->demands,
			instance->offer_lists,
			instance->distance_matrix,
			env
		);
	}
	std::cout << "***Construction***" << std::endl;

	/* Improvement */
	Solutions iprvSolutions;
	if (methods[1] == "CAHImprove") {
		CAHImprovement cah_improve_sol = CAHImprovement();
		for (auto initSln : initSolutions) {
			iprvSolutions = cah_improve_sol.improve(
				instance->dimension,
				instance->demands,
				instance->offer_lists,
				instance->offer_sort_lists,
				instance->distance_matrix,
				initSln
			);
			Solutions &chkSolutions = iprvSolutions;
			MyTPPChecker tpp_checker = MyTPPChecker();
			for (auto sln : chkSolutions) {
				tpp_checker.myTppChecker(
					sln, instance->dimension,
					instance->demands,
					instance->offer_lists,
					instance->distance_matrix
				);
			}
		}
	}
	std::cout << "***Improvement***" << std::endl;

	/* Checker */
	
	

	/* Printer */
	//solutionsPrinter(mySolutions);

	/* Visualizer */
	//auto t = solutions->at(0);
	//MatlabVisualizer::visualize(instance, t->first);

	delete instance;
}

void SolverDispatcher::iteratedModel(Solution & sln, TPPLIBInstance *instance, szx::Solver::Environment &env) {
	
	/* Construction */
	Solutions initSolutions;
	GurobiSolver grb_sol = GurobiSolver();
	initSolutions = grb_sol.construct(
		instance->dimension,
		instance->demands,
		instance->offer_lists,
		instance->distance_matrix,
		env
	);
	sln = initSolutions[0];
}
