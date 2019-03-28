#include "CAHSolver.h"

Solutions * CAHSolver::construct(
	unsigned int dimension,
	std::vector<int> demands,
	std::vector<std::vector<PriAva>> offer_lists,
	std::vector<std::vector<int>> distance_matrix
) {
	std::vector<int> isTravelled(dimension, 0);
	isTravelled[0] = 1;

	auto tour = std::vector<unsigned int>();
	tour.emplace_back(0);

	/* Initialization */
	// [TODO] demands 随机排序
	int h = 0;  // 产品编号 -> 需求量: demands[h]
	int currNode = 0;

	/* First units of product 1 */
	double minCost = -1.0;
	for (int i = 0; i < dimension; i++) {
		if (currNode == i || isTravelled[i]) continue;
		double currCost = 2.0*distance_matrix[currNode][i] / (double)offer_lists[i][h].second + (double)offer_lists[i][h].first;
		if (currCost < minCost || minCost == -1.0) {
			minCost = currCost;
			currNode = i;
		}
	}
	tour.emplace_back(currNode);
	isTravelled[currNode] = 1;

	/* Remaining units of product 1 */
	/* pass when demands[h]=1 */
	
	
	/* return */
	auto *result = new Solutions();
	return result;
}
