#include "AIConstructor_FSM.h"
#include "AIActor_Driver.h"


bool AIConstructor_FSM::Init() {

	method = AIConstructorBase::Method::FSM;
	return true;
}


/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_FSM::DefineActions()
{

	//Define the function for Accelerating
	auto accelFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		actor->Accel(20.f);

		// NOTE: If you are using an AI Method where actions continue beween updates (e.g. Behaviour Trees)
		//   actions should return ACTION_RUNNING, ACTION_SUCCESS or ACTION_FAILURE as appropriate
		//   to ensure that the same action will repeatedly be called each update tick
		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionAccelerate", accelFunction);


	//Define the function for Steering to centre of track
	auto centreFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_Driver* actor = static_cast<AIActor_Driver*>(bb.GetActorContext());

		float curSteer = bb.GetValue<float>("TrackXPos");

		if (curSteer > 0.2)
			actor->Turn(-0.05f);
		else if (curSteer < -0.2)
			actor->Turn(0.05f);
		
		// NOTE: If you are using an AI Method where actions continue beween updates (e.g. Behaviour Trees)
		//   actions should return ACTION_RUNNING, ACTION_SUCCESS or ACTION_FAILURE as appropriate
		//   to ensure that the same action will repeatedly be called each update tick
		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionCentre", centreFunction);

}


/*
	DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_FSM::DefineConsiderations()
{

	// See other AIConstructors for example Consideration

}



/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_FSM::DefineOptions()
{

	// create the options for the AI Reasoner to select from
	AddOptionByName("OptionAccelerate", "ActionAccelerate");
	AddOptionByName("OptionCentre", "ActionCentre");

}



