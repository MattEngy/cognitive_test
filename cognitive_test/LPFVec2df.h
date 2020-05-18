#pragma once

#include "Eigen-3.3.7/Eigen/Dense"

class LPFVec2df
{

public:
	Eigen::Vector2f curVal;
	float T;

	LPFVec2df() {};
	LPFVec2df(Eigen::Vector2f firstVal, float T);
	~LPFVec2df();

	Eigen::Vector2f next(Eigen::Vector2f newVal, float timeElapsed);
};

