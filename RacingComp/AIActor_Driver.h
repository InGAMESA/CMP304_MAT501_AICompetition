#pragma once

#include "../AI_Framework/framework.h"
#include <SFML/Graphics.hpp>
#include "GOAPNode.h"

class GameController;


/*
* Car contains the data assoicated with the racing cars and their statu
*/
struct Car {
    float z=0;// world position forward
    float x=0; // world positon track left/right position
    float speed=0; // speed
    sf::Texture* texture = nullptr;
    bool isPlayer = false;
    int collisionCount = 0;;// total collisions in the race so far
    int lastCollisionSegment = 0;;// the segment the car last collided at
    int steerZone = 0; // the index ID of the current steer zone, used to determine display sprite
    bool isActiveCollision = false; // if the car is currently colliding with an obstacle
    bool isActiveRumble = false; // if the car is currently positioned on the rumble strip (track edge)
    float inputSteerAxis = 0; // steer direction -1 (left) to 1 (right)
    float inputAccelAxis = 0; //  accel amount -ve = deceleration
    bool isSkidding = false; // if the car is curretnly skidding
    int controllerMethod = 0;; // the enum class ID of the GameController::AIControllerType
    bool isActive = false; // if the car is currently racing. will be false when completed track
    float finishTime = 0; // the car's track finsih time. will be 0 when not completed track. 
    float updateSteerChange = 0; // the amount of steer change to be applied this update
    float updateAccelChange = 0; // the amount of accel change to be applied this update
    float updateDecelChange = 0; // the amount of devel change to be applied this update

};


/*
AIActor_Driver is an AIActorBase that represents the entities driving the cars
*/
class AIActor_Driver :
    public AIActorBase
{

public:
    AIActor_Driver();

    // Turn State provides the state for the GOAP process
    enum TurnState { Waiting, Planning, Executing, GoalReached };

    // AIActorBase overrides
    bool Init(std::string _id, AIConstructorBase& _constructor) override;
    void Update(float _dt) override;
    void PreTransition() override;

    void SetDriverCar(Car& _car);
    Car& GetDriverCar();
    void SetGameContext(GameController& _gameController);

    // Action Function
    ActionStatus Accel(float _amount);
    ActionStatus Decel(float _amount);
    ActionStatus Turn(float _amount);


    //GameStateKeys lists the blackboard keys related to the GOAP world state
    const std::vector<std::string> GameStateKeys = { "TestKey" };

    //StartingState gives the starting state of the GameStateKeys at initialisation
    const std::vector<bool> StartingState = { false };

    //GOAP Methods

    void ClearAtLocations();
    void InitWorldState();
    void SetPlanningState();
    WorldState GetBBWorldState();
    void SetBBWorldState(WorldState _ws);
    AIBrainBlackboard& GetPlanningBlackboard();
    void UpdateTurnStatus(TurnState _turnState);
    void RefreshActionBB();
    bool isPlanning = true;

    float updateTurnChange = 0;
    float updateAccelChange = 0;
    float updateDecelChange = 0;

private:
    // bounds and limits for the Actions
    const float maxAccelSpeed = 120.f;
    const float maxAccelChange = 30.f;
    const float maxDecelSpeed = 250.f;
    const float maxDecelChange = 40.f;
    const float maxTurnSpeed = 0.5f;


    const float turnBound = 1;

    // the car currently being driven
    Car* driverCar = nullptr;


    // reference to the Game Controller
    GameController* gameController = nullptr;




};

