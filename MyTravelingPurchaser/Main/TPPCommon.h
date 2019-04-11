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
	int opitimization;   // ���Ž� 
	lyan::Tour tour;     // ·�ɱ�
	lyan::PlanTable planTable; // �ɹ��ƻ�: planTable[i][h] - �ӽڵ� i �����Ʒ h ������
}Solution;

typedef std::vector<Solution> Solutions;