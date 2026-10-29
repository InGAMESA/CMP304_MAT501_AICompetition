#include "AIConstructor_DT.h"

#include "AIActor_Driver.h"

bool AIConstructor_DT::Init() {

	method = AIConstructorBase::Method::DT;
	return true;
}



/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_DT::DefineActions()
{

	// TASK TODO - Add Additional Actions
	 
	
	//Define the function for  turning away from collision
	auto avoidFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		// always turn left
		actor->Turn(-0.5f);

		// NOTE: If you are using an AI Method where actions continue beween updates (e.g. Behaviour Trees)
		//   actions should return ACTION_RUNNING, ACTION_SUCCESS or ACTION_FAILURE as appropriate
		//   to ensure that the same action will repeatedly be called each update tick
		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAvoid", avoidFunction);




	//Define the function accelerating and centering
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
		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAccel", accelFunction);


}


/*
	DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_DT::DefineConsiderations()
{


	// is there an obstacle of any type in reactDistance
	auto cObstacle = [](AIBrainBlackboardBase& bb) -> bool {

		int reactDistance = 10;

		// get all collision types from blackboard
		std::vector< AIMath_Global::Vector2f > _oil = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleOil");
		std::vector< AIMath_Global::Vector2f > _bollard = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleBollard");
		std::vector< AIMath_Global::Vector2f > _trash = bb.GetValue<	std::vector< AIMath_Global::Vector2f > >("ObstacleTrash");

		// check if in car's path
		bool obstacleInPath = false;
		float curX = bb.GetValue<float>("TrackXPos");
		float _collideDist = 0.1f;

		for (auto c : _oil)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Oil detected at " << c.y << std::endl;
				return true;
			}
		}

		for (auto c : _bollard)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Bollard detected at " << c.y << std::endl;
				return true;
			}
		}

		for (auto c : _trash)
		{
			if (std::abs(c.x - curX) < _collideDist && c.y <= reactDistance)
			{
				//std::cout << " Trash detected at " << c.y << std::endl;
				return true;
			}
		}
		return(false);

		};



	AddConsiderationByName("ConsiderationObstacle", cObstacle);


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
void AIConstructor_DT::DefineOptions()
{




	// TASK TODO
	// Add Leaf Node Actions using AddOptionByName()
	// 
	AddOptionByName("OptionAvoid", "ActionAvoid");
	AddOptionByName("OptionAccel", "ActionAccel");


	// Add Decision Tree Node Options (Sub Reasoner) using AddTreeNodeOptionByName()


	// TASK TODO
	// Add Considerations to Options using AddOptionConsideration()
	AddOptionConsideration("OptionAvoid", "ConsiderationObstacle");
	AddOptionConsideration("OptionAccel", "ConsiderationNOTObstacle");


	// TASK TODO
	// Create Tree Structure by adding Options to the Root and any Sub Reasoner Tree Node Option
	// Start with the Root - e.g. 	AddOptionToTreeNode("Root", "OptionMelee");
	// You will not need to create the Root, it is already created
	// Add Options to any SubReasoner Option 


	// Define Tree
	AddOptionToTreeNode("Root", "OptionAvoid");
	AddOptionToTreeNode("Root", "OptionAccel");











}

