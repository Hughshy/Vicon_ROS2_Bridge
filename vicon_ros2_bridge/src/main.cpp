//
// Created by zcl on 23-10-18.
//
#include "ViconSDK.h"
#include <iostream>
using namespace ViconDataStreamSDK::CPP;
int main(){
    ViconSDK vicon;


    vicon.myClient.GetFrame();
    cout << vicon.myClient.GetFrameRate().FrameRateHz;

//    auto t = vicon.getSegHomoTransMatrix("test", "test");
//    std::cout << t << endl;
    auto sub = vicon.getCaptureTree();
    cout << sub.subjects[0].markers[0].x << endl;
    return 0;
}