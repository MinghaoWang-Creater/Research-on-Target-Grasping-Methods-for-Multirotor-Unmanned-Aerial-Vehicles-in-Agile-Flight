#!/usr/bin/env python3 
import rospy
from mavros_msgs.msg import RCIn
# from dynamixel_sdk_examples.msg import SetPosition

class DynamixelController:
    def __init__(self):
        rospy.init_node('dynamixel_controller_subscriber')
        self.sub = rospy.Subscriber('/mavros/rc/in', RCIn, self.rc_callback)
        # self.pub = rospy.Publisher('/set_position', SetPosition, queue_size=10)
        self.rate = rospy.Rate(10) # 发布速率

    def rc_callback(self, msg):
        # 解析拨杆位置
        throttle = msg.channels[7] # 假设拨杆信息在第3个通道
        if throttle >= 1700: # 拨至最上位置
            print("up")
        elif 1300 <= throttle < 1700: # 中间位置
            print("mid")
        else: # 最下位置
            print("down")
        # position = self.map_throttle_to_position(throttle)

        # # 创建SetPosition消息
        # set_position_msg = SetPosition()
        # set_position_msg.id = 1 # 舵机ID
        # set_position_msg.position = position

        # # 发布消息
        # self.pub.publish(set_position_msg)

    # def map_throttle_to_position(self, throttle):
    #     # 根据拨杆位置映射舵机位置
    #     if throttle >= 1700: # 拨至最上位置
    #         return 0
    #     elif 1300 <= throttle < 1700: # 中间位置
    #         return 500
    #     else: # 最下位置
    #         return 1000

    def run(self):
        while not rospy.is_shutdown():
            self.rate.sleep()

if __name__ == '__main__':
    controller = DynamixelController()
    controller.run()

