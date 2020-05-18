// cognitive_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <memory>

#include "params.h"
#include "uniqueObjDB.h"
#include "Eigen-3.3.7/Eigen/Dense"

using namespace std;
using namespace Eigen;

void filterSimilarDetections(list<rawObj>& detectionsList);
unique_ptr<rawObj[]> formArray(list<rawObj>& detectionsList);

int main() {
	ifstream fileInp;
	string trackObjString;
    string inpFileName = "trm.169.049.det.tsv";
	fileInp.open(inpFileName);

    ofstream fileOutp;
    fileOutp.open(inpFileName + ".result", ios::trunc);
    
	getline(fileInp, trackObjString);
    fileOutp << "msec\ttype\tx_gm\ty_gm\tid\tyaw\tv" << endl;

	getline(fileInp, trackObjString);
	rawObj ro(trackObjString);
	list<rawObj> curRawDetections = { ro };
	unsigned int curTimestamp = ro.timestamp;

	list<uniqueObj> uniqueObjects = { uniqueObj(0 , ro) };
	unsigned int nextID = 1;
    list<rawObj> outFileBuf;
    getline(fileInp, trackObjString);
	while (!fileInp.eof()) {
        ro = rawObj(trackObjString);
        getline(fileInp, trackObjString);	
		if ((ro.timestamp == curTimestamp) && (!fileInp.eof())) {
			curRawDetections.push_back(ro);
		} else { //switching to the next tick
            if (fileInp.eof()) {
                curRawDetections.push_back(ro);
            }
			list<rawObj>::iterator curRawDetectionsIt;
			list<uniqueObj>::iterator uniqueObjIt;
            //NOTE: this function uses ID field of objects in curRawDetections
            //to store correspinding array index (for optimization props)
            unsigned int curRawDetectionsCnt = curRawDetections.size();
            unique_ptr<rawObj []> curObjArrayBuf = formArray(curRawDetections);
			//process tick
			if (curRawDetections.size() >= 2) {
				filterSimilarDetections(curRawDetections);
			}
            for (uniqueObjIt = uniqueObjects.begin(); uniqueObjIt != uniqueObjects.end(); uniqueObjIt++) {
                float timeElapsed = (curTimestamp - uniqueObjIt->track.back().timestamp) * MSEC_TO_SEC;
				float minError = MERGE_THRESHOLD * 1000;
				list<rawObj>::iterator minErrorObj;
				bool matchFound = false;
                for (curRawDetectionsIt = curRawDetections.begin(); curRawDetectionsIt != curRawDetections.end(); curRawDetectionsIt++) {
					float curError;
					bool pointMatches = uniqueObjIt->checkPointMatch(*curRawDetectionsIt, timeElapsed, &curError);
					matchFound |= pointMatches;
					if (pointMatches && (curError < minError)) {
						minErrorObj = curRawDetectionsIt;
						minError = curError;
					}
				}
				if (matchFound) {
                    uniqueObjIt->addPoint(*minErrorObj, timeElapsed);
                    curObjArrayBuf[minErrorObj->ID].ID = uniqueObjIt->id;
                    curObjArrayBuf[minErrorObj->ID].course = uniqueObjIt->track.back().course;
                    curObjArrayBuf[minErrorObj->ID].velocity = uniqueObjIt->track.back().velocity.norm();
                    curRawDetections.erase(minErrorObj);
				}
			}
            //create new uniqueObjects for point that didnt matched any existing uniqueObjects
            for (curRawDetectionsIt = curRawDetections.begin(); curRawDetectionsIt != curRawDetections.end(); curRawDetectionsIt++) {
                curObjArrayBuf[curRawDetectionsIt->ID].ID = nextID;
                uniqueObjects.push_back(uniqueObj(nextID++, *curRawDetectionsIt));
            }

            //transfer curObjArrayBuf to outFileBuf
            for (unsigned int i = 0; i < curRawDetectionsCnt; ++i) {
                outFileBuf.push_back(curObjArrayBuf[i]);
            }

            //switch to the next tick
			curTimestamp = ro.timestamp;
			curRawDetections.clear();
			curRawDetections.push_back(ro);
//            delete curObjArrayBuf;
		}
	}
    uniqueObjects.remove_if([](const uniqueObj & u) { return u.track.size() < (2 / 0.03); });
    list<rawObj>::iterator outFileBufIt;
    // setting id of record to -1 if this id isnt present in uniqueObjects after filtering;
    // migh be better to switch to maps
    list<uniqueObj>::iterator uniqueObjIt;
    for (outFileBufIt = outFileBuf.begin(); outFileBufIt != outFileBuf.end(); outFileBufIt++) {
        bool isPresent = false;
        for (uniqueObjIt = uniqueObjects.begin(); (!isPresent) && (uniqueObjIt != uniqueObjects.end()); uniqueObjIt++) {
            isPresent |= outFileBufIt->ID == uniqueObjIt->id;
        }
        if (!isPresent) {
            outFileBufIt->ID = -1;
        }
    }
    //write file buffer to output file
    for (outFileBufIt = outFileBuf.begin(); outFileBufIt != outFileBuf.end(); outFileBufIt++) {
        fileOutp << outFileBufIt->getString() << endl;
    }
    fileInp.close();
    fileOutp.close();
	cout << nextID;

//	list<uniqueObject>::iterator uniqueObjIt = uniqueObjects.begin();
//	while (uniqueObjIt != uniqueObjects.end()) { /*for each detection check if it matches any track from unique objects list*/
//		if (uniqueObjIt->track.size() < (5 / 0.003)) {
//			uniqueObjects.erase(uniqueObjIt.);
//		}
//	}
	int dummy;
	cin >> dummy;
    return 0;
}

void filterSimilarDetections(list<rawObj>& detectionsList) {
	list<rawObj>::const_iterator it1;
	list<rawObj>::const_iterator it2;
	detectionsList.push_back(rawObj());
	for (it1 = detectionsList.begin(); it1 != prev(detectionsList.end()); ++it1) {
		for (it2 = next(it1); it2 != prev(detectionsList.end());) {
			if ((it1->type == it2->type) && ((it1->pos - it2->pos).norm() < SIMILARITY_POS_THRESHOLD)) {
				it2 = detectionsList.erase(it2);
			}
			else {
				++it2;
			}
		}
	}
	detectionsList.pop_back();
}

unique_ptr<rawObj []> formArray(list<rawObj>& detectionsList) {
    unique_ptr<rawObj []> ptrBuf(new rawObj[detectionsList.size()]);
    list<rawObj>::iterator it = detectionsList.begin();
    for (unsigned int i = 0; i < detectionsList.size(); ++i) {
        ptrBuf[i] = *it;
        it->ID = i;
        it++;
    }
    return ptrBuf;
}
