cmake_minimum_required(VERSION 3.5)
project(vicon_ros2_bridge)
set(CMAKE_CXX_FLAGS "-O3")

find_package(Eigen3 REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(vicon_msgs REQUIRED)
#判断vicon_msgs是否找到

if(NOT vicon_msgs_FOUND)
    message(FATAL_ERROR "vicon_msgs not found")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/third-party/ViconDataStreamSDK_Linux64
)

link_directories(
    ${CMAKE_SOURCE_DIR}/src/third-party/ViconDataStreamSDK_Linux64
)

set(EXTRA_LIBS libViconDataStreamSDK_CPP.so -pthread)
add_executable(vicon_test
    src/main.cpp src/ViconSDK.cpp
)

target_link_libraries(vicon_test
    ${EXTRA_LIBS}
)

add_executable(vicon_ros2_bridge
    src/vicon_ros2_bridge.cpp src/ViconSDK.cpp)
target_link_libraries(vicon_ros2_bridge
    ${EXTRA_LIBS})
ament_target_dependencies(vicon_ros2_bridge rclcpp vicon_msgs)



install(TARGETS
    vicon_test
    vicon_ros2_bridge
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()