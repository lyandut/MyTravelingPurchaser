#pragma once
#include <iostream>
#include <vector>

using PriQua = std::pair<int, int>;

namespace lyan {
	using Tour = std::vector<int>;
	using DemmandList = std::vector<int>;
	using OfferList = std::vector<std::vector<PriQua>>;
	using OfferTable = std::vector<int>;
	using PlanTable = std::vector<OfferTable>;
	using DisMatrix = std::vector<std::vector<int>>;
}

typedef struct {
	int nodeNo;
	int price;
	int quantity;
}NodePriQua;

typedef struct {
	int opitimization;   // 最优解 
	lyan::Tour tour;     // 路由表
	lyan::PlanTable planTable; // 采购计划: planTable[i][h] - 从节点 i 购买产品 h 的数量
}Solution;

typedef std::vector<Solution> Solutions;