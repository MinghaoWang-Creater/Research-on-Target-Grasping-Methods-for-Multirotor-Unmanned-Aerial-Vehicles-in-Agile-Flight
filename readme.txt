roscore
rosrun dynamixel_sdk_examples read_write_node
roslaunch mavros px4.launch
rosrun hand_controller hand_control2.py

roslaunch mocap_qualisys multi_qualisys.launch
roslaunch Trajectory_Planner planner.launch

rosbag record -a
