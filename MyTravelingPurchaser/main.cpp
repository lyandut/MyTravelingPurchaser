#include <ctime>
#include "./Solvers/SolverDispatcher.h"

const std::string FOLDER = "D:\\My.C++\\MyTravelingPurchaser\\InstanceOfTPP";


int main() {
	srand(time(NULL));

	//std::string instanceName = "\\Clase1\\Singh33_2.33.50.1.5.tpp";
	//std::string instanceName = "\\Clase2\\PearnChien.10.50.1.tpp";
	std::string instanceName = "\\Clase3\\EEuclideo.50.50.1.tpp";
	//std::string instanceName = "\\Clase5\\CapEuclideo.50.50.1.1.tpp";
	
	std::string file = FOLDER + instanceName;
	auto methods = std::vector<std::string>();
	methods.push_back("CAH");
	methods.push_back("CAHImprove");

	SolverDispatcher::dispatch(file, methods);

	system("PAUSE");
	return 0;
}