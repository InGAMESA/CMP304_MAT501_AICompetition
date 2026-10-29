#include "AIRandom_Global.h"
#include <stdlib.h>


AIRandom_Global* AIRandom_Global::instancePointer = nullptr;

AIRandom_Global* AIRandom_Global::getInstance()
{

	if (instancePointer == nullptr)
	{
		instancePointer = new AIRandom_Global();
	}

	return(instancePointer);
}



double AIRandom_Global::GetRandom01()
{
	return  rdist01(rengine);
}

int AIRandom_Global::GetRandomInt(int _range)
{

	if (_range <= 0)
		return 0;

	return rand() % _range;
}
double AIRandom_Global::GetRandomFloat(float _range)
{
	return GetRandom01() * _range;
}

int AIRandom_Global::GetRandomInt(int _rangeMin, int _rangeMax)
{
	int _range = _rangeMax - _rangeMin;

	return (GetRandom01() * _range) + _rangeMin;
}

float AIRandom_Global::GetRandomFloat(float _rangeMin, float _rangeMax)
{
	float _range = _rangeMax - _rangeMin;

	return (GetRandom01() * _range) + _rangeMin;
}