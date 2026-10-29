#include "BT_ConcurrentNode.h"

#include "AIConstructor_BT.h"
#include "BT_DecoratorNode.h"
#include "BT_FallbackNode.h"
#include "BT_SequenceNode.h"

void BT_ConcurrentNode::Think()
{

	//BONUS TASK TODO - complete logic if needed

}


ActionStatus BT_ConcurrentNode::Act()
{

	//BONUS TASK TODO - complete logic if needed




	return GetStatus();
}

void BT_ConcurrentNode::Reset()
{

	BT_ReasonerNode::Reset();
	runningOptionIndex = 0;
	selectedOption = nullptr;
	SetStatus(ActionStatus::ACTION_IDLE);
}

