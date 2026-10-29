#include "BT_SequenceNode.h"
#include "AIConstructor_BT.h"


#include "BT_DecoratorNode.h"
#include "BT_FallbackNode.h"
#include <iostream>



/*
* Think should check of the conditions (if any) for this node are met, and set the status accordingly
*/
void BT_SequenceNode::Think()
{
	ActionStatus _currentStatus = GetStatus();
	if (_currentStatus == ActionStatus::ACTION_IDLE)
	{
		Start();
	}
	else if (_currentStatus == ActionStatus::ACTION_RUNNING)
	{

		// select the current running index, but check if any of the conditions are met
		selectedOption = options[runningOptionIndex].get();

		std::vector<std::shared_ptr<AIConsiderationBase>> conditionList = selectedOption->GetOptionConsiderations();



		bool conditionsMet = true;
		for (auto c : conditionList)
		{
			bool ruleResult = c->Calculate(*actorBlackboard);

			if (!ruleResult)
			{
				std::cout << "[" << actorBlackboard->GetActorContext()->GetActorID() << "] BT Log: Sequence Node - " << selectedOption->GetOptionID() << " Consideration " << c->GetID() <<" was not TRUE " << std::endl;
				conditionsMet = false;
			}
		}

		if (!conditionsMet)
		{
			
			SetStatus(ActionStatus::ACTION_FAILURE);
		}
	}


}


/*
* Act should maintin the status tracking of the child nodes and handle their reporting of sucess and failure
*/
ActionStatus BT_SequenceNode::Act()
{
	if (selectedOption != nullptr)
	{


		// IF GetStatus() is FAILURE, the node conditions have failed, and so the Node should stop and report failuire to it's parent

		if (GetStatus() == ActionStatus::ACTION_FAILURE)
		{
			std::cout << "[" << actorBlackboard->GetActorContext()->GetActorID() << "] BT Log: SequenceNode " << reasonerID << " Action has Failed due to Option " << std::endl;
			return GetStatus();
		}
		else
		{



			//	# PSEUDOCODE
			//  
			// 	Get Current Child Node Status
			//	
			//  IF Current Child Node = SUCCESS:
			//		IF Current Child = Last Child:
			//			Status = SUCCESS
			//		ELSE:
			//			Move to next Child Node
			//	ELSE IF Current Child Node = FAILURE
			//		Status = FAILURE
			//	ELSE IF Current Child Node = RUNNING
			//		Status = RUNNING

			ActionStatus _actionResult = selectedOption->GetOptionAction()->PerformAction(*actorBlackboard);

			// if child node reports failure, move to next child if possible
			if (_actionResult == ActionStatus::ACTION_SUCCESS)
			{
				std::cout << "[" << actorBlackboard->GetActorContext()->GetActorID() << "] BT Log: Sequence Node " << reasonerID << " is moving to next option " << runningOptionIndex << " as success of " << selectedOption->GetOptionID() << std::endl;
				runningOptionIndex++;
				if (runningOptionIndex >= options.size())
				{
					std::cout << "[" << actorBlackboard->GetActorContext()->GetActorID() << "] BT Log: Sequence Node " << reasonerID << " Action has Completed  " << std::endl;
					SetStatus(ActionStatus::ACTION_SUCCESS);
				}
			}
			else if (_actionResult == ActionStatus::ACTION_FAILURE)
			{
				// if child node has suceeded, this Fallback node has suceeded
				std::cout << "[" << actorBlackboard->GetActorContext()->GetActorID() << "] BT Log: Sequence Node Failed - " << " due to failure of " << selectedOption->GetOptionID() << std::endl;
				SetStatus(ActionStatus::ACTION_FAILURE);
				if (reasonerID == "Root")
					Reset();
			}
			else
			{
				SetStatus(ActionStatus::ACTION_RUNNING);
			}


		}


	}


	// return GetStatus
	return GetStatus();
}

void BT_SequenceNode::Reset()
{

	BT_ReasonerNode::Reset();
	runningOptionIndex = 0;
	selectedOption = nullptr;
	SetStatus(ActionStatus::ACTION_IDLE);
}



