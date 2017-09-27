#ifndef MYMAZESOLVER_HPP
#define MYMAZESOLVER_HPP

#include "MazeSolver.hpp"
#include "Maze.hpp"
#include "MazeSolution.hpp"


class MyMazeSolver : public MazeSolver
{
	public:
	
		std::vector<std::vector<int>> visited;

		void solveMaze(const Maze& maze, MazeSolution& mazeSolution);

		void recursiveSolveF(const Maze& maze, MazeSolution& mazeSolution, int x, int y);
};

#endif // MYMAZESOLVER_HPP
