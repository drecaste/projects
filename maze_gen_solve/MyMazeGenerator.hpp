#ifndef MYMAZEGENERATOR_HPP
#define MYMAZEGENERATOR_HPP

#include "MazeGenerator.hpp"
#include <vector>


class MyMazeGenerator : public MazeGenerator
{
	public:
		
		std::vector<std::vector<int>> visited;
		
		void generateMaze(Maze& maze);

		void recursiveFunc(Maze& maze, int x, int y);
};

#endif // MYMAZEGENERATOR_HPP
