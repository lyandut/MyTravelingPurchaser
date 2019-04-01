#pragma once
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <functional>
#include <cctype>
#include <cmath>
#include "../common.h"



class TPPLIBInstance
{
public:
	std::string name;
	std::string comment;
	std::string instance_type;
	std::string edge_weight_type;
	std::string edge_weight_format; /* 默认完全图: COMPLETE */
	std::string edge_data_format;
	std::string node_coord_type;
	std::string display_data_type;

	/* 参与算法求解 */
	unsigned int dimension;
	std::vector<int> demands;
	std::vector<std::vector<PriQua>> offer_lists;
	std::vector<std::vector<NodePriQua>> offer_sort_lists;
	std::vector<std::vector<int>> distance_matrix;

	/* 非完全图中描述边的连接关系 */
	std::vector<std::pair<int, int>> edge_list;
	/* 点坐标，根据距离函数求路由开销存入 distance_matrix */
	std::vector<std::vector<double>> coordinates;
	std::vector<std::vector<double>> display_coordinates;
	

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
		offer_lists = std::vector<std::vector<PriQua>>();
		offer_sort_lists = std::vector<std::vector<NodePriQua>>();
		distance_matrix = std::vector<std::vector<int>>();
		edge_list = std::vector<std::pair<int, int>>();
		coordinates = std::vector<std::vector<double>>();
		display_coordinates = std::vector<std::vector<double>>();
	}
	
	~TPPLIBInstance() {}
};


class TPPLIBInstanceBuilder {
public:
	static TPPLIBInstance * buildFromFile(std::string file_name);
};

