#pragma once
#include <iostream>
#include <io.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <cmath>

class TPPLIBInstance
{
public:
	std::string name;
	std::string comment;
	std::string instance_type;
	std::string graph_type;
	std::string edge_type;
	std::string edge_weight_type;
	std::string edge_weight_format;
	std::string edge_data_format;
	std::string node_type;
	std::string node_coord_type;
	std::string display_data_type;

	unsigned int dimension;
	int capacity;
	std::vector<int> *alternate_depots;
	std::vector<std::pair<int, int>> *fixed_edges;
	std::vector<std::pair<int, int>> *edge_list;
	std::vector<std::pair<int, std::vector<int> *> *> *adj_list;
	int *demands;
	int *node_weights;

	double coord1_offset;
	double coord1_scale;

	double coord2_offset;
	double coord2_scale;

	double coord3_offset;
	double coord3_scale;

	double **coordinates;
	double **display_coordinates;
	int **distance_matrix;

	TPPLIBInstance(){
		name = "not specified";
		comment = "not specified";
		instance_type = "TPP";
		graph_type = "COMPLETE_GRAPH";
		edge_type = "UNDIRECTED";
		edge_weight_type = "not specified";
		edge_weight_format = "not specified";
		edge_data_format = "COMPLETE";
		node_type = "UNWEIGHTED_NODES";
		node_coord_type = "NO_COORDS";
		dimension = 0;
		capacity = 0;

		coord1_offset = 0;
		coord2_offset = 0;
		coord3_offset = 0;

		coord1_scale = 1;
		coord2_scale = 1;
		coord3_scale = 1;

		display_data_type = "NO_DISPLAY";

		coordinates = nullptr;
		alternate_depots = nullptr;
		demands = nullptr;
		fixed_edges = nullptr;
		display_coordinates = nullptr;
		node_weights = nullptr;
		edge_list = nullptr;
		adj_list = nullptr;
		distance_matrix = nullptr;
	}
	
	~TPPLIBInstance() {
		if (coordinates != nullptr) {
			for (int i = 0; i < dimension; ++i) {
				delete[] coordinates[i];
			}
			delete[] coordinates;
		}
		delete alternate_depots;
		delete[] demands;
		delete fixed_edges;
		if (display_coordinates != nullptr) {
			for (int i = 0; i < dimension; ++i) {
				delete[] display_coordinates[i];
			}
			delete[] display_coordinates;
		}
		delete[] node_weights;
		delete edge_list;

		if (adj_list != nullptr) {
			for (auto item : *adj_list) {
				delete item->second;
				delete item;
			}
			delete adj_list;
		}

		if (distance_matrix != nullptr) {
			for (int i = 0; i < dimension; ++i) {
				delete[] distance_matrix[i];
			}
			delete[] distance_matrix;
		}
	}
};


class TPPLIBInstanceBuilder {
public:
	static TPPLIBInstance *buildFromFile(const char *file_name);
};
