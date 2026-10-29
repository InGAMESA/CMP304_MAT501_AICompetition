#include "RBS_Reasoner.h"



RBS_Reasoner::~RBS_Reasoner()
{


}


bool RBS_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context) {
	actorBlackboard = &_context;
	reasonerID = _id;

	return true;
}



/*
Think is called on demand to choose from the possible Options that AI has.

The function should implement the logic of the AI method to choose one Option as the selectedOption.
*/
void RBS_Reasoner::Think()
{

	//for all options, check if they can be Triggered, and select one to Fire
	for (const auto& option : options)
	{
		// get all Consideration for the Option
		std::vector<std::shared_ptr<AIConsiderationBase>> ruleList = option->GetOptionConsiderations();

		
		// very simplistic RBS Reasoner - will simply select the first rule whose conditions are met
		// Note - there is no Arbiter currently implemented

		bool conditionsMet = true;

		// check all conditions
		for (auto r : ruleList)
		{
			if (!r->Calculate(*actorBlackboard))
				conditionsMet = false;
		}

		if (conditionsMet)
		{
			selectedOption = option.get();
			break;
		}

	}


	//  By the end of the function,  'selectedOption' should be set to the option that is to be Fired
	//  Use option.get() to obtain a copy of the option to set selectedOption pointer
	// (the Firing will happen automatically later)


}

