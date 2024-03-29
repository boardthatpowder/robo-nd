#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the pick_objects node
  ros::init(argc, argv, "pick_objects");

  ros::NodeHandle n;

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();



  // Define a position and orientation for pickup goal
  n.param("/my_robot/pickup/pose_pos_x", goal.target_pose.pose.position.x, 0.0f);
  n.param("/my_robot/pickup/pose_pos_y", goal.target_pose.pose.position.y, 0.0f);
  n.param("/my_robot/pickup/pose_orient_w", goal.target_pose.pose.orientation.w, 0.0f);

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending pickup goal");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the robot reached the pickup goal");
  else
    ROS_INFO("The base failed to move to the pickup goal for some reason");

  // Wait 5 sec at the pickup goal
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting at the pickup goal location");
  }


  // Define a position and orientation for dropoff goal
  n.param("/my_robot/dropoff/pose_pos_x", goal.target_pose.pose.position.x, 0.0f);
  n.param("/my_robot/dropoff/pose_pos_y", goal.target_pose.pose.position.y, 0.0f);
  n.param("/my_robot/dropoff/pose_orient_w", goal.target_pose.pose.orientation.w, 0.0f);

   // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending dropoff goal");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the robot reached the dropoff goal");
  else
    ROS_INFO("The base failed to move to the dropoff goal for some reason");

  return 0;
}