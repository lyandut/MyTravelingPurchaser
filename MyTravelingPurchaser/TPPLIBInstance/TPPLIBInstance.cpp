#include "TPPLIBInstance.h"

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}


static void read_string_inside(std::istringstream &str_stream, std::string &str) {
	std::getline(str_stream, str);
	trim(str);
	if (str[0] == ':') {
		str.erase(0, 1);
		ltrim(str);
	}
}


static std::string read_specification_part(std::ifstream &file, TPPLIBInstance *instance) {

	std::string line;
	std::string keyword;

	while (!file.eof()) {
		std::getline(file, line);
		std::istringstream str_stream(line);
		str_stream >> keyword;

		if (keyword.compare(keyword.length() - 1, 1, ":") == 0) {
			keyword.pop_back();
		}
		else {
			// is either does not exist or waiting
			str_stream >> std::ws; // eats up white spaces
			int c = str_stream.peek();
			if ((char)c == ':') {
				str_stream.get();
			}
		}

		if (keyword == "NAME") {
			read_string_inside(str_stream, instance->name);
		}
		else if (keyword == "COMMENT") {
			read_string_inside(str_stream, instance->comment);
		}
		else if (keyword == "TYPE") {
			read_string_inside(str_stream, instance->instance_type);
		}
		else if (keyword == "DIMENSION") {
			str_stream >> instance->dimension;
		}
		else if (keyword == "EDGE_WEIGHT_TYPE") {
			read_string_inside(str_stream, instance->edge_weight_type);
		}
		else if (keyword == "EDGE_WEIGHT_FORMAT") {
			read_string_inside(str_stream, instance->edge_weight_format);
		}
		else if (keyword == "EDGE_DATA_FORMAT") {
			read_string_inside(str_stream, instance->edge_data_format);
		}
		else if (keyword == "NODE_COORD_TYPE") {
			read_string_inside(str_stream, instance->node_coord_type);
		}
		else if (keyword == "DISPLAY_DATA_TYPE") {
			read_string_inside(str_stream, instance->display_data_type);
		}
		else {
			break;
		}
	}

	return keyword;
}

