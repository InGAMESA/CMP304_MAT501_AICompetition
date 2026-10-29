#pragma once
#include "../AI_Framework/framework.h"
#include "MCTSNode.h"
class MCTS_Reasoner :
    public AIReasonerBase
{
public:
	bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
	void Think() override;

private:
	MCTSNode* rootNode;

	AIBlackboard_Global* Global_Blackboard;
};

