#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "ros/ros.h"
#include <geometry_msgs/Point.h>

using namespace std;

const string fp = "/home/gg/catkin_gx_ws/src/trajectory_publisher/result.csv";

ros::Publisher trajectory_pub;
vector <vector<double>> traj;
int counter = 0;

void csvParser(string filename, vector <vector<double>> &result) {
    ROS_INFO("Starting reading from file.");
    ifstream in;
    in.open(filename);
    while (in.good()) {
        string lineStr;
        while (getline(in, lineStr)) {
            stringstream ss(lineStr);
            string str;
            vector<double> lineArray;
            while (getline(ss, str, ',')) {
                double num = stod(str);
                lineArray.emplace_back(num);
            }
            result.emplace_back(lineArray);
        }
    }
    in.close();
    ROS_INFO("Reading from trajectory file finished.");
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "publish_tragectory");
    ros::NodeHandle nh;
    trajectory_pub = nh.advertise<geometry_msgs::Point>("ring_buffer/desire_point", 10);
    csvParser(fp, traj);
    ROS_INFO("start publishing");
    ros::Rate loop_rate(10);
    double initx = traj[0][1] / 100;
    double inity = traj[0][2] / 100;
    while (ros::ok() && counter < traj.size()) {
        static geometry_msgs::Point pt;
        vector<double> row = traj[counter];
        double tempx = row[1] / 100;
        double tempy = row[2] / 100;
        pt.y = tempx - initx;
        pt.x = inity - tempy;
        counter++;
        trajectory_pub.publish(pt);
        loop_rate.sleep();
    }
    ROS_INFO("end publishing");
    return 0;
}