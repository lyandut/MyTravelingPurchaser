#pragma once
#include "../TPPLIBInstance/TPPLIBInstance.h"

typedef struct {
	double opitimization;   // ���Ž� 
	std::vector<int> tour;  // ·�ɱ�
	std::vector<std::vector<int>> planTable; // �ɹ��ƻ�
}Solution;
typedef std::vector<Solution> Solutions;


class ConstructionHeuristic {
public:
	virtual Solutions construct(
		unsigned int dimension,
		const std::vector<int> &demands,
		const std::vector<std::vector<PriAva>> &offer_lists,
		const std::vector<std::vector<int>> &distance_matrix
	) = 0;
};
