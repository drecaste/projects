#include "HelperFunctions.hpp"

int coordinateToCell(int x, int y, int mazeWidth)
{
	int result = y * mazeWidth + x;
	
	return result;
}

int calcRandDirection(std::vector<int>directions)
{
	int Number = rand() % (directions.size());
	int randNumber = directions[Number];

	return randNumber;
}
