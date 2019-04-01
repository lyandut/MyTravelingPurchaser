#pragma once
#include <iostream>
#include <vector>

typedef std::pair<int, int> PriQua;

typedef struct {
	int nodeNo;
	int price;
	int quantity;
}NodePriQua;

typedef struct {
	double opitimization;   // 最优解 
	std::vector<int> tour;  // 路由表
	std::vector<std::vector<int>> planTable; // 采购计划: planTable[i][h] - 从节点 i 购买产品 h 的数量
}Solution;

typedef std::vector<Solution> Solutions;