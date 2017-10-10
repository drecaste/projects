#ifndef MYOTHELLOAI_HPP
#define MYOTHELLOAI_HPP


#include "OthelloAI.hpp"
#include "OthelloGameState.hpp"
#include "OthelloBoard.hpp"


namespace mine
{
	class MyOthelloAI : public OthelloAI
	{
		public:
			virtual std::pair<int, int> chooseMove(const OthelloGameState& state);

			int search(OthelloGameState& state, int depth);

			int evaluationFunction(OthelloGameState& state);
	};
}


#endif // MYOTHELLOAI_HPP
