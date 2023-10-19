//
// Created by zcl on 23-10-18.
//
#include "ViconSDK.h"
#include <iostream>
using namespace ViconDataStreamSDK::CPP;
int main(){
    ViconSDK vicon;
    vicon.myClient.GetFrame();
    auto t = vicon.getSegHomoTransMatrix("test", "test");
    std::cout << t << endl;
    auto sub = vicon.getSubjectTrans(0);
    cout << sub.markers[0].name << endl;
    return 0;
}