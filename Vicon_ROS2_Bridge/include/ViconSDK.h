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
    std::vector<captureTree::subject> getCaptureTree();
public:
	vector<double> globalTransAndRotMatrix;
    ViconDataStreamSDK::CPP::Client myClient;

private:
	/** 创建客户端 */

	Output_GetSubjectCount OutputSubCount_;
	Output_GetSegmentCount OutputSegCount_;
	Output_GetMarkerCount OutputMarkCount_;

    std::vector<captureTree::subject> captureTree_;




};

#endif // !VICONSDK_H
