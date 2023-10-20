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
	void ViconSubAndSegShow();
    Eigen::Matrix<double,6,1> getSegHomoTransMatrix(std::string subjectName, std::string segmentName);
	captureTree::marker getOneMarkerTrans(uint subjectIndex, uint markerCount);
    captureTree::segment getSegmentTrans(uint subjectIndex, uint segmentCount);
    captureTree::subject getSubjectTrans(uint subjectIndex);
    captureTree::captureTree getCaptureTree();
public:
	vector<double> globalTransAndRotMatrix;
    ViconDataStreamSDK::CPP::Client myClient;

private:

	Output_GetSubjectCount OutputSubCount_;
	Output_GetSegmentCount OutputSegCount_;
	Output_GetMarkerCount OutputMarkCount_;

    captureTree::captureTree captureTree_;




};

#endif // !VICONSDK_H
