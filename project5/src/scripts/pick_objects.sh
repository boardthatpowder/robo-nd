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

xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PROJECT_ROOT/src/worlds/project5.world " &
sleep 5
#xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch " &
xterm -e " roslaunch pick_objects amcl_project5.launch " &
sleep 5
#xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch " &
xterm -e " roslaunch pick_objects view_navigation_project5.launch " &
sleep 5
xterm -e " roslaunch pick_objects pick_objects.launch " &

