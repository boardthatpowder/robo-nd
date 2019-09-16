#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>




class AddMarkers 
{
public:
  AddMarkers();

private:
  ros::NodeHandle n;

  // pickup goal
  float pickup_pose_x, pickup_pose_y, pickup_pose_z; 
  float pickup_orient_x, pickup_orient_y, pickup_orient_z, pickup_orient_w;

  // dropoff goal
  float dropoff_pose_x, dropoff_pose_y, dropoff_pose_z; 
  float dropoff_orient_x, dropoff_orient_y, dropoff_orient_z, dropoff_orient_w;

  // track state of goal
  bool pickedUp=false;
  bool droppedOff=false;
  float pos_thresh=0.1;

  // track current odom
  geometry_msgs::Pose odom;

  // marker
  uint32_t shape;
  visualization_msgs::Marker marker;


  // publishers / subscriverrs
  ros::Publisher marker_pub;
  ros::Subscriber odom_subscriber;

  void odomCallback(const nav_msgs::Odometry::ConstPtr &msg);
  void process();
};

// constructor
AddMarkers::AddMarkers() {
  // retrieve markers from config
  n.param("/my_robot/pickup/pose_pos_x", pickup_pose_x, 0.0f);
  n.param("/my_robot/pickup/pose_pos_y", pickup_pose_y, 0.0f);
  n.param("/my_robot/pickup/pose_pos_z", pickup_pose_z, 0.0f);
  n.param("/my_robot/pickup/pose_orient_x", pickup_orient_x, 0.0f);
  n.param("/my_robot/pickup/pose_orient_y", pickup_orient_y, 0.0f);
  n.param("/my_robot/pickup/pose_orient_z", pickup_orient_z, 0.0f);
  n.param("/my_robot/pickup/pose_orient_w", pickup_orient_w, 0.0f);

  n.param("/my_robot/dropoff/pose_pos_x", dropoff_pose_x, 1.0f);
  n.param("/my_robot/dropoff/pose_pos_y", dropoff_pose_y, 1.0f);
  n.param("/my_robot/dropoff/pose_pos_z", dropoff_pose_z, 0.0f);
  n.param("/my_robot/dropoff/pose_orient_x", dropoff_orient_x, 0.0f);
  n.param("/my_robot/dropoff/pose_orient_y", dropoff_orient_y, 0.0f);
  n.param("/my_robot/dropoff/pose_orient_z", dropoff_orient_z, 0.0f);
  n.param("/my_robot/dropoff/pose_orient_w", dropoff_orient_w, 0.0f);

  ROS_INFO("Pickup goal: pose: x:%f, y:%f, z:%f. orient: a:%f, y:%f, z:%f, w:%f",
    pickup_pose_x,pickup_pose_y,pickup_pose_z,pickup_orient_x,pickup_orient_y,pickup_orient_z,pickup_orient_w);

  ROS_INFO("Dropoff goal: pose: x:%f, y:%f, z:%f. orient: a:%f, y:%f, z:%f, w:%f",
    dropoff_pose_x,dropoff_pose_y,dropoff_pose_z,dropoff_orient_x,dropoff_orient_y,dropoff_orient_z,dropoff_orient_w);

  // Set our initial shape type to be a cube
  shape = visualization_msgs::Marker::CUBE;

  // publishers/subscribers
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  odom_subscriber = n.subscribe("/odom", 10, &AddMarkers::odomCallback, this);

  // marker
  marker.header.frame_id = "map";
  marker.ns = "add_markers";
  marker.id = 0;
  marker.type = shape;
  marker.scale.x = 0.5;
  marker.scale.y = 0.5;
  marker.scale.z = 0.5;
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

}

void AddMarkers::odomCallback(const nav_msgs::Odometry::ConstPtr &msg) {

  // store the current pose
  odom.position.x = msg->pose.pose.position.x;
  odom.position.y = msg->pose.pose.position.y;
  odom.position.z = msg->pose.pose.position.z;
  odom.orientation.x = msg->pose.pose.orientation.x;
  odom.orientation.y = msg->pose.pose.orientation.y;
  odom.orientation.z = msg->pose.pose.orientation.z;
  odom.orientation.w = msg->pose.pose.orientation.w;

  // ROS_INFO_STREAM("odomCallback(): odom: " << odom);

  this->process();

}

void AddMarkers::process() {

  while (marker_pub.getNumSubscribers() < 1)
  {
    if (!ros::ok()) return;
    ROS_WARN_ONCE("process(): Please create a subscriber to the marker");
    sleep(1);
  }

  marker.header.stamp = ros::Time::now();
  marker.lifetime = ros::Duration();

  ROS_INFO_THROTTLE(1, "process(): Pre pickedUp: %d, droppedOff: %d", pickedUp, droppedOff);


  if (!pickedUp) {
    // not picked up yet, therefore add pickup marker      
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = pickup_pose_x;
    marker.pose.position.y = pickup_pose_y;
    marker.pose.position.z = pickup_pose_z;
    marker.pose.orientation.x = pickup_orient_x;
    marker.pose.orientation.y = pickup_orient_y;
    marker.pose.orientation.z = pickup_orient_z;
    marker.pose.orientation.w = pickup_orient_w;

    // check if close...
    float dx=abs(odom.position.x-pickup_pose_x);
    float dy=abs(odom.position.y-pickup_pose_y);

    if (dx<pos_thresh && dy<pos_thresh ) {
      ROS_INFO_THROTTLE(1, "process(): getting close to pick up !");
      // we're close
      marker.action = visualization_msgs::Marker::DELETE;
      pickedUp=true;
    }
    

  } else if (!droppedOff) {

    // check if close...
    float dx=abs(odom.position.x-dropoff_pose_x);
    float dy=abs(odom.position.y-dropoff_pose_y);

    if (dx<pos_thresh && dy<pos_thresh ) {
      ROS_INFO_THROTTLE(1, "process(): getting close to drop off!");
      droppedOff=true;
    }
  } else {

    // show dropoff marker
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = dropoff_pose_x;
    marker.pose.position.y = dropoff_pose_y;
    marker.pose.position.z = dropoff_pose_z;
    marker.pose.orientation.x = dropoff_orient_x;
    marker.pose.orientation.y = dropoff_orient_y;
    marker.pose.orientation.z = dropoff_orient_z;
    marker.pose.orientation.w = dropoff_orient_w;
  }

  // Publish the marker
  marker_pub.publish(marker);

  ROS_INFO_THROTTLE(1, "process(): Post pickedUp: %d, droppedOff: %d", pickedUp, droppedOff);

  ros::spinOnce();

}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");

  // encapsulate all our logic in a calss
  AddMarkers addMarkers;
  
  ros::Rate r(1);
  ros::spin();
  return 0;
  
}