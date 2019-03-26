#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TPPLIBInstance.h"
using namespace std;


// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(),
		std::find_if(
			s.begin(), s.end(), 
			std::not1(std::ptr_fun<int, int>(std::isspace))
		)
	);
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
		s.end());
}
int main() {

	std::string line = "   TYPE      : TPP";
	std::string keyword, value;

	cout << "line: " << line << endl;

	std::istringstream str_stream(line);
	str_stream >> keyword;
	cout << "keyword: " << keyword << endl;


	system("PAUSE");
	return 0;
}