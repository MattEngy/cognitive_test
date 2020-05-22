#pragma once

#include <vector>
#include <list>

//#define MERGE_THRESHOLD 0.20f
//#define MERGE_THRESHOLD_RAW 0.5f
#define PEDESTRIAN_MERGE_THRESHOLD 0.2f
#define PEDESTRIAN_MERGE_THRESHOLD_RAW 0.5f
#define VEHICLE_MERGE_THRESHOLD 0.5f
#define VEHICLE_MERGE_THRESHOLD_RAW 1.0f
#define TRAFFICLIGHT_MERGE_THRESHOLD 0.5f
#define TRAFFICLIGHT_MERGE_THRESHOLD_RAW 1.0f
//#define UPDATE_PERIOD 0.025f //approx. 25ms between frames
#define OBJECT_MISSED_SEC_MAX 2.0f
#define SIMILARITY_POS_THRESHOLD 0.005f
#define MSEC_TO_SEC 0.001f
#define VELOCITY_LPF_T 0.12f
#define ACC_LPF_T 0.00f

class trackObjType {
public:
    enum type { pedestrian, vehicle, trafficLight };
    trackObjType() = default;
    trackObjType(type typeVal) : _type(typeVal) { }
    operator type() const { return _type; }
    static float getThreshold(trackObjType type, bool isRaw) {
        if (!isRaw) {
            switch (type._type) {
            case pedestrian:
                return PEDESTRIAN_MERGE_THRESHOLD;
            case vehicle:
                return VEHICLE_MERGE_THRESHOLD;
            case trafficLight:
                return TRAFFICLIGHT_MERGE_THRESHOLD;
            }
        } else {
            switch (type._type) {
            case pedestrian:
                return PEDESTRIAN_MERGE_THRESHOLD_RAW;
            case vehicle:
                return VEHICLE_MERGE_THRESHOLD_RAW;
            case trafficLight:
                return TRAFFICLIGHT_MERGE_THRESHOLD_RAW;
            }
        }
    }
private:
    type _type;
};