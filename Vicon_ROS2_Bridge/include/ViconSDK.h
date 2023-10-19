#ifndef VICONSDK_H
#define VICONSDK_H
#include "DataStreamClient.h"
#include "ViconCaptureTree.h"
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace ViconDataStreamSDK::CPP;
class ViconSDK
{
public:
	ViconSDK();
	void ViconSubAndSegGet();
    Eigen::Matrix4d getSegHomoTransMatrix(std::string subjectName, std::string segmentName);
	captureTree::marker getOneMarkerTrans(uint subjectIndex, uint markerCount);
    captureTree::segment getSegmentTrans(uint subjectIndex, uint segmentCount);
    captureTree::subject getSubjectTrans(uint subjectIndex);
public:
	vector<double> globalTransAndRotMatrix;
    ViconDataStreamSDK::CPP::Client myClient;

private:
	/** 创建客户端 */

	Output_GetSubjectCount OutputSubCount;
	Output_GetSubjectName OutputSubName;

	Output_GetSegmentCount OutputSegCount;
	Output_GetSegmentName OutputSegName;

	Output_GetMarkerCount OutputMarkCount;
	Output_GetMarkerName OutputMarkName;

	Output_GetSegmentGlobalTranslation OutputGlobalTrans;
	Output_GetSegmentGlobalRotationMatrix OutputGlobalRotMatix;

    Output_GetMarkerGlobalTranslation OutputMarkerGlobalTrans;

	unsigned int _baseGetPointsNum;
};

#endif // !VICONSDK_H
