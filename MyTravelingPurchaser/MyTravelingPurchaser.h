#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class MyTravelingPurchaser
{
public:
	MyTravelingPurchaser();
	~MyTravelingPurchaser();
	
	int dimension;
	
	void readInstance(string fileName);
};

//bool Distance_Graph::load_from_file(std::string filename)
//{
//	std::string data;
//	std::fstream file;
//	std::size_t position;
//	int number_of_towns;
//	file.open(filename, std::ios::in);
//	if (file.is_open()) {
//		//NAME :
//		getline(file, data);
//		//TYPE :
//		getline(file, data);
//		position = data.find('TSP');
//		if (position > 0) {
//			//COMMENT :
//			getline(file, data);
//			position = data.find(':');
//			//std::cout << std::setw(25) << "Comment to a problem: " << data.substr(position + 1) << std::endl;
//			//DIMENSION :
//			getline(file, data);
//			position = data.find(':');
//			//std::cout << std::setw(25) << "Number of towns: " << data.substr(position + 1) << std::endl;
//			set_matrix_sizeof(std::stoi(data.substr(position + 1)));
//			//EDGE_WEIGHT_TYPE : EXPLICIT
//			getline(file, data);
//			position = data.find("EXPLICIT");
//			if (position > 0 && position < data.size()) {
//				//std::cout << std::setw(25) << "Edge Weight Type: " << data.substr(position) << std::endl;
//				if (position > 0) {
//					//EDGE_WEIGHT_FORMAT : LOWER_DIAG_ROW
//					getline(file, data);
//					position = data.find("LOWER_DIAG_ROW");
//					if (position > 0 && position < data.size()) {
//						//std::cout << std::setw(25) << "Matrix Type: " << data.substr(position) << std::endl;
//						getline(file, data);
//						matrix_type = "LOWER_DIAG_ROW";
//						getline(file, data);
//						do {
//							instert_line_lower_diagram(data);
//							getline(file, data);
//							position = data.find("EOF");
//							if (position == -1) {
//								position = data.find("DISPLAY_DATA_SECTION");
//							}
//						} while (position == -1);
//						lower_to_full();
//					}
//					position = data.find("FULL_MATRIX");
//					if (position > 0 && position < data.size()) {
//						//std::cout << std::setw(25) << "Matrix Type: " << data.substr(position) << std::endl;
//						matrix_type = "FULL_MATRIX";
//						getline(file, data); //DISPLAY_DATA_TYPE: TWOD_DISPLAY	
//						getline(file, data); //EDGE_WEIGHT_SECTION
//						getline(file, data);
//						int line = 0;
//						do {
//							instert_line_full_diagram(data, line);
//							line++;
//							getline(file, data);
//							position = data.find("DISPLAY_DATA_SECTION");
//						} while (position == -1);
//					}
//					position = data.find("UPPER_DIAG_ROW");
//					if (position > 0 && position < data.size()) {
//						//std::cout << std::setw(25) << "Matrix Type: " << data.substr(position) << std::endl;
//						matrix_type = "UPPER_DIAG_ROW";
//						getline(file, data); //DISPLAY_DATA_TYPE: NO_DISPLAY
//						getline(file, data); //EDGE_WEIGHT_SECTION
//						getline(file, data);
//						do {
//							instert_line_upper_diagram(data);
//							getline(file, data);
//							position = data.find("EOF");
//							if (position == -1) {
//								position = data.find("DISPLAY_DATA_SECTION");
//							}
//						} while (position == -1);
//						upper_to_full();
//					}
//				}
//				else {
//					std::cout << "Program doesn't work with non-explicit distances" << std::endl;
//				}
//			}
//		}
//		else {
//			std::cout << "Problem is not TSP" << std::endl;
//			return false;
//		}
//	}
//	else {
//		std::cout << "No such problem file";
//		return false;
//	}
//	return true;
//}