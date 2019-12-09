cmake_minimum_required(VERSION 2.8.3)
project(trajectory_publisher)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS
        roscpp
        std_msgs
        geometry_msgs
        )

catkin_package(
)

## Your package locations should be listed before other locations
include_directories(
        ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/trajectory_publish.cpp)


 target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
 )

