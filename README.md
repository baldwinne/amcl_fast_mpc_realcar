## amcl+fast_planner+mpc实现差速小车定位与规控
## 参考的项目：
轨迹规划：https://github.com/HKUST-Aerial-Robotics/Fast-Planner
MPC： https://github.com/Dangko/ego-planner-for-ground-robot
https://github.com/USE-jx/LMPC_OSQP_EIGEN/
## 代码运行效果视频：
单无人车实车运行视频：https://www.bilibili.com/video/BV1Q2WEeoEWF/?spm_id_from=333.999.0.0&vd_source=f4ec4aca12529934576bd467bb68a430
多无人车多点访问仿真视频：https://www.bilibili.com/video/BV1ZE421379M/?spm_id_from=333.999.0.0&vd_source=f4ec4aca12529934576bd467bb68a430
## 求解器
需要安装OSQP，OSQP-Eigen，自行找博客安装。
## 编译
编译mpc_tracking 这个功能包报错找不到什么包可以先编译fastplanner，然后再编译其他的，当然没编译过fastplanner的需要去安装点东西，具体可以去fastplannergithub仓库去看去安装一下。

## packages介绍

## 1 robot

`功能：` 启动仿真环境，会出现一个带雷达的小车。
source devel/setup.bash

`启动：`
启动实车：roslaunch amclwithmap_pkg amcl_with_map.launch map_name:=my_map sim:=false
启动仿真：roslaunch amclwithmap_pkg amcl_with_map.launch map_name:=my_map08202100 sim:=true

实车中：1、编写了AMCL功能包、map_Server；加入了handsfree_hw小车底盘、雷达硬件驱动
2、更改fast_planner订阅的/odom
3、更改mpc_node订阅或发布的/odom、/cmd_vel
4、注释了handsfree_hw包里的joint_publisher避免重复启动该节点
## 2 lidar2world

`功能：` 把雷达系的点云转到odom系下，传给fastplanner的建图包。

`启动：`  rosrun lidar2world lidar2world_node

## 3 mpc_tracking

`功能：` 跟踪fastplanner生成的局部轨迹

`启动：` rosrun mpc_tracking mpc_tracking_node

### 4 Fast-planner

`功能：` 实时建立小车前方摄像头可视范围内的障碍物地图，实时地生成避障轨迹，是一个位置环境的局部规划器。

详情请见：https://github.com/HKUST-Aerial-Robotics/Fast-Planner

1. 运行算法：`roslaunch plan_manage 16_lidar.launch`
2. 运行rviz可视化：`roslaunch plan_manage rviz.launch`


