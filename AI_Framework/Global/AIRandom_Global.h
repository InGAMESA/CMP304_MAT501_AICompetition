#pragma once

#include <stdlib.h>
#include <time.h>
#include <random>

/*
CMP304/MAT501 AI Framework (2025)
*/


class AIRandom_Global
{
public:

	AIRandom_Global(const AIRandom_Global& obj) = delete;

	static AIRandom_Global* getInstance();

	double GetRandom01();
	int GetRandomInt(int _range);
	double GetRandomFloat(float _range);
	int GetRandomInt(int _rangeMin, int _rangeMax);
	float GetRandomFloat(float _rangeMin, float _rangeMax);


private:

	static AIRandom_Global* instancePointer;
	AIRandom_Global() { srand((unsigned int) (time(0))); }

	std::mt19937 rengine{ std::random_device{}() };
	std::uniform_real_distribution<double> rdist01{ 0.0, 1.0 };

};


