#include "MyMazeGenerator.hpp"
#include <iostream>
#include "Maze.hpp"
#include "HelperFunctions.hpp"

void MyMazeGenerator::generateMaze(Maze& maze)
{
	visited.assign(maze.getWidth(), std::vector<int>(maze.getHeight(), 0));

	maze.addAllWalls();

	int start_x = 0;
	int start_y = 0;

	recursiveFunc(maze, start_x, start_y);

	return;
}


void MyMazeGenerator::recursiveFunc(Maze& maze, int x, int y)
{
	visited[x][y] = 1;
	std::vector<int> directions;
	
	int mazeWidth = maze.getWidth();
	int mazeHeight = maze.getHeight();
	int mazeSize = (mazeWidth * mazeHeight) - 1;
	
	
	if((y > 0) && (visited[x][y - 1] == 0) && (coordinateToCell(x, y - 1, mazeWidth) >= 0) && (coordinateToCell(x, y - 1, mazeWidth) <= mazeSize) )
	{
		directions.push_back(0); // up
	}

	if((y < mazeHeight - 1) && (visited[x][y + 1] == 0) && (coordinateToCell(x, y + 1, mazeWidth) >= 0) && (coordinateToCell(x, y + 1, mazeWidth) <= mazeSize) )
	{
		directions.push_back(1); // down
	}

	if((x < mazeWidth - 1) && (visited[x + 1][y] == 0) && (coordinateToCell(x + 1, y, mazeWidth) >= 0) && (coordinateToCell(x + 1, y, mazeWidth) <= mazeSize) )
	{
		directions.push_back(2); // right
	}

	if((x > 0) && (visited[x - 1][y] == 0) && (coordinateToCell(x - 1, y, mazeWidth) >= 0) && (coordinateToCell(x - 1, y, mazeWidth) <= mazeSize) )
	{
		directions.push_back(3); // left
	}
	
	
	if(directions.size() == 0) 	// no possible directions
	{
		return;
	}
	

	switch(calcRandDirection(directions))
	{
		case 0: // up
		{
			maze.removeWall(x, y, Direction::up);
			recursiveFunc(maze, x, y - 1);
			break;
		}

		case 1: // down
		{
			maze.removeWall(x, y, Direction::down);
			recursiveFunc(maze, x, y + 1);
			break;
		}

		case 2: // right
		{
			maze.removeWall(x, y, Direction::right);
			recursiveFunc(maze, x + 1, y);
			break;
		}

		case 3: // left
		{
			maze.removeWall(x, y, Direction::left);
			recursiveFunc(maze, x - 1, y);
			break;
		}
	}
	recursiveFunc(maze, x, y);
}
