#include "FSM_Reasoner.h"



FSM_Reasoner::~FSM_Reasoner()
{


}


bool FSM_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context) {
	actorBlackboard = &_context;
	reasonerID = _id;

	return true;
}



/*
Think is called on demand to choose from the possible Options that AI has.

The function should implement the logic of the AI method to choose one Option as the selectedOption.
*/
void FSM_Reasoner::Think()
{
	// note current state
	DriverState _lastState = currentState;


	// Basic FMS code - switch state based on events
	//  currently, only event that causes change is state is related to the car's X position on the track
	float _xPos = actorBlackboard->GetValue<float>("TrackXPos");

	if (currentState == DriverState::Accelerating)
	{
		
		// if not at centre 
		if (_xPos > 0.2f || _xPos < -0.2f )
		{
			currentState = DriverState::Centering;
			selectedOption = GetOptionByName("OptionCentre");
		}


	
	}
	else if (currentState == DriverState::Centering)
	{
		// if at centre
		if (std::abs(_xPos) < 0.2)
		{
			currentState = DriverState::Accelerating;
			selectedOption = GetOptionByName("OptionAccelerate");
		}

	}


	// if state changed, call the Pre/Post Transition functions for cleanup/initialisation

	if (_lastState != currentState)
	{

		actorBlackboard->GetActorContext()->PreTransition();
	}


}



void FSM_Reasoner::SetStartingState(DriverState _state, std::string _stateName)
{
	currentState = _state;
	selectedOption = GetOptionByName(_stateName);

}
