#pragma once

#include <vector>
#include <list>

#define MERGE_THRESHOLD 0.20f
#define MERGE_THRESHOLD_RAW 0.5f
#define PEDESTRIAN_MERGE_THRESHOLD 0.5
#define VEHICLE_MERGE_THRESHOLD 0.5
#define TRAFFICLIGHT_MERGE_THRESHOLD 0.5
//#define UPDATE_PERIOD 0.025f //approx. 25ms between frames
#define OBJECT_MISSED_SEC_MAX 1.0f
#define SIMILARITY_POS_THRESHOLD 0.005
#define MSEC_TO_SEC 0.001f
#define VELOCITY_LPF_T 0.25
#define ACC_LPF_T 0.09

//TODO: chage obj type enum to class and make static method of getting threshold
enum trackObjType { pedestrian, vehicle, trafficLight };

