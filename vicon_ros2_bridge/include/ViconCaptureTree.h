//
// Created by zcl on 23-10-19.
//

#ifndef VICON_ROS2_BRIDGE_VICONCAPTURETREE_H
#define VICON_ROS2_BRIDGE_VICONCAPTURETREE_H
#include <string>
#include <vector>
#include <Eigen/Dense>
namespace captureTree {
    typedef struct {
        std::string name;
        uint index;
        double x;
        double y;
        double z;
    } marker;

    typedef struct {
        std::string name;
        uint index;
        Eigen::Matrix<double,6,1> globalTrans;
    } segment;

    typedef struct {
        std::string name;
        uint index;
        std::vector<segment> segments;
        std::vector<marker> markers;
    } subject;

    typedef struct {
        uint32_t frameNumber;
        double latency;
        uint frameRate;
        std::vector<subject> subjects;
    } captureTree;

}


#endif //VICON_ROS2_BRIDGE_VICONCAPTURETREE_H
