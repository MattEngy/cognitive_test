#include "stdafx.h"
#include "LPFVec2df.h"

LPFVec2df::LPFVec2df(Eigen::Vector2f firstVal, float Tval)
{
	T = Tval;
	curVal = firstVal;
}


LPFVec2df::~LPFVec2df()
{
}

Eigen::Vector2f LPFVec2df::next(Eigen::Vector2f newVal, float timeElapsed) {
	curVal = newVal * (timeElapsed / (T + timeElapsed)) + curVal * (T / (T + timeElapsed));
	return  curVal;
}
