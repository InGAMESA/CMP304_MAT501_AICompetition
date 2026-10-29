#include "AIConstructor_BT.h"
#include "AIActor_Driver.h"

bool AIConstructor_BT::Init() {

	method = AIConstructorBase::Method::BT;
	return true;
}







/*
DefineActions creates all the possible actions that the AI can perform
*/
	void AIConstructor_BT::DefineActions()
	{

		// TASK TODO - Add Additional Actions
		
	

		//Define the function accelerating 
		auto accelFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
			AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

			// example of getting a blackboard value
			float curXpos = bb.GetValue<float>("TrackXPos");

			// example of applying an action
			// other functions are 
			//   	actor->Turn();
			//		actor->Decel();
					

			actor->Accel(20.f);


			// NOTE: If you are using an AI Method where actions continue beween updates (e.g. Behaviour Trees)
			//   actions should return ACTION_RUNNING, ACTION_SUCCESS or ACTION_FAILURE as appropriate
			//   to ensure that the same action will repeatedly be called each update tick
			return ActionStatus::ACTION_RUNNING;
			};


		// Link the function to an identifier
		AddActionByName("ActionAccel", accelFunction);


	}


	/*
		DefineConsiderations creates all Considerations that will be linked to Options
	// Options will only be selected if the Condition is true
	*/
	void AIConstructor_BT::DefineConsiderations()
	{


		// is there NOT an obstacle of any type in reactDistance

		auto cNotObstacle = [](AIBrainBlackboardBase& bb) -> bool {

			int reactDistance = 10;

			// get all collision types from blackboard
			std::vector< AIMath_Global::Vector2f > _oil = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleOil");
			std::vector< AIMath_Global::Vector2f > _bollard = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleBollard");
			std::vector< AIMath_Global::Vector2f > _trash = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleTrash");

			// check if in car's path
			bool obstacleInPath = false;
			float curX = bb.GetValue<float>("TrackXPos");

			// check collision against a boundry distance
			float _collideDist = 0.1f;

			for (auto c : _oil)
			{
				if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
				{
					return false;
				}
			}

			for (auto c : _bollard)
			{
				if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
				{
					return false;
				}
			}

			for (auto c : _trash)
			{
				if (abs(c.x - curX) < _collideDist && c.y <= reactDistance)
				{
					return false;
				}
			}
			return(true);

			};



		AddConsiderationByName("ConsiderationNOTObstacle", cNotObstacle);




	}

/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_BT::DefineOptions()
{


	// Set the Node Type for the Root Node
	rootType = AIReasonerBase::NodeType::Fallback;

	// - LEAF NODES - ACTIONS - 
	// Add any leaf nodes (actions) using AddOptionByName()


	
	AddOptionByName("OptionAccel", "ActionAccel");

	


	// - CONTROL NODES - SUB REASONERS - 
	// Add any Control Nodes using AddControlNodeByName()
	// You do not need to create the Root, that node is already created (called "Root")

	
	AddControlNodeByName("OptionAccelSeq", AIReasonerBase::Sequence);


	// - CONSIDERATIONS - 
	// Add any considerations to Decorators using AddDecoratorConsideration()
	// Add any considerations to Leaf nodes using AddOptionConsideration()


	// example - 
	AddOptionConsideration("OptionAccel", "ConsiderationNOTObstacle");




	// -  TREE CONNECTIONS - 
	// Add connections between nodes using AddOptionToTreeNode()
	

	AddOptionToTreeNode("Root", "OptionAccelSeq");

	AddOptionToTreeNode("OptionAccelSeq", "OptionAccel");





}




















// ** Helper Functions **
// these link to the AI Framework and should not be edited 



void AIConstructor_BT::AddControlNodeByName(std::string _subReasonerName, AIReasonerBase::NodeType _nodeType, int _priority)
{



	// create the Action - a SubReasoner Actions
	std::shared_ptr<AIActionSubReasoner> _subReasoner = std::make_shared<AIActionSubReasoner>();

	_subReasoner->Init(_subReasonerName, nullptr);
	_subReasoner->SetReasonerType(_nodeType);
	actions.insert({ _subReasonerName, _subReasoner });

	

	// create the option, and link to the Action
	AIOptionDefinition _option = AIOptionDefinition();
	_option.SetOptionName(_subReasonerName);
	_option.SetActionName(_subReasonerName);
	_option.SetPriority(_priority);
	options.insert({ _subReasonerName, _option });


}



void AIConstructor_BT::AddDecoratorConsideration(std::string _decorator, std::string  _consideration) {

	decoratorConditions.insert({ _decorator, _consideration,  });


}

std::vector< std::shared_ptr<AIConsiderationBase>> AIConstructor_BT::GetConsiderationsForKey(std::string _decorator)
{

	std::vector< std::shared_ptr<AIConsiderationBase>> _matches;
	for (std::multimap<std::string, std::string>::iterator it = decoratorConditions.begin(); it != decoratorConditions.end(); ++it) {

		if (it->first == _decorator)
		{
			_matches.push_back(considerations[it->second]);
		}
	}

	return _matches;
}