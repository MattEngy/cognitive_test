#pragma once

#include "params.h"
#include "Eigen-3.3.7/Eigen/Dense"

class trackPoint
{
public:
    Eigen::Vector2f pos,
                    velocity,
                    acc;
	float course;
	unsigned int timestamp;

	trackPoint(const Eigen::Vector2f& posVal, const Eigen::Vector2f& velocityVal, const Eigen::Vector2f& accVal,
        float courseVal, unsigned int timestamp);
	~trackPoint();
};

