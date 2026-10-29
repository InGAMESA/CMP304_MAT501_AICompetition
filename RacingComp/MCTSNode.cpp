#include "MCTSNode.h"
#include "AIRandom_Global.h"

#include <iostream>
// class functions
MCTSNode::MCTSNode()
{
	isEndState = false;
	parent = nullptr;
	wins = 0;
	visits = 0;
	simulationOutcome = 0;

}

MCTSNode::~MCTSNode()
{
	// parent will be deleted by its parent so we don't need to clean it up here
	parent = nullptr;

	for (auto branch : branches)
	{
		delete branch;
	}
	branches.clear();
	availableMoves.clear();

}



/*
* Select - will perform the Select step of the MCTS algorithm
*  will select down a path according to the selection policy until leaf is found
*/
MCTSNode* MCTSNode::Select()
{

	if (availableMoves.size() > 0)
	{
		// still unexplored branches, explore
		return this;
	}

	if (isEndState)
	{
		// leaf node, return and will be handled by Reasoner
		return this;
	}



	// TASK TODO - replace below below code with UCT based selection
	//  - remember to call 'Select()' on the node 




	int randomBranch = rand() % branches.size();
	return branches[randomBranch]->Select();





}

/*
* Expand  - will perform the Expansion step of the MCTS algorithm
* will add a new node by randomly selecting from all possible moves
*/
MCTSNode* MCTSNode::Expand()
{

	if (isEndState)
	{
		//std::cout << "Cannont expand - leaf " << std::endl;
		return NULL; // if this is game end state, we cannot expand
	}
	else
	{
		// pick a random move from, the unexplored ones

		if (availableMoves.size() == 0)
		{
			std::cout << "WARNING [MCTS Expand]: no avaialble move, cannot expand, returning nullptr " << std::endl;
			return nullptr;
		}

		int randomMove = rand() % availableMoves.size();
		GameAction nextAction = availableMoves[randomMove];

		// delete the selected move from vector
		availableMoves.erase(availableMoves.begin() + randomMove);

		// create a new child node 
		MCTSNode* childNode = new MCTSNode();
		childNode->setParent(this);


		// create the new world state and set to current node's state
		WorldState newWorldState = this->worldState;


		// apply the move to the world state and set the child's state
		newWorldState.SetAndApplyAction(nextAction);
		childNode->setGameState(newWorldState);

		// add the newly created node as a child of current node		
		branches.push_back(childNode);

		// reutrn node, for simulation
		return childNode;
	}

}
#/*
* Simulate  - will perform the Simulate step of the MCTS algorithm
* will play random moves until a end state is found, and set the simulationWinner
*/
void MCTSNode::Simulate()
{


	// TODO - This code will randomly pick a winner - replace this code with your own simulation code
	if (AIRandom_Global::getInstance()->GetRandom01() >= 0.5f)
	{
		simulationOutcome = 0;
	}
	else
	{
		simulationOutcome = 1;
	}



	//TASK TODO - replace above code with the simulation functionality
	// - ensure you set 'simulationWinner' in this function

}



/*
* CalcResult will determing if the AI won or lost, and trigger the back propagation
*/

void MCTSNode::CalcResult()
{

	
	int result = 0;


	// TODO must call Backpropagate with the result value based on leaf node outcome


	Backpropagate(result);
}



/*
* Backpropagate  - will perform the Backpropagate step of the MCTS algorithm
* the result will be returned back up the tree branches to the root node
*/

void MCTSNode::Backpropagate(int result)
{

	//TASK TODO

	// adjust wins, visits or other tracking data

	// if the node has a parent, call parent->Backpropagate with the results
	// the node will be NULL if there is no parment


	if (parent != nullptr)
		parent->Backpropagate(result);

}



/**
*  generatePossibleMoves will calculate the possible moves from this node and set the isEndState variable appropriately
*  should only be called once when node's game state is defined
*/
void MCTSNode::generatePossibleMoves()
{
	// *** TODO *** based on WorldState, populate availableMoves with possbile actions
	
}


/**
*  setGameState will set the game state for the node, as well as calling generatePossibleMoves to update status
*/

void MCTSNode::setGameState(WorldState newWorldState)
{
	worldState = newWorldState;
	generatePossibleMoves();
}


MCTSNode* MCTSNode::FindHighestRankingChild()
{

	if (branches.size() == 0)
		return NULL;

	float maxWins = 0;
	int maxIndex = 0;

	for (int i = 0; i < branches.size(); i++)
	{

		float nodeWinRate = ((float)branches[i]->wins) / branches[i]->visits;

		if (nodeWinRate > maxWins)
		{
			maxIndex = i;
			maxWins = nodeWinRate;
		}
	}

	return branches[maxIndex];
}


#/*
* resetNode will return the node to its default values
*/

void MCTSNode::resetNode()
{
	branches.clear();
	wins = 0;
	visits = 0;
	availableMoves.clear();
}