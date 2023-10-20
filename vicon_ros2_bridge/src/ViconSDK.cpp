#include "ViconSDK.h"

ViconSDK::ViconSDK()
{
//TODO:设置ViconSDK的IP地址
	ViconDataStreamSDK::CPP::Output_Connect output_connect = myClient.Connect("192.168.10.1");
	switch (output_connect.Result)
	{
	case 2:
		cout << "Vicon Server connect successfully!" << endl;
		break;
	case 3:
		cout << "The HostName parameter passed to Connect() is invalid" << endl;
		break;
	case 5:
		cout << "Vicon SDK was called whilst already connected to a DataStream" << endl;
		break;
	case 6:
		cout << "Vicon Client could not establish a connection to the DataStream server" << endl;
		break;
	default:
		break;
	}
	/** Vicon轻量化数据流运动学数据单元使能，该功能用在连接服务器以后和尝试读取数据之前 */
	ViconDataStreamSDK::CPP::Output_EnableSegmentData output_enableSegmentData = myClient.EnableSegmentData();
	switch (output_enableSegmentData.Result)
	{
	case 2:
		cout << "Segment lightweight data enable successfully!" << endl;
		break;
	case 9:
		cout << "Segment lightweight data enable unsuccessfully, because do not have a connection to the DataStream server!" << endl;
		break;
	default:
		break;
	}

	/** 设置数据流模式：服务器推送，客户端拉取等*/
	myClient.SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPull);
	myClient.EnableMarkerData();

	sleep(1);
    ViconSubAndSegShow();
}

void ViconSDK::ViconSubAndSegShow()
{
	myClient.GetFrame();
	OutputSubCount_ = myClient.GetSubjectCount();
	cout << "********************Subjects and Markers information*********************" << endl;
    cout <<"_______subject_______\n";
    cout << "the number of Subject is " << OutputSubCount_.SubjectCount << endl;

//TODO:GetSubjectName
	for (unsigned int SubCount = 0; SubCount < OutputSubCount_.SubjectCount; SubCount++)
	{
		auto subName = myClient.GetSubjectName(SubCount);
		cout << "NO: " << SubCount << " Subject name is :" << subName.SubjectName << endl;

		cout << "    _______segment_______\n";
		OutputSegCount_ = myClient.GetSegmentCount(subName.SubjectName);
		cout << "    the number of " << subName.SubjectName << " Segment is :" << OutputSegCount_.SegmentCount << endl;
		for (unsigned int SegCount = 0; SegCount < OutputSegCount_.SegmentCount; SegCount++)
		{
			auto segName = myClient.GetSegmentName(subName.SubjectName, SegCount);
			//cout << SegCount << " Segment name is :" << OutputSegName.SegmentName << endl;
			cout << "    NO: " << SegCount << " Segment name is :" << segName.SegmentName << endl;
		}
		cout << "    _______marker_______\n";
		OutputMarkCount_ = myClient.GetMarkerCount(subName.SubjectName);
		cout << "    the number of " << subName.SubjectName << " marker is :" << OutputMarkCount_.MarkerCount << endl;
		for (unsigned int MarkerCount = 0; MarkerCount < OutputMarkCount_.MarkerCount; MarkerCount++)
		{
			auto markName = myClient.GetMarkerName(subName.SubjectName, MarkerCount);
			//cout << SegCount << " Segment name is :" << OutputSegName.SegmentName << endl;
			cout << "    NO: " << MarkerCount << " marker name is :" << markName.MarkerName << endl;
		}
		//cout << "-----------------------------------";
		cout << endl;
	}
	cout << "***********************************************************************\n" << endl;
}

Eigen::Matrix<double, 6,1> ViconSDK::getSegHomoTransMatrix(std::string subjectName,std::string segmentName){
    Eigen::Matrix<double,6,1> homoTransMatrix;
    Output_GetSegmentGlobalTranslation globalTrans = myClient.GetSegmentGlobalTranslation(subjectName, segmentName);
    Output_GetSegmentGlobalRotationEulerXYZ globalRotEular = myClient.GetSegmentGlobalRotationEulerXYZ(subjectName, segmentName);
    Eigen::Map<Eigen::Vector3d> rot(globalRotEular.Rotation);
    Eigen::Map<Eigen::Vector3d> pos(globalTrans.Translation);
    homoTransMatrix.block<3,1>(0,0) = rot;
    homoTransMatrix.block<3,1>(3,0) = pos;

    return homoTransMatrix;
}


captureTree::marker ViconSDK::getOneMarkerTrans(uint subjectIndex, uint markerCount){
	captureTree::marker marker;
    std::string subjectName = myClient.GetSubjectName(subjectIndex).SubjectName;
    marker.name = myClient.GetMarkerName(subjectName, markerCount).MarkerName;
    marker.index = markerCount;
    marker.x = myClient.GetMarkerGlobalTranslation(subjectName, marker.name).Translation[0];
    marker.y = myClient.GetMarkerGlobalTranslation(subjectName, marker.name).Translation[1];
    marker.z = myClient.GetMarkerGlobalTranslation(subjectName, marker.name).Translation[2];
    return marker;
}

captureTree::segment ViconSDK::getSegmentTrans(uint subjectIndex, uint segmentCount){
    captureTree::segment segment;
    std::string subjectName = myClient.GetSubjectName(subjectIndex).SubjectName;
    segment.name = myClient.GetSegmentName(subjectName, segmentCount).SegmentName;
    segment.index = segmentCount;
    segment.globalTrans = getSegHomoTransMatrix(subjectName, segment.name);
    return segment;
}

captureTree::subject ViconSDK::getSubjectTrans(uint subjectIndex){
    captureTree::subject subject;
    subject.name = myClient.GetSubjectName(subjectIndex).SubjectName;
    uint segmentsNum = myClient.GetSegmentCount(subject.name).SegmentCount;
    for(int i = 0; i < segmentsNum; i++){
        auto segmentTrans = getSegmentTrans(subjectIndex, i);
        subject.segments.push_back(segmentTrans);
    }
    uint markersNum = myClient.GetMarkerCount(subject.name).MarkerCount;
    for(int i = 0; i < markersNum; i++){
        auto markerTrans = getOneMarkerTrans(subjectIndex, i);
        subject.markers.push_back(markerTrans);
    }
    return subject;
}

captureTree::captureTree ViconSDK::getCaptureTree(){
    captureTree::captureTree captureTree;
    uint subjectNum = myClient.GetSubjectCount().SubjectCount;
    captureTree.frameNumber = myClient.GetFrameNumber().FrameNumber;
    captureTree.latency = myClient.GetLatencyTotal().Total;
    captureTree.frameRate = myClient.GetFrameRate().FrameRateHz;
    for(int i = 0; i < subjectNum; i++){
        auto subjectTrans = getSubjectTrans(i);
        captureTree.subjects.push_back(subjectTrans);
    }

    return captureTree;
}









