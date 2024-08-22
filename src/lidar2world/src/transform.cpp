#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>
#include <tf2/transform_datatypes.h>
#include <sensor_msgs/LaserScan.h>

ros::Publisher points_pub;
tf2_ros::Buffer tfBuffer;

/*void pointCloudCallback(const sensor_msgs::PointCloud2::ConstPtr &msg) {


    try {
        geometry_msgs::TransformStamped transformStamped = tfBuffer.lookupTransform("odom", "base_link", ros::Time(0));

        sensor_msgs::PointCloud2 transformed_cloud;
        tf2::doTransform(*msg, transformed_cloud, transformStamped);

        transformed_cloud.header.frame_id = "odom";
        points_pub.publish(transformed_cloud);
    } catch(tf2::TransformException &e) {
        ROS_WARN("Failed to transform point cloud: %s", e.what());
    }

}*/
void pointCloudCallback(const sensor_msgs::LaserScanConstPtr& laser_scan_msg);
void pointCloudCallback(const sensor_msgs::LaserScanConstPtr& laser_scan_msg)
{
    try {
        // 创建一个PointCloud2类型的消息
        sensor_msgs::PointCloud2 point_cloud_msg;

        // 将LaserScan消息转换为PointCloud2类型
        sensor_msgs::PointCloud2Modifier modifier(point_cloud_msg);
        modifier.setPointCloud2FieldsByString(1, "xyz");
        modifier.resize(laser_scan_msg->ranges.size());
        sensor_msgs::PointCloud2Iterator<float> iter_x(point_cloud_msg, "x");
        sensor_msgs::PointCloud2Iterator<float> iter_y(point_cloud_msg, "y");
        sensor_msgs::PointCloud2Iterator<float> iter_z(point_cloud_msg, "z");
        for (size_t i = 0; i < laser_scan_msg->ranges.size(); ++i, ++iter_x, ++iter_y, ++iter_z) {
            double range = laser_scan_msg->ranges[i];
            double angle = laser_scan_msg->angle_min + i * laser_scan_msg->angle_increment;
            *iter_x = range * cos(angle);
            *iter_y = range * sin(angle);
            *iter_z = 0.0; // Assuming the laser is planar
        }

        // 进行坐标变换
        geometry_msgs::TransformStamped transformStamped = tfBuffer.lookupTransform("odom", "laser_link", ros::Time(0));
        tf2::doTransform(point_cloud_msg, point_cloud_msg, transformStamped);

        // 发布转换后的PointCloud2消息
        points_pub.publish(point_cloud_msg);
    } catch(tf2::TransformException &e) {
        ROS_WARN("Failed to transform point cloud: %s", e.what());
    }
}

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "point_cloud_transform_node");
    ros::NodeHandle nh;

//    ros::Subscriber points_sub = nh.subscribe("/velodyne_points", 10, pointCloudCallback);
    ros::Subscriber points_sub = nh.subscribe("/scan", 10, pointCloudCallback);
    points_pub = nh.advertise<sensor_msgs::PointCloud2>("point_cloud_map", 10);

    
    tf2_ros::TransformListener tfListener(tfBuffer);
    ros::spin();

    return 0;

}
