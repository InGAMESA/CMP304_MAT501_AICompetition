#pragma once
#include <vector>
#include "WorldState.h"

class MCTSNode
{
public:
	MCTSNode();
	~MCTSNode();


	// MCTS steps
	MCTSNode* Select();
	MCTSNode* Expand();
	void Simulate();
	void Backpropagate(int reward);

	// tree functions
	MCTSNode* FindHighestRankingChild();
	void resetNode();
	void CalcResult();

	void setParent(MCTSNode* newParent) { parent = newParent; }
	void setGameState(WorldState newWorldState);

	//status
	std::vector<GameAction> availableMoves; 

private:

	//MCTS Node data - traditional data wins/visits are below, more may need to be added
	int wins; // number of times won
	int visits; // number of times visited

	//tree variables
	std::vector<MCTSNode*> branches; // child nodes
	MCTSNode* parent; // parent node

	// status tracking
	WorldState worldState; // game state at this node
	bool isEndState; // if either game is won or no moves possible

	void generatePossibleMoves();
	int simulationOutcome;
};

