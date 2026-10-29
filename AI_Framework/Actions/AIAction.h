#ifndef AI_ACTION_H
#define AI_ACTION_H


/*
CMP304/MAT501 AI Framework (2025)
*/

#pragma once

#include "../AIBase/AIActionBase.h"
class AIAction :
    public AIActionBase
{
public:
    bool Init(std::string _id, std::function<ActionStatus(AIBrainBlackboardBase&)> _action) override;
    void SetCost(float _cost) { cost = _cost; };
    float cost = 0;
};

#endif // !AI_ACTION_H