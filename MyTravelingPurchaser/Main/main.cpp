#include <ctime>
#include "TPPConfig.h"
#include "../Solvers/SolverDispatcher.h"



void test() {
	using namespace std;
	cout << "hello TPP!" << endl;
}

void run() {
	srand(time(NULL));

	std::string file = std::string(FOLDER) + CLASE + INSTANCENAME;
	auto methods = std::vector<std::string>();
	methods.emplace_back("CAH");
	//methods.emplace_back("GRB");
	methods.emplace_back("CAHImprove");

	SolverDispatcher::dispatch(file, methods);
}

int main() {
	//test();
	run();
	system("PAUSE");
	return 0;
}
