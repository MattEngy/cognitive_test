#pragma once
class LPFf
{
protected:

public:
	float T;
	float curVal;

	LPFf(float firstVal, float T);
	~LPFf();

	float next(float newVal, float timeElapsed);
};

