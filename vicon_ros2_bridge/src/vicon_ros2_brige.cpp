#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "ViconCaptureTree.h"
#include "ViconSDK.h"
#include "vicon_msgs/msg/vicon_capture_tree.hpp"

class Vicon_ROS2_Bridge : public rclcpp::Node
{
public:
    Vicon_ROS2_Bridge() : Node("Vicon_ROS2_Bridge"){

        vicon_capture_tree_pub_ = this->create_publisher<vicon_msgs::msg::ViconCaptureTree>("vicon_capture_tree", 10);
        timer_ = this->create_wall_timer(
            5ms, std::bind(&Vicon_ROS2_Bridge::timer_callback, this));
    };

private:
    vicon_msgs::msg::ViconCaptureTree vicon_capture_tree_msg_;
    rclcpp::Publisher<vicon_msgs::msg::ViconCaptureTree>::SharedPtr vicon_capture_tree_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    ViconSDK vicon_sdk_;

    void timer_callback(){
        vicon_sdk_.myClient.GetFrame();
        auto captureTree = vicon_sdk_.getCaptureTree();
        vicon_capture_tree_msg_ = captureTree2msg(captureTree);
        vicon_capture_tree_pub_->publish(vicon_capture_tree_msg_);
    };

    //将ViconCaptureTree转换为vicon_msgs::msg::ViconCaptureTree的函数
    vicon_msgs::msg::ViconCaptureTree captureTree2msg(captureTree::captureTree captureTree){
        vicon_msgs::msg::ViconCaptureTree vicon_capture_tree_msg;
        vicon_capture_tree_msg.frame_number = captureTree.frameNumber;
        vicon_capture_tree_msg.latency = captureTree.latency;
        vicon_capture_tree_msg.frame_rate = captureTree.frameRate;
        vicon_capture_tree_msg.subjects.resize(captureTree.subjects.size());
        vicon_capture_tree_msg.subjects = subjects2msg(captureTree.subjects);
        return vicon_capture_tree_msg;
    }

    std::vector<vicon_msgs::msg::Subject> subjects2msg(std::vector<captureTree::subject> subjects){
        std::vector<vicon_msgs::msg::Subject> subjects_msg;
        subjects_msg.resize(subjects.size());
        for(int i = 0; i < subjects.size(); i++){
            subjects_msg[i].name = subjects[i].name;
            subjects_msg[i].index = subjects[i].index;
            subjects_msg[i].segments.resize(subjects[i].segments.size());
            subjects_msg[i].segments = segments2msg(subjects[i].segments);
            subjects_msg[i].markers.resize(subjects[i].markers.size());
            subjects_msg[i].markers = markers2msg(subjects[i].markers);
        }
        return subjects_msg;
    }

    std::vector<vicon_msgs::msg::Segment> segments2msg(std::vector<captureTree::segment> segments){
        std::vector<vicon_msgs::msg::Segment> segments_msg;
        segments_msg.resize(segments.size());
        for(int i = 0; i < segments.size(); i++){
            segments_msg[i].name = segments[i].name;
            segments_msg[i].index = segments[i].index;
            for (int j = 0; j < 6; j++){
                segments_msg[i].global_trans[j] = segments[i].globalTrans[j];
            }
        } 
        return segments_msg;
    }

    std::vector<vicon_msgs::msg::Marker> markers2msg(std::vector<captureTree::marker> markers){
        std::vector<vicon_msgs::msg::Marker> markers_msg;
        markers_msg.resize(markers.size());
        for(int i = 0; i < markers.size(); i++){
            markers_msg[i].name = markers[i].name;
            markers_msg[i].index = markers[i].index;
            markers_msg[i].x = markers[i].x;
            markers_msg[i].y = markers[i].y;
            markers_msg[i].z = markers[i].z;
        }
        return markers_msg;
    }

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Vicon_ROS2_Bridge>());
    rclcpp::shutdown();
    return 0;
}