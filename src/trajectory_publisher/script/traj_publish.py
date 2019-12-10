import pandas as pd
import rospy
from geometry_msgs.msg import Point

pub_rate = 100


def csv_reader(filename):
    df = pd.read_csv(filename, header=None)
    label = ['t', 'x', 'y', 'vx', 'vy']
    df.columns = label
    return df


def publisher():
    path = 'result.csv'
    rospy.loginfo("Start Reading CSV file.")
    df = csv_reader(path)
    rospy.loginfo("Successfully read csv file.")

    rospy.init_node('traj_publish')
    pub = rospy.Publisher('traj_publisher', Point, queue_size=10)
    rate = rospy.Rate(pub_rate)
    i = 0
    row = df.shape[0]
    rospy.loginfo("start publishing")
    while (not rospy.is_shutdown()) and i < row:
        pt = Point()
        pt.x = df.x[i]
        pt.y = df.y[i]
        pub.publish(pt)
        i+=1
        rate.sleep()

    rospy.loginfo("End publishing")


if __name__ == '__main__':
    publisher()
