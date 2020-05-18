#include "stdafx.h"
#include "LPFf.h"

LPFf::LPFf(float firstVal, float T)
{
	T = 0;
	curVal = firstVal;
}

LPFf::~LPFf()
{
}

float LPFf::next(float newVal, float timeElapsed) {
	curVal = newVal * (timeElapsed / (T + timeElapsed)) + curVal * (T / (T + timeElapsed));
	return  curVal;
}
