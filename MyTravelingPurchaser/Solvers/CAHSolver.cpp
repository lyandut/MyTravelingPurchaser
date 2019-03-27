#include "CAHSolver.h"

Solutions * CAHSolver::construct(std::vector<std::vector<int>> distance_matrix, unsigned int n)
{
	int *travelled = new int[n];
	for (int i = 1; i < n; ++i)
		travelled[i] = 0;
	travelled[0] = 1;

	auto tour = std::vector<unsigned int>();
	tour.emplace_back(0);

	int total = 1;
	int objective = 0;
	int node = 0;
	int min_index = -1;
	while (total < n) {
		int min = -1;
		min_index = -1;
		for (int i = 0; i < n; ++i) {
			if (i == node || travelled[i] == 1)continue;
			if (min == -1) {
				min = distance_matrix[node][i];
				min_index = i;
			}
			else if (min > distance_matrix[node][i]) {
				min = distance_matrix[node][i];
				min_index = i;
			}
		}
		objective += distance_matrix[node][min_index];
		tour.emplace_back(min_index);
		node = min_index;
		total++;
		travelled[node] = 1;
	}

	objective += distance_matrix[min_index][0];

	Solution sol = Solution();

	sol.first = tour;
	sol.second = objective;

	auto *result = new Solutions();
	result->emplace_back(sol);
	return result;
}
