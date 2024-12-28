#!/usr/bin/env python3

import rospy
from dynamixel_sdk_examples.msg import SetPosition

def main():
    rospy.init_node('dynamixel_controller_publisher')
    pub = rospy.Publisher('/set_position', SetPosition, queue_size=10)
    rate = rospy.Rate(10)  # 发布速率

    while not rospy.is_shutdown():
        # 创建SetPosition消息
        msg1 = SetPosition()
        msg1.id = 1  # 舵机ID
        msg1.position = 1000  # 目标位置
        msg2 = SetPosition()
        msg2.id = 2  # 舵机ID
        msg2.position = 1000  # 目标位置

        # 发布消息
        pub.publish(msg1)
        pub.publish(msg2)
        rate.sleep()

        # # 创建SetPosition消息
        # msg1 = SetPosition()
        # msg1.id = 1  # 舵机ID
        # msg1.position = 0  # 目标位置
        # msg2 = SetPosition()
        # msg2.id = 2  # 舵机ID
        # msg2.position = 0  # 目标位置

        # # 发布消息
        # pub.publish(msg1)
        # pub.publish(msg2)
        # rate.sleep()

if __name__ == '__main__':
    main()
