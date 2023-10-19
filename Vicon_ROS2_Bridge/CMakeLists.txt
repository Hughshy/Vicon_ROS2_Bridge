cmake_minimum_required(VERSION 3.5)
project(Vicon_ROS2_Bridge)
#set(CMAKE_CXX_FLAGS "-O3")

find_package(Eigen3 REQUIRED)

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

install(TARGETS
    vicon_test
)