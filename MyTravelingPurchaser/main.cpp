#include <iostream>
#include "./Solvers/SolverDispatcher.h"



int main() {

	std::string file = "D:\\My.C++\\MyTravelingPurchaser\\InstanceOfTPP\\Clase5\\CapEuclideo.50.50.1.1.tpp";
	auto methods = std::vector<std::string>();
	methods.push_back("CAH");

	SolverDispatcher::dispatch(file, methods);

	system("PAUSE");
	return 0;
}