#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 PROJECT_ROOT" >&2
  exit 1
fi
if ! [ -e "$1" ]; then
  echo "PROJECT_ROOT not found" >&2
  exit 1
fi
if ! [ -d "$1" ]; then
  echo "PROJECT_ROOT not a directory" >&2
  exit 1
fi

export PROJECT_ROOT=$1

#xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PROJECT_ROOT/src/worlds/project5.world " &
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch " &
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 5
xterm -e " roslaunch add_markers add_markers.launch " &