// On error buildFromFile method will return nullptr
// and set the error_no
TPPLIBInstance * TPPLIBInstanceBuilder::buildFromFile(std::string filename) {

	std::ifstream file(filename);
	if (!file.is_open()) {
		return nullptr;
	}

	auto instance = new TPPLIBInstance();

	std::string keyword = read_specification_part(file, instance);
	std::string line;
	while (!file.eof()) {
		/* 二维/三维点坐标 */
		if (keyword == "NODE_COORD_SECTION") {
			int n = instance->dimension;
			int xyz = 0;
			instance->coordinates = std::vector<std::vector<double>>(n);

			if (instance->node_coord_type == "TWOD_COORDS" || instance->node_coord_type == "NO_COORDS") {
				for (int i = 0; i < n; i++) {
					instance->coordinates[i] = std::vector<double>(2);
				}
				xyz = 2;
			}
			else if (instance->node_coord_type == "THREED_COORDS") {
				for (int i = 0; i < n; i++) {
					instance->coordinates[i] = std::vector<double>(3);
				}
				xyz = 3;
			}
			else {
				//NODE_COORD_SECTION started however node_coord_type is NO_COORDS
				return nullptr;
				file.close();
			}

			for (int i = 0; i < n; i++) {
				int node_index;
				std::getline(file, line);
				std::stringstream str_stream_2(line);
				str_stream_2 >> node_index;
				for (int j = 0; j < xyz; ++j) {
					str_stream_2 >> instance->coordinates[node_index - 1][j];
				}
			}
		}
		/* 需求分布 */
		else if (keyword == "DEMAND_SECTION") {
			int demand_num;
			file >> demand_num;
			instance->demands = std::vector<int>(demand_num);
			for (int i = 0; i < demand_num; ++i) {
				int demand_index;
				file >> demand_index;
				file >> instance->demands[demand_index - 1];
			}
		}
		else if (keyword == "OFFER_SECTION") {
			int demand_num = instance->demands.size();
			int n = instance->dimension;
			instance->offer_lists = std::vector<std::vector<PriQua>>(n, std::vector<PriQua>(demand_num, std::make_pair(-1, 0)));
			instance->offer_sort_lists = std::vector<std::vector<NodePriQua>>(demand_num);
			for (int i = 0; i < n; i++) {
				int node_index, offer_num;
				std::getline(file, line);
				std::stringstream str_stream_offer(line);
				str_stream_offer >> node_index >> offer_num;
				for (int j = 0; j < offer_num; j++) {
					int offId, offPrice, offAvailability;
					str_stream_offer >> offId >> offPrice >> offAvailability;
					instance->offer_lists[node_index - 1][offId - 1].first = offPrice;
					instance->offer_lists[node_index - 1][offId - 1].second = offAvailability;
					instance->offer_sort_lists[offId - 1].push_back({ node_index - 1, offPrice, offAvailability });
				}
			}
		}
		/* 坐标，我也不知道和 NODE_COORD_SECTION 有啥区别 =_= */
		else if (keyword == "DISPLAY_DATA_SECTION") {
			int n = instance->dimension;
			instance->display_coordinates = std::vector<std::vector<double>>(n, std::vector<double>(2));

			for (int i = 0; i < n; i++) {
				int node_index;
				file >> node_index;
				file >> instance->display_coordinates[node_index - 1][0];
				file >> instance->display_coordinates[node_index - 1][1];
			}
		}
		/* 边格式，表示非完全图中边的互联关系 */
		else if (keyword == "EDGE_DATA_SECTION") {
			if (instance->edge_data_format == "EDGE_LIST") {
				auto edge_list = std::vector<std::pair<int, int>>();
				int i, j;
				file >> i;
				while (i != -1) {
					file >> j;
					edge_list.emplace_back(i, j);
					file >> i;
				}
				instance->edge_list = edge_list;
			}
		}
		/* 边的权重（路由开销） */
		else if (keyword == "EDGE_WEIGHT_SECTION") {
			int n = instance->dimension;
			auto distance_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n));

			if (instance->edge_weight_format == "FUNCTION") {
				std::cerr << "Edge weight format 'function' is not implemented yet " << std::endl;
			}
			else if (instance->edge_weight_format == "FULL_MATRIX") {
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < n; ++j) {
						file >> distance_matrix[i][j];
					}
				}
			}
			else if (instance->edge_weight_format == "UPPER_ROW") {
				for (int i = 0; i < n; ++i) {
					for (int j = i + 1; j < n; ++j) {
						file >> distance_matrix[i][j];
						distance_matrix[j][i] = distance_matrix[i][j];
					}
				}
			}
			else if (instance->edge_weight_format == "LOWER_ROW") {
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < i; ++j) {
						file >> distance_matrix[i][j];
						distance_matrix[j][i] = distance_matrix[i][j];
					}
				}
			}
			else if (instance->edge_weight_format == "UPPER_DIAG_ROW") {
				for (int i = 0; i < n; ++i) {
					for (int j = i; j < n; ++j) {
						file >> distance_matrix[i][j];
						distance_matrix[j][i] = distance_matrix[i][j];
					}
				}
			}
			else if (instance->edge_weight_format == "LOWER_DIAG_ROW") {
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j <= i; ++j) {
						file >> distance_matrix[i][j];
						distance_matrix[j][i] = distance_matrix[i][j];
					}
				}
			}
			else if (instance->edge_weight_format == "UPPER_COL") {
				//HEY WHY NOT WASTE SPATIAL LOCALITY AND THRASH OUR CACHE
				for (int i = 0; i < n; ++i) {
					for (int j = i + 1; j < n; ++j) {
						file >> distance_matrix[j][i];
						distance_matrix[i][j] = distance_matrix[j][i];
					}
				}
			}
			else if (instance->edge_weight_format == "LOWER_COL") {
				//HEY WHY NOT WASTE SPATIAL LOCALITY AND THRASH OUR CACHE
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j < i; ++j) {
						file >> distance_matrix[j][i];
						distance_matrix[i][j] = distance_matrix[j][i];
					}
				}
			}
			else if (instance->edge_weight_format == "UPPER_DIAG_COL") {
				//HEY WHY NOT WASTE SPATIAL LOCALITY AND THRASH OUR CACHE
				for (int i = 0; i < n; ++i) {
					for (int j = i; j < n; ++j) {
						file >> distance_matrix[j][i];
						distance_matrix[i][j] = distance_matrix[j][i];
					}
				}
			}
			else if (instance->edge_weight_format == "LOWER_DIAG_COL") {
				//HEY WHY NOT WASTE SPATIAL LOCALITY AND THRASH OUR CACHE
				for (int i = 0; i < n; ++i) {
					for (int j = 0; j <= i; ++j) {
						file >> distance_matrix[j][i];
						distance_matrix[i][j] = distance_matrix[j][i];
					}
				}
			}
			else if (instance->edge_weight_format == "WEIGHT_LIST") {
				int i, j;
				file >> i;
				while (i != -1) {
					file >> j;
					file >> distance_matrix[i][j];
					distance_matrix[j][i] = distance_matrix[i][j];
				}
			}

			instance->distance_matrix = distance_matrix;
		}

		if (!file.eof()) {
			file >> keyword;
			if (keyword.compare(keyword.length() - 1, 1, ":") == 0) {
				keyword.pop_back();
			}
			std::getline(file, line);  // eats up Enter
		}
	}

	file.close();

	/*
	 * 根据距离函数计算距离
	 */
	if (!instance->distance_matrix.size()) {
		int n = instance->dimension;
		instance->distance_matrix = std::vector<std::vector<int>>(n, std::vector<int>(n));

		if (instance->edge_weight_type == "EUC_2D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					int dist = std::lround(std::sqrt((dx * dx) + (dy * dy)));
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
		else if (instance->edge_weight_type == "EUC_3D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					double dz = instance->coordinates[i][2] - instance->coordinates[j][2];
					int dist = std::lround(std::sqrt((dx * dx) + (dy * dy) + (dz * dz)));
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
		else if (instance->edge_weight_type == "MAN_2D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					int dist = std::lround(dx + dy);
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
		else if (instance->edge_weight_type == "MAN_3D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					double dz = instance->coordinates[i][2] - instance->coordinates[j][2];
					int dist = std::lround(dx + dy + dz);
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
		else if (instance->edge_weight_type == "MAX_2D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					int dist = std::lround(std::max(dx, dy));
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
		else if (instance->edge_weight_type == "MAX_3D") {
			for (int i = 0; i < n; ++i) {
				for (int j = i + 1; j < n; ++j) {
					double dx = instance->coordinates[i][0] - instance->coordinates[j][0];
					double dy = instance->coordinates[i][1] - instance->coordinates[j][1];
					double dz = instance->coordinates[i][2] - instance->coordinates[j][2];
					int dist = std::lround(std::max(std::max(dx, dy), dz));
					instance->distance_matrix[i][j] = dist;
					instance->distance_matrix[j][i] = dist;
				}
			}
		}
	}

	/*
	 * 按照商品价格升序排序，存入 offer_sort_lists
	 */
	for (int i = 0; i < instance->offer_sort_lists.size(); i++) {
		sort(
			instance->offer_sort_lists[i].begin(), 
			instance->offer_sort_lists[i].end(),
			[](const NodePriQua &a, const NodePriQua &b) { return a.price < b.price; }
		);
	}

	return instance;
}