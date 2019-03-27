#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>
#include <cctype>
#include <cmath>

typedef std::tuple<int, int, int> IdPriAva;

typedef struct {
	int offerNum;
	std::vector<IdPriAva> offerTuple;
}NodeOfferList;

class TPPLIBInstance
{
public:
	std::string name;
	std::string comment;
	std::string instance_type;
	std::string edge_weight_type;
	std::string edge_weight_format;
	std::string edge_data_format;
	std::string node_coord_type;
	std::string display_data_type;

	unsigned int dimension;
	std::vector<int> demands;
	std::vector<NodeOfferList> offer_lists;
	std::vector<std::pair<int, int>> edge_list;
	std::vector<std::vector<double>> coordinates;
	std::vector<std::vector<double>> display_coordinates;
	std::vector<std::vector<int>> distance_matrix;

	TPPLIBInstance(){
		name = "not specified";
		comment = "not specified";
		instance_type = "TPP";
		edge_weight_type = "not specified";
		edge_weight_format = "not specified";
		edge_data_format = "COMPLETE";
		node_coord_type = "NO_COORDS";
		display_data_type = "NO_DISPLAY";

		dimension = 0;
		demands = std::vector<int>();
		offer_lists = std::vector<NodeOfferList>();
		edge_list = std::vector<std::pair<int, int>>();
		coordinates = std::vector<std::vector<double>>();
		display_coordinates = std::vector<std::vector<double>>();
		distance_matrix = std::vector<std::vector<int>>();
	}
	
	~TPPLIBInstance() {}
};


class TPPLIBInstanceBuilder {
public:
	static TPPLIBInstance * buildFromFile(std::string file_name);
};

