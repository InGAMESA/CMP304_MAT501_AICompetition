#include "AIBrain_Driver.h"
#include "RBS_Reasoner.h"
#include "FSM_Reasoner.h"
#include "DT_Reasoner.h"
#include "BT_ReasonerNode.h"
#include "MCTS_Reasoner.h"
#include "AIConstructor_BT.h"
#include "BT_DecoratorNode.h"
#include "BT_FallbackNode.h"
#include "BT_SequenceNode.h"


void AIBrain_Driver::Update()
{
	// call the reasoner to get the selected action
	brainReasoner->Update();

}

bool AIBrain_Driver::Init(AIConstructorBase& _constructor, AIActorBase& _actorContext)
{
	brainBlackboard = std::make_unique<AIBrainBlackboard>();
	brainBlackboard->Init(_actorContext);

	// create the brain based on the constructor method
	if (_constructor.method == AIConstructorBase::Method::RBS)
	{
		brainReasoner = std::make_unique < RBS_Reasoner >();

		brainReasoner->SetOptions(_constructor);

		brainReasoner->Init("RBS Reasoner", *brainBlackboard);

	}
	else if(_constructor.method == AIConstructorBase::Method::FSM)
	{
		brainReasoner = std::make_unique < FSM_Reasoner >();

		brainReasoner->SetOptions(_constructor);

		brainReasoner->Init("FMS Reasoner", *brainBlackboard);

		FSM_Reasoner* _reasoner = static_cast<FSM_Reasoner*>(GetReasoner());
		_reasoner->SetStartingState(FSM_Reasoner::DriverState::Accelerating, "OptionAccelerate");
	}
	else if (_constructor.method == AIConstructorBase::Method::BT)
	{
		AIConstructor_BT* _btConstructor = static_cast<AIConstructor_BT*>(&_constructor);

		if (_btConstructor->rootType == AIReasonerBase::NodeType::Decorator)
		{
			brainReasoner = std::make_unique<BT_DecoratorNode>();

		}
		else if (_btConstructor->rootType == AIReasonerBase::NodeType::Fallback)
		{
			brainReasoner = std::make_unique <BT_FallbackNode>();
		}
		else if (_btConstructor->rootType == AIReasonerBase::NodeType::Sequence)
		{
			brainReasoner = std::make_unique <BT_SequenceNode>();
		}


		// the reasoner must be called 'Root' is using the helper functions for construction
		brainReasoner->Init("Root", *brainBlackboard);
		brainReasoner->SetOptions(_constructor);
	}
	else if (_constructor.method == AIConstructorBase::Method::DT)
	{
		brainReasoner = std::make_unique<DT_Reasoner>();

		// the reasoner must be called 'Root' is using the helper functions for construction
		brainReasoner->Init("Root", *brainBlackboard);
		brainReasoner->SetOptions(_constructor);
	}
	else if (_constructor.method == AIConstructorBase::Method::MCTS)
	{
		brainReasoner = std::make_unique<MCTS_Reasoner>();

		brainReasoner->SetOptions(_constructor);

		brainReasoner->Init("MCTS Reasoner", *brainBlackboard);

	}

	return true;
}
