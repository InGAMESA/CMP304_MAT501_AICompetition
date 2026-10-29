#include "AIActor_Driver.h"
#include "AIBrain_Driver.h"
#include "GameController.h"

AIActor_Driver::AIActor_Driver()
{

}



// **  AIActorBase overrides **

bool AIActor_Driver::Init(std::string _id, AIConstructorBase & _constructor)
{
	actorID = _id;

	// create new brain
	brain = std::make_unique<AIBrain_Driver>();

	brain->Init(_constructor, *this);



	return true;
}

/*
* SetGameContext will set the game controller context
*
 - AI decision making data should be on the blackboard, not accessed via the game controller

*/
void AIActor_Driver::SetGameContext(GameController& _gameController) {

	gameController = &_gameController;
}


void AIActor_Driver::SetDriverCar(Car& _car)
{
	driverCar = &_car;
}

/*
* Update will cause the AI Actor to use the brain to make a decision
*/
void AIActor_Driver::Update(float _dt)
{
	brain->Update();
}



/*
* unsed, but can be used for initialisation
*/
void AIActor_Driver::PreTransition()
{

}


/*
* Accel will increase the driver's car acceleation axis by the amount provided
* Must be a positive value
*/
ActionStatus AIActor_Driver::Accel(float _amount)
{

	driverCar->updateAccelChange = _amount;


	return ActionStatus::ACTION_SUCCESS;

}

/*
* Accel will decrease the driver's car acceleation axis by the amount provided 
* * Must be a positive value
*/
ActionStatus AIActor_Driver::Decel(float _amount)
{

	driverCar->updateDecelChange = _amount;



	return ActionStatus::ACTION_SUCCESS;
}


/*
* Accel will change the driver's car acceleation axis
*/
ActionStatus AIActor_Driver::Turn(float _amount)
{

	driverCar->updateSteerChange = _amount;



	return ActionStatus::ACTION_SUCCESS;

}

void AIActor_Driver::RefreshActionBB()
{
	EditBBValue("SteerAxis", driverCar->inputSteerAxis);
	EditBBValue("AccelAxis", driverCar->inputAccelAxis);
}



Car& AIActor_Driver::GetDriverCar()
{
	return *driverCar;
}



