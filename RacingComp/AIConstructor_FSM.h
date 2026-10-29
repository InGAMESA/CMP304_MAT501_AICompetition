#pragma once
#include "../AI_Framework/framework.h"

class AIConstructor_FSM :
    public AIConstructorBase
{
public:
	bool Init() override;
	void DefineActions() override;
	void DefineConsiderations() override;
	void DefineOptions() override;


private:
};

