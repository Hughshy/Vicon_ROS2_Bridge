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
	
	/** 设置客户端缓存frames的buffer长度 */
	//myClient.SetBufferSize(1);

	/** 设置数据流模式：服务器推送，客户端拉取等*/
	myClient.SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPull);
	myClient.EnableMarkerData();
	//myClient.EnableDeviceData();
	/** 获取和设置物体以及刚体名称 */
	sleep(1);
	ViconSubAndSegGet();
}

void ViconSDK::ViconSubAndSegGet()
{
	myClient.GetFrame();
	OutputSubCount = myClient.GetSubjectCount();
	cout << "********************Subjects and Markers information*********************" << endl;
    cout <<"_______subject_______\n";
    cout << "the number of Subject is " << OutputSubCount.SubjectCount << endl;

//TODO:GetSubjectName
	for (unsigned int SubCount = 0; SubCount < OutputSubCount.SubjectCount; SubCount++)
	{
		OutputSubName = myClient.GetSubjectName(SubCount);
		cout << "NO: " << SubCount << " Subject name is :" << OutputSubName.SubjectName << endl;

		cout << "    _______segment_______\n";
		OutputSegCount = myClient.GetSegmentCount(OutputSubName.SubjectName);
		cout << "    the number of " << OutputSubName.SubjectName << " Segment is :" << OutputSegCount.SegmentCount << endl;
		for (unsigned int SegCount = 0; SegCount < OutputSegCount.SegmentCount; SegCount++)
		{
			OutputSegName = myClient.GetSegmentName(OutputSubName.SubjectName, SegCount);
			//cout << SegCount << " Segment name is :" << OutputSegName.SegmentName << endl;
			cout << "    NO: " << SegCount << " Segment name is :" << OutputSegName.SegmentName << endl;
		}
		cout << "       _______marker_______\n";
		OutputMarkCount = myClient.GetMarkerCount(OutputSubName.SubjectName);
		cout << "       the number of " << OutputSubName.SubjectName << " marker is :" << OutputMarkCount.MarkerCount << endl;
		for (unsigned int MarkerCount = 0; MarkerCount < OutputMarkCount.MarkerCount; MarkerCount++)
		{
			OutputMarkName = myClient.GetMarkerName(OutputSubName.SubjectName, MarkerCount);
			//cout << SegCount << " Segment name is :" << OutputSegName.SegmentName << endl;
			cout << "       NO: " << MarkerCount << " marker name is :" << OutputMarkName.MarkerName << endl;
		}
		//cout << "-----------------------------------";
		cout << endl;
	}
	cout << "***********************************************************************\n" << endl;
}

Eigen::Matrix4d ViconSDK::getSegHomoTransMatrix(std::string subjectName,std::string segmentName){
    Eigen::Matrix4d homoTransMatrix = Eigen::Matrix4d::Identity();
    Output_GetSegmentGlobalTranslation globalTrans = myClient.GetSegmentGlobalTranslation(subjectName, segmentName);
    Output_GetSegmentGlobalRotationMatrix globalRotMatix = myClient.GetSegmentGlobalRotationMatrix(subjectName, segmentName);
    Eigen::Map<Eigen::Matrix3d> rot(globalRotMatix.Rotation);
    Eigen::Map<Eigen::Vector3d> pos(globalTrans.Translation);
    homoTransMatrix.block<3,3>(0,0) = rot;
    homoTransMatrix.block<3,1>(0,3) = pos;

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









