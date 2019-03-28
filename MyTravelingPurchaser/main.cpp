#include <ctime>
#include "./Solvers/SolverDispatcher.h"

const std::string FOLDER = "D:\\My.C++\\MyTravelingPurchaser\\InstanceOfTPP\\Clase1\\";


int main() {
	srand(time(NULL));

	std::string instanceName = "Singh33_2.33.50.1.5.tpp";
	std::string file = FOLDER + instanceName;
	auto methods = std::vector<std::string>();
	methods.push_back("CAH");

	SolverDispatcher::dispatch(file, methods);

	system("PAUSE");
	return 0;
}