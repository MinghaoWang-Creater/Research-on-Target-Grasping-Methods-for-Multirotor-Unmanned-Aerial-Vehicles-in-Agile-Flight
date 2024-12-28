#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include "control.h"
using namespace std;

nav_msgs::Path plan_tra;
// geometry_msgs::PoseStamped px4_pose;

// 回调函数：储存飞控当前状态   store controller's current state
mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

// 回调函数：应用目标轨迹中的轨迹点信息  apply trajectory point information from target trajectory
void trajectory_cb(const nav_msgs::Path::ConstPtr &wp)
{
    plan_tra = *wp;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber pose_sub;
    ros::Subscriber path_sub;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(60.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    // pose_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 10, pose_cb);     //订阅飞机位置pose    subscribe drone pose
    path_sub = nh.subscribe<nav_msgs::Path>("/tra_generation", 10,trajectory_cb);     //订阅轨迹消息  subscribe trajectory information

    geometry_msgs::PoseStamped pose;
    // pose.pose.position.x = 0;
    // pose.pose.position.y = 0;
    // pose.pose.position.z = 2;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    // mavros_msgs::SetMode offb_set_mode;
    // offb_set_mode.request.custom_mode = "OFFBOARD";

    // mavros_msgs::CommandBool arm_cmd;
    // arm_cmd.request.value = true;
    // std::cout<<"hhhhhhhhhhhh"<<std::endl;

    // ros::Time last_request = ros::Time::now();
    // ros::Time t = last_request;

    while(ros::ok()){
        // if( current_state.mode != "OFFBOARD" &&
        //     (ros::Time::now() - last_request > ros::Duration(5.0))){
        //     if( set_mode_client.call(offb_set_mode) &&
        //         offb_set_mode.response.mode_sent){
        //         ROS_INFO("Offboard enabled");
        //     }
        //     last_request = ros::Time::now();
        // } else {
        //     if( !current_state.armed &&
        //         (ros::Time::now() - last_request > ros::Duration(5.0))){
        //         if( arming_client.call(arm_cmd) &&
        //             arm_cmd.response.success){
        //             ROS_INFO("Vehicle armed");
        //         }
        //         last_request = ros::Time::now();
        //     }
        // }

        // if( current_state.mode != "OFFBOARD" &&
        //     !current_state.armed){
        //     if( set_mode_client.call(offb_set_mode) &&
        //         offb_set_mode.response.mode_sent){
        //         ROS_INFO("Offboard enabled");
        //     }
        //     if( arming_client.call(arm_cmd) &&
        //             arm_cmd.response.success){
        //             ROS_INFO("Vehicle armed");
        //     }
        //     last_request = ros::Time::now();
        // }


        pose.header.frame_id = "/map";
        pose.header.stamp = ros::Time::now();
        // ros::Time t0 = ros::Time::now();
    
        for(int i=0; i < plan_tra.poses.size(); i++)
        {
            // 发布轨迹控制话题（速度or位置） publish trajectory controlling topic
            pose.pose.position.x = plan_tra.poses[i].pose.position.x;
            pose.pose.position.y = plan_tra.poses[i].pose.position.y;
            pose.pose.position.z = plan_tra.poses[i].pose.position.z;
            // pose.pose.header.stamp = t0 + 0.01*i;

            local_pos_pub.publish(pose); // 位置控制  position controlling
            // std::cout<<"hahahahha"<<std::endl;
            ros::spinOnce();
            rate.sleep();
        }
        

        // local_pos_pub.publish(pose);

        // ros::spinOnce();
        // rate.sleep();
    }

    

    return 0;
}