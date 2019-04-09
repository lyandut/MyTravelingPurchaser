#include <ctime>
#include "TPPConfig.h"
#include "../Solvers/SolverDispatcher.h"



void test() {
	//using namespace std;
	//using namespace szx;
	//int *a = new int[6]{ 1, 2, 3, 4, 5, 6 };
	//Arr2D<int> myArr(2, 3, a);
	////lkh::AdjMat adjMat = myArr;
	//cout << myArr.at(0, 0) << endl;
}

void run() {
	srand(time(NULL));

	std::string file = std::string(FOLDER) + CLASE + INSTANCENAME;
	auto methods = std::vector<std::string>();
	//methods.emplace_back("CAH");
	methods.emplace_back("GRB");
	methods.emplace_back("CAHImprove");

	SolverDispatcher::dispatch(file, methods);
}

int main() {
	//test();
	run();
	system("PAUSE");
	return 0;
}
