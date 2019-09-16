# Project 5 (Home Service) Writeup

## Packages

This project makes use of the following ROS packages:

- slam_gmapping
- turtlebot
- turtlebot_interactions
- turtlebot_simulator

The following custom packages were created:

### pick_objects

The `pick_objects` node programmatically sets 2 navigation goals, which are configured in `src/pick_objects/launch/pick_objects.launch`.  Once the robot has reached the first goal, it will pause for 5 seconds, before venturing onto the next.

### add_markers

The `add_markers` node reads in the odometry to detect the robots current position.  If the robot has not reached the first goal yet, a marker is drawn where the first goal resides.  Once it reaches the first goal, it is hidden, then later readded as a marker when the robot reaches the target goal.

Note that this described behavior is per the following extract from the project rubik, which is slightly different to what is listed in the project lessons:

- Initially show the marker at the pickup zone.
- Hide the marker once your robot reach the pickup zone.
- Wait 5 seconds to simulate a pickup.
- Show the marker at the drop off zone once your robot reaches it.

## Mapping

The map created as part of project 3, using pgm_map_creator, was reused for this project.

This can be tested by running the following, where <project_dir> should be replaced by the absolute root directory.

```sh
src/scripts/test_slam.sh <project_dir>

# e.g.
src/scripts/test_slam.sh /home/workspace
```


## Localization and Navigation

Localizaion and navigation can be tested by running the following (again replacing <project_dir>):

```sh
src/scripts/test_navigation.sh <project_dir>

# e.g.
src/scripts/test_navigation.sh /home/workspace
```

## Home Service run

The end soluion can be executed by running the following following (again replacing <project_dir>):

```sh
src/scripts/home_service.sh <project_dir>

# e.g.
src/scripts/home_service.sh /home/workspace
```