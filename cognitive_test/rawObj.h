//#pragma once

#include <vector>

#include "params.h"
#include "Eigen-3.3.7/Eigen/Dense"

class rawObj
{
public:
    trackObjType type;
    Eigen::Vector2f pos;
    unsigned int timestamp;
    int ID = -1;
    
    float velocity = 0,
        course = 0;

    const bool operator==(const rawObj a);

	rawObj(std::string paramsAsStr);
	rawObj() {};
	~rawObj();

    std::string rawObj::getString();
};
