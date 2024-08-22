cmake_minimum_required(VERSION 3.0.2)
project(amclwithmap_pkg)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  nav_msgs
  geometry_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

# 如果你有源代码文件，需要编译成可执行文件
#add_executable(amcl_node src/amcl_node.cpp)
#target_link_libraries(amcl_node ${catkin_LIBRARIES})

# 如果你有测试文件，类似地添加它们
# add_executable(test_node src/test_node.cpp)
# target_link_libraries(test_node ${catkin_LIBRARIES})

