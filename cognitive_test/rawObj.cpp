#include "stdafx.h"
#include "rawObj.h"
#include <string>
#include <sstream>


rawObj::rawObj(const std::string& paramsAsStr)
{
	std::stringstream paramStrStream(paramsAsStr);
	std::string buf;
	getline(paramStrStream, buf, '\t');

	timestamp = stoi(buf);
	getline(paramStrStream, buf, '\t');

	if (buf == "Pedestrian") {
		type = trackObjType::pedestrian;
	} else if (buf == "Vehicle") {
		type = trackObjType::vehicle;
	} else if (buf == "TrafficLight") {
		type = trackObjType::trafficLight;
	}
	
	getline(paramStrStream, buf, '\t');
	float posXbuf = stof(buf);
	paramStrStream >> buf;
	float posYbuf = stof(buf);
//	getline(paramStrStream, buf, '\t');
	pos = Eigen::Vector2f(posXbuf, posYbuf);
}

const bool rawObj::operator==(const rawObj a) {
	if ((pos == a.pos) && (timestamp == a.timestamp) && (type == a.type)) {
		return true;
	}
	return false;
}


rawObj::~rawObj()
{
}

std::string rawObj::getString() {
    std::string typeStr;
    switch (type) {
    case trackObjType::pedestrian:
        typeStr = "Pedestrian";
        break;
    case trackObjType::vehicle:
        typeStr = "Vehicle";
        break;
    case trackObjType::trafficLight:
        typeStr = "TrafficLight";
        break;
    }

    return std::to_string(timestamp) + '\t' +
        typeStr + '\t' +
        std::to_string(pos.x()) + '\t' +
        std::to_string(pos.y()) + '\t' +
        std::to_string(ID) + '\t' +
        std::to_string(course) + '\t' +
        std::to_string(velocity);
}

