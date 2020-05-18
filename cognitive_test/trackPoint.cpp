#include "stdafx.h"
#include "trackPoint.h"


trackPoint::trackPoint(const Eigen::Vector2f& posVal, const Eigen::Vector2f& velocityVal, const Eigen::Vector2f& accVal,
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
