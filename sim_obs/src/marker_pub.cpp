#include "ros/ros.h"
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <gazebo_msgs/ModelStates.h>
#include <iostream>
#include <vector>

using namespace std;


typedef struct Pose
{
    double x,y,z;
    double vx,vy,vz,vw;
}Pose;

typedef struct modelState
{
    Pose pose;
    string itemName;
}modelState;

class markerVis
{
    public:
        markerVis();
        ~markerVis();
        void start(void);
        
    private:
        void callback(const gazebo_msgs::ModelStates::ConstPtr& msg);
        ros::Publisher markerPub;
        ros::Subscriber gazebo_sub;
        std::vector<modelState> modelStateVector;
    
        

}; // end of class


markerVis::markerVis()
{
    //Private parameters handler
    ros::NodeHandle node_handle;
    //Publishers and Subscribers
    markerPub = node_handle.advertise<visualization_msgs::MarkerArray>( "detected_obstacle", 0 );
    gazebo_sub = node_handle.subscribe("/gazebo/model_states",1,&markerVis::callback,this);
}

markerVis::~markerVis()
{
};

void markerVis::callback(const gazebo_msgs::ModelStates::ConstPtr& msg){
    modelStateVector.clear();
    for (int i = 0; i < msg->name.size(); i++){
        
        if(!msg->name[i].compare("ground_plane") || !msg->name[i].compare("isci_robot")){
            continue;
        }
        else{            
            modelState model;
            model.itemName = msg->name[i];
            model.pose.x=msg->pose[i].position.x;
            model.pose.y=msg->pose[i].position.y;
            model.pose.z=msg->pose[i].position.z;
            model.pose.vx= msg->pose[i].orientation.x;
            model.pose.vy= msg->pose[i].orientation.y;
            model.pose.vz= msg->pose[i].orientation.z;
            model.pose.vw= msg->pose[i].orientation.w;
            modelStateVector.push_back(model);
        }
    }
}

void markerVis::start()
{
    ros::Rate rate(60);
    while (ros::ok()) {
        
        visualization_msgs::MarkerArray marker_all;
        visualization_msgs::Marker marker;
        marker.header.frame_id = "odom";
        marker.header.stamp = ros::Time();
        marker.ns = "my_namespace";
        marker.scale.x = 0.5;
        marker.scale.y = 0.5;
        marker.scale.z = 1.8;
        marker.color.a = 0.8; // Don't forget to set the alpha!
        marker.color.r = 0.4;
        marker.color.g = 0.4;
        marker.color.b = 0.4;

        for (int ik = 0; ik < modelStateVector.size(); ik++)
        {
            marker.id = ik;            
            marker.action = visualization_msgs::Marker::ADD;
            if(!modelStateVector[ik].itemName.compare("move_unit_cylinder") ){
                marker.type = visualization_msgs::Marker::CYLINDER;
            }
            marker.pose.position.x = modelStateVector[ik].pose.x;
            marker.pose.position.y =  modelStateVector[ik].pose.y;
            marker.pose.position.z =  modelStateVector[ik].pose.z;
            marker.pose.orientation.x = modelStateVector[ik].pose.vx;
            marker.pose.orientation.y = modelStateVector[ik].pose.vy;
            marker.pose.orientation.z = modelStateVector[ik].pose.vz;
            marker.pose.orientation.w = modelStateVector[ik].pose.vw;
            marker_all.markers.push_back(marker);
        }

        markerPub.publish( marker_all );
        ros::spinOnce();
        rate.sleep();
    }
    
}

/*****************/
/* MAIN FUNCTION */
/*****************/
int main(int argc, char **argv)
{
    //Initiate ROS
    ros::init(argc, argv, "marker");
    markerVis marker;
    
    marker.start();
        
    ros::waitForShutdown();
    return 0;
}
