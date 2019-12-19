#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "ros/ros.h"
#include <geometry_msgs/Point.h>

using namespace std;

//const string fp = "/home/gg/catkin_gx_ws/src/trajectory_publisher/realexpMap1-astar.csv";
const string fp = "/home/gg/catkin_gx_ws/src/trajectory_publisher/realExpriment1-smooth.csv";


ros::Publisher trajectory_pub;
vector <vector<double>> traj;


void csvParser(const string &filename, vector <vector<double>> &result) {
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
	int counter = 0;
	float hz = 20;
    ros::init(argc, argv, "publish_tragectory");
    ros::NodeHandle nh;
    trajectory_pub = nh.advertise<geometry_msgs::Point>("ring_buffer/desire_point", 10);
    csvParser(fp, traj);
    ROS_INFO("start publishing");
    ROS_INFO("FILE name:");
    ROS_INFO_STREAM(fp);
    ros::Rate loop_rate(hz);
    double initx = traj[0][0] / 100;
    double inity = traj[0][1] / 100;
    while (ros::ok() && counter < hz * 5) {
    	ROS_INFO("waiting");
    	counter++;
    	loop_rate.sleep();
    }
    counter = 0;
    while (ros::ok() && counter < traj.size()) {
    	// ROS_INFO("in loop");
        static geometry_msgs::Point pt;
        if (counter < 0){
        	ROS_INFO("counter less than 0");
        } else {
			vector<double> row = traj[counter];
        	double tempx = row[0] / 100;
        	double tempy = row[1] / 100;


        	// // astar use following
        	// pt.x = tempx - initx;
        	// pt.y = tempy - inity;

        	// path smooth use folloing
        	pt.y = tempx - initx;
        	pt.x = inity - tempy;

        
        	trajectory_pub.publish(pt);
        }
		
		counter++;
        loop_rate.sleep();
    }
    ROS_INFO("end publishing");
    return 0;
}