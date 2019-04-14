#include <ctime>
#include "TPPConfig.h"
#include "../Solvers/SolverDispatcher.h"
#include "../MyUtility/Simulator.h"

void simulate() {
	using namespace szx;
	//Simulator::initDefaultEnvironment();

	Simulator sim;
	//sim.debug();
	//sim.benchmark(1);
	sim.parallelBenchmark(1);
	//sim.generateInstance();
}

void myRun() {
	std::string file = std::string(FOLDER) + CLASE + INSTANCENAME;
	auto methods = std::vector<std::string>();
	//methods.emplace_back("CAH");
	methods.emplace_back("GRB");
	methods.emplace_back("CAHImprove");

	SolverDispatcher::dispatch(file, methods);
}

void test() {
	using namespace std;
	cout << "hello TPP!" << endl;
}

int main() {
	//test();
	//myRun();
	simulate();
	system("PAUSE");
	return 0;
}
