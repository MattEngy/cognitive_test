#pragma once

#include "rawObj.h"
#include "Eigen-3.3.7/Eigen/Dense"
#include "LPFVec2df.h"
#include "trackPoint.h"

class uniqueObj {
public:
	int id;
	trackObjType type;
	std::list<trackPoint> track;
	LPFVec2df velocityLPF;
    LPFVec2df accLPF;
	unsigned int noDetectTicks;


	uniqueObj(const unsigned int newId, const rawObj& initialPoint);
	uniqueObj() {};
	~uniqueObj();

//	bool pointAddAttepmt(rawObj newPoint, const float timeElapsed);
    bool checkPointMatch(const rawObj& newPoint, const float timeElapsed, float* error) const;
	void addPoint(const rawObj& newPoint, const float timeElapsed);
};
