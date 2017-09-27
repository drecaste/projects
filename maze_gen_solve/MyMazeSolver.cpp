#include "MyMazeSolver.hpp"
#include <iostream>
#include "HelperFunctions.hpp"


void MyMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution)
{
	visited.assign(maze.getWidth(), std::vector<int>(maze.getHeight(), 0));

	mazeSolution.restart();

	MyMazeSolver::recursiveSolveF(maze, mazeSolution, std::get<0>(mazeSolution.getStartingCell()), std::get<1>(mazeSolution.getStartingCell()));

	return;
}


void MyMazeSolver::recursiveSolveF(const Maze& maze, MazeSolution& mazeSolution, int x, int y)
{
	visited[x][y] = 1;
	int mazeWidth = maze.getWidth() - 1;
	int mazeHeight = maze.getHeight() - 1;

	std::vector<int> directions;

	while(!mazeSolution.isComplete())
	{
		if((y > 0) &&(visited[x][y - 1] == 0) && !(maze.wallExists(x, y, Direction::up)))
		{
			directions.push_back(0); // up
		}

		if((y < mazeHeight) && (visited[x][y + 1] == 0) && !(maze.wallExists(x, y, Direction::down)))
		{
			directions.push_back(1); // down
		}

		if((x < mazeWidth) && (visited[x + 1][y] == 0) && !(maze.wallExists(x, y, Direction::right)))
		{
			directions.push_back(2); // right
		}

		if((x > 0) && (visited[x - 1][y] == 0) && !(maze.wallExists(x, y, Direction::left)))
		{
			directions.push_back(3); // left
		}
		
		
		if((directions.size() == 0)) // no possible directions
		{
			mazeSolution.backUp();
			return recursiveSolveF(maze, mazeSolution, std::get<0>(mazeSolution.getCurrentCell()), std::get<1>(mazeSolution.getCurrentCell()));
		}

		
		switch(calcRandDirection(directions))
		{
			case 0: // up
			{
				mazeSolution.extend(Direction::up);
				recursiveSolveF(maze, mazeSolution, x, y - 1);
				break;
			}
		
			case 1: // down
			{
				mazeSolution.extend(Direction::down);
				recursiveSolveF(maze, mazeSolution, x, y + 1);
				break;

			}
			
			case 2: // right
			{
				mazeSolution.extend(Direction::right);
				recursiveSolveF(maze, mazeSolution, x + 1, y);
				break;
			}

			case 3: // left
			{
				mazeSolution.extend(Direction::left);
				recursiveSolveF(maze, mazeSolution, x - 1, y);
				break;
			}
		}
	}
}
