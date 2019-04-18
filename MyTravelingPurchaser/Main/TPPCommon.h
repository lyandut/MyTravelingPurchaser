#pragma once
#include <iostream>
#include <vector>

using PriQua = std::pair<int, int>;
typedef struct {
	int nodeNo;
	int price;
	int quantity;
}NodePriQua;

namespace lyan {
	using Tour = std::vector<int>;
	using DemmandList = std::vector<int>;
	// OfferList[n][k] - node n supply product k with <price, quantity>
	using OfferList = std::vector<std::vector<PriQua>>;
	// OfferSortList[k][i] - prioduct k, i_th element {nodeNo, price, quantity}
	using OfferSortList = std::vector<std::vector<NodePriQua>>;
	using OfferTable = std::vector<int>;
	using PlanTable = std::vector<OfferTable>;
	using DisMatrix = std::vector<std::vector<int>>;
	using ExcPair = std::pair<int, int>;
	using ExcPairList = std::vector<ExcPair>;
}

struct Solution{
	int opitimization;   // ���Ž� 
	lyan::Tour tour;     // ·�ɱ�
	lyan::PlanTable planTable; // �ɹ��ƻ�: planTable[i][h] - �ӽڵ� i �����Ʒ h ������
	Solution(int opt, lyan::Tour tour, lyan::PlanTable plan) 
		: opitimization(opt), tour(tour), planTable(plan) {}
	Solution() {}
};

typedef std::vector<Solution> Solutions;