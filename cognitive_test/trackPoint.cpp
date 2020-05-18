#include "stdafx.h"
#include "trackPoint.h"


trackPoint::trackPoint(Eigen::Vector2f posVal, Eigen::Vector2f velocityVal, Eigen::Vector2f accVal,
    float courseVal, unsigned int timestampVal)
{
	pos = posVal;
	velocity = velocityVal;
    acc = accVal;
	course = courseVal;
	timestamp = timestampVal;
}


trackPoint::~trackPoint()
{
}
