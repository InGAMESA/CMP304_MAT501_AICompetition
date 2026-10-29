#include "DT_Reasoner.h"

#include "AIConstructor_DT.h"

bool DT_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context) {
	actorBlackboard = &_context;
	reasonerID = _id;


	return true;
}


/*
Think is called on demand to choose from the possible Options that AI has.

The function should implement the logic of the AI method to choose one Option as the selectedOption.
*/
void DT_Reasoner::Think()
{



	//for all options, check if they can be Triggered, and select one to Fire
	for (const auto& option : options)
	{


		// get all Consideration for the Option
		std::vector<std::shared_ptr<AIConsiderationBase>> ruleList = option->GetOptionConsiderations();



		// DT Think() logic
		//  Check if all Considerations in the ruleList are TRUE
		//  Call Calculate() on each Consideration to get a boolean true/false return value if the condition is met
		//  Pass the blackboard to the Calculate function  e.g. Calculate(*actorBlackboard))
		//		
		// 
		//  If a rule's conditions are all met, then it may be selected



		bool conditionsMet = true;
		for (auto c : ruleList)
		{

			bool ruleResult = c->Calculate(*actorBlackboard);
	
			if (!ruleResult)
			{
				conditionsMet = false;
			}
		}

		if (conditionsMet)
		{
			// select this branch to explore
			selectedOption = option.get();

			break;
		}

	}




	// TASK TODO - By the end of the function,  'selectedOption' should be set to the option that is to be Fired
	// (the Firing will happen automatically later)



}
























/*
---- AI Framework Function Overrides  - ** Do Not Edit ** ----
*/

/*
SetOptions will create and initalise the Options and Considerations as defeined in the Constuctor
This includes creating and initalising any Sub Reasoners

*/


void DT_Reasoner::SetOptions(AIConstructorBase& _constructor)
{
	AIConstructor_DT* dtConstructor = static_cast<AIConstructor_DT*>(&_constructor);

	std::vector<std::string> _reasonerOptions = dtConstructor->GetOptionNamesForKey(reasonerID);

	std::vector<AIOptionDefinition> _options = _constructor.GetOptionVector(_reasonerOptions);

	for (int i = 0; i < _options.size(); i++)
	{
		// create a new Option, owned by this reasoner
		std::unique_ptr<AIOption> o = std::make_unique<AIOption>();


		//check if the Action is of type AIActionSubReasoner
		// if so, the Option should be constructed as a SubReasoner
		std::string on = typeid(*_constructor.actions[_options[i].actionName]).name();
		std::string::size_type n = on.find("AIActionSubReasoner");



		if (n != std::string::npos)
		{

			// if the action for the option is an AIActionSubReasoner, we need to create the SubReasoner, and add its options
			std::shared_ptr<AIActionSubReasoner> actionReasoner = std::make_shared<AIActionSubReasoner>();
			actionReasoner->Init(on, nullptr);


			DT_Reasoner* subReasoner = new DT_Reasoner();
			std::string _optionName = _options[i].optionName;

			subReasoner->Init(_optionName, *actorBlackboard);

			std::vector<std::string> _suboptions = dtConstructor->GetOptionNamesForKey(_optionName);

			// call the SetOptions function to add options associated with this option
			// will also crate and initalise any sub reasoner actions within them
			subReasoner->SetOptions(_constructor);

			actionReasoner->SetChildReasoner(*subReasoner);

			o->Init(_optionName, actionReasoner, _options[i].priority);

		}
		else
		{
			o->Init(_options[i].optionName, _constructor.actions[_options[i].actionName], _options[i].priority);

		}


		for (int j = 0; j < _options[i].considerationNames.size(); j++)
		{
			if (!_constructor.considerations.contains(_options[i].considerationNames[j]))
			{
				std::cout << "ERROR [AIReasonerBase] : No Consideration " << _options[i].considerationNames[j] << " found  - did you add the correct Consideration? " << std::endl;
				return;
			}

			// link the consideration created by the contructor to the Option
			o->AddOptionConsideration(_constructor.considerations[_options[i].considerationNames[j]]);
		}

		options.push_back(move(o));
	}

}