#include <list>
#include <cmath>

#include "stdafx.h"
#include "uniqueObj.h"
#include "params.h"

#define sqr(a) ((a)*(a))

uniqueObj::uniqueObj(const uint32_t newId, const rawObj& initialPoint)
{
	id = newId;
	type = initialPoint.type;
	trackPoint buf(initialPoint.pos, Eigen::Vector2f::Zero(), Eigen::Vector2f::Zero(), 0.0f, initialPoint.timestamp);
	track.push_back(buf);
	noDetectTicks = 0;
	velocityLPF = LPFVec2df(Eigen::Vector2f(0.0f, 0.0f), VELOCITY_LPF_T);
    accLPF = LPFVec2df(Eigen::Vector2f(0.0f, 0.0f), ACC_LPF_T);
}

uniqueObj::~uniqueObj()
{
}

/* bool uniqueObj::pointAddAttepmt(rawObj newPoint, const float timeElapsed) {
	Eigen::Vector2f predictedPos;
//	Eigen::Vector2f velocityCur;
	float megreThresholdBuf;
	if (track.size() >= 2) {
		velocityLPF.next((track.back().pos - prev(track.end(), 2)->pos) / timeElapsed, timeElapsed);

//		Eigen::Vector2f accCur = (velocityCur.curVal - velocityPrev) / timeElapsed;
		predictedPos = track.back().pos +
		               velocityLPF.curVal * (timeElapsed);// +
		               //accCur / 2 * sqr(timeElapsed);
		megreThresholdBuf = MERGE_THRESHOLD;
	}
	else {
		predictedPos = track.back().pos;
		megreThresholdBuf = MERGE_THRESHOLD_RAW;
	}

	if ((predictedPos - newPoint.pos).norm() < megreThresholdBuf) {
		if (track.size() >= 2) {
//			velocityPrev = velocityCur.curVal;;
		}
		trackPoint buf(newPoint.pos, velocityLPF.curVal, 0.0f, newPoint.timestapm);
		track.push_back(buf);
		return true;
	}
	else {
		return false;
	}
}*/

bool uniqueObj::checkPointMatch(const rawObj& newPoint, const float timeElapsed, float* error) const {
	Eigen::Vector2f predictedPos;
	//	Eigen::Vector2f velocityCur;
    LPFVec2df velocityLPFbuf = velocityLPF;
    LPFVec2df accLPFbuf = accLPF;
	float megreThresholdBuf;
	if (track.size() >= 2) {
        //velocityLPFbuf.next((track.back().pos - prev(track.end(), 2)->pos) / timeElapsed, timeElapsed);
        //accLPFbuf.next((track.back().velocity - prev(track.end(), 2)->velocity) / timeElapsed, timeElapsed);
		//		Eigen::Vector2f accCur = (velocityCur.curVal - velocityPrev) / timeElapsed;
        float prevTimeElapsed = (track.back().timestamp - prev(track.end(), 2)->timestamp) * MSEC_TO_SEC;
        predictedPos = track.back().pos +
            velocityLPF.curVal * (timeElapsed) +
            accLPFbuf.curVal * 0.5f * sqr(timeElapsed);
		megreThresholdBuf = MERGE_THRESHOLD;
	}
	else {
		predictedPos = track.back().pos;
		megreThresholdBuf = MERGE_THRESHOLD_RAW;
	}
	float predictionError = (predictedPos - newPoint.pos).norm();
    *error = predictionError;
	if ((predictionError < megreThresholdBuf) && (type == newPoint.type)) {
	    return true;
	}
	else {
		return false;
	}
}

void uniqueObj::addPoint(const rawObj& newPoint, const float timeElapsed) {
    Eigen::Vector2f curVelocity = (newPoint.pos - track.back().pos) / timeElapsed;
    Eigen::Vector2f curAcc = (curVelocity - track.back().velocity) / timeElapsed;
    velocityLPF.next(curVelocity, timeElapsed);
    accLPF.next(curAcc, timeElapsed);
    float courseBuf;
    float x = velocityLPF.curVal.x(),
          y = velocityLPF.curVal.y();
    const static float PI_2 = atan(1) * 2;
    if (velocityLPF.curVal.norm() > 0.01) {
        if (x >= y) {
            if (-x <= y) {
                courseBuf = atan(y / x) + PI_2 * 3;
            }
            else {
                courseBuf = atan(x / -y) + PI_2 * 2;
            }
        }
        else {
            if (-x <= y) {
                courseBuf = atan(x / -y);
            }
            else {
                courseBuf = atan(-y / -x) + PI_2;
            }
        }
    }
    else {
        courseBuf = 0.0f;
    }
    trackPoint buf(newPoint.pos, velocityLPF.curVal, accLPF.curVal, courseBuf, newPoint.timestamp);
    track.push_back(buf);
}
