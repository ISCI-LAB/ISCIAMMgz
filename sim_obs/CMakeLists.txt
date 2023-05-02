cmake_minimum_required(VERSION 3.0.2)
project(sim_obs)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  roscpp
  rospy
  std_msgs
  tf
  gazebo_ros
  visualization_msgs
)

find_package(Boost REQUIRED COMPONENTS
  system
  filesystem
  log_setup
  log
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES sim_obs
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
  DEPENDS
    Boost
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

link_directories(${GAZEBO_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

# For earthquake_plugin plugin
add_library(plannar_unitCylinder_plugin src/plannar_unitCylinder.cpp)
target_link_libraries(plannar_unitCylinder_plugin ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

#maker pub
ADD_EXECUTABLE(marker_pub src/marker_pub.cpp )
TARGET_LINK_LIBRARIES(marker_pub ${catkin_LIBRARIES} ${Boost_LIBRARIES} rt)

#obs_telep
ADD_EXECUTABLE(obs_telep src/obs_telep.cpp )
TARGET_LINK_LIBRARIES(obs_telep ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})
