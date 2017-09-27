#include"MyOthelloAI.hpp"
#include<iostream>

// Extremely Basic Evaluation Function
int mine::MyOthelloAI::evaluationFunction(OthelloGameState& state)
{
	if(state.isWhiteTurn())
	{
		return state.whiteScore() - state.blackScore();
	}
	return state.blackScore() - state.whiteScore();
}


std::pair<int, int> mine::MyOthelloAI::chooseMove(const OthelloGameState& state)
{
	std::pair<int, int> move (0, 0);
	int currentEvaluation = -1000;

	for(int i = 0; i < state.board().width(); i++)
	{
		for(int j = 0; j < state.board().width(); j++)
		{
			if(state.isValidMove(i, j))
			{
				std::unique_ptr<OthelloGameState> tempState = state.clone();
				tempState->makeMove(i, j);
				int tempEvaluation = search(* tempState, 3); // depth = 3
				
				if(currentEvaluation < tempEvaluation)
				{
					currentEvaluation = tempEvaluation;
					move.first = i;
					move.second = j;
				}
			}
		}
	}
	return move;
}


int mine::MyOthelloAI::search(OthelloGameState& state, int depth)
{
	int score = evaluationFunction(state);

	if(depth == 0)
	{
		return evaluationFunction(state);
	}

	else
	{
		if(state.isWhiteTurn())
		{
			for(int i = 0; i < state.board().width(); i++)
			{
				for(int j = 0; j < state.board().width(); j++)
				{
					if(state.isValidMove(i, j))
					{
						std::unique_ptr<OthelloGameState> altState = state.clone();
						altState->makeMove(i, j);
						int temp = search(* altState, depth - 1);
						if(temp < score)
						{
							score = temp;
						}
					}
				}
			}
			return score;
		}
		else
		{
			for(int i = 0; i < state.board().width(); i++)
			{
				for(int j = 0; j < state.board().width(); j++)
				{
					if(state.isValidMove(i, j))
					{
						std::unique_ptr<OthelloGameState> altState = state.clone();
						altState->makeMove(i, j);
						int temp = search(* altState, depth - 1);
						if(temp > score)
						{
							score = temp;
						}
					}
				}
			}
			return score;
		}
	}
}