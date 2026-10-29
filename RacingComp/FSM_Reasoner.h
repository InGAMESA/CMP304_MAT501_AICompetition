#pragma once
#include "../../AI_Framework/framework.h"
class FSM_Reasoner :
    public AIReasonerBase
{
public:

    //States that the FSM can be in
    enum class DriverState { Accelerating, Centering };

    ~FSM_Reasoner();

    // AIReasonerBase overrides
    bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
    void Think() override;
    void SetStartingState(DriverState _state, std::string _stateName);
    // Tracking
    DriverState currentState;

private:
};

