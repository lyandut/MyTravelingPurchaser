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
	double opitimization;   // ���Ž� 
	std::vector<int> tour;  // ·�ɱ�
	std::vector<std::vector<int>> planTable; // �ɹ��ƻ�: planTable[i][h] - �ӽڵ� i �����Ʒ h ������
}Solution;

typedef std::vector<Solution> Solutions;