#include "MCTS_Reasoner.h"

bool MCTS_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context)
{
	actorBlackboard = &_context;
	reasonerID = _id;
	Global_Blackboard = AIBlackboard_Global::getInstance();
	return true;

}


void MCTS_Reasoner::Think()
{

	// create a new MCTS TicTacToeNode which will be used to create the tree
	rootNode = new MCTSNode();

	//TODO - update blackboard for status tracking


	//set current main game state

		// var to track the number of iterations
	int runCount = 0;

	int _maxRuns = 1000; // TODO configure

	do {

		// Select - choose a node that will be expanded
		MCTSNode* selectedNode = rootNode->Select();

		// Expand - expand the tree from this node
		MCTSNode* expandedNode = selectedNode->Expand();

		// expandedNode will be NULL if it's a leaf node and simulation is not possible
		if (!expandedNode == NULL)
		{
			// Simulate - Simulate to an end point, starting with aiMarker turn. 	
			expandedNode->Simulate();

			// CalcResult will trigger the back propagation of the result to the root node
			expandedNode->CalcResult();
		}

		runCount++;

	} while (runCount < _maxRuns);


	// perform the action - find child node with highest ranking
	MCTSNode* highestChild = rootNode->FindHighestRankingChild();
	//GameAction bestAction = highestChild->getGameState().gameAction;

	// only a single option - make a move
	selectedOption = options[0].get();



	// TODO - update blackbaord with any data
	// brain or global


}