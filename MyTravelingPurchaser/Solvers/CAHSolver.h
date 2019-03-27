#pragma once

#include "./ConstructionHeuristic.h"

class CAHSolver : public ConstructionHeuristic {
public:
	Solutions * construct(std::vector<std::vector<int>> distance_matrix, unsigned int n) override;
};

