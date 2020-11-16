
#include <iostream>
#include <math.h>

#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
using namespace cv;

#include "robot.hh"

using std::cout;
using std::endl;

/*
To view the camera image in time, you could press CTRL-T in Gazebo
, choosing the Topic-"~/tankbot0/tankbot/camera_sensor/link/camera/image",
then a Image Window will pop up, in order to view the Image in time.
*/

int
get_line_status(Robot* robot) {
    if (!robot->frame.empty()) {
        // there is a 255 separator after every 3 rgb values, so we get 4
        cv::Vec4b left_front = robot->frame.at<Vec4b>(0, 0);
        cv::Vec4b left_back = robot->frame.at<Vec4b>(1, 0);
        cv::Vec4b right_front = robot->frame.at<Vec4b>(0, 1);
        cv::Vec4b right_back = robot->frame.at<Vec4b>(1, 1);

        int leftfront = false;
        int leftback = false;
        int rightfront = false;
        int rightback = false;

        // find dark lines
        if (left_front[0] < 100 && left_front[1] < 100 && left_front[2] < 100)
        {
            leftfront = true;
        }
        if (left_back[0] < 100 && left_back[1] < 100 && left_back[2] < 100)
        {
            leftback = true;
        }
        if (right_front[0] < 100 && right_front[1] < 100 && right_front[2] < 100)
        {
            rightfront = true;
        }
        if (right_back[0] < 100 && right_back[1] < 100 && right_back[2] < 100)
        {
            rightback = true;
        }

        // return status like the mBot ranger
        if (leftfront && leftback && rightfront && rightback)
        {
            return 3;
        }
        else if (leftfront && leftback)
        {
            return 1;
        }
        else if (rightfront && rightback)
        {
            return 2;
        }

    }
    return 0;
}

void
callback(Robot* robot)
{
    if (robot->range < 1){
        robot->set_vel(-1, 1);
        return;
    }

    int line_status = get_line_status(robot);
    cout << "Line status: " << line_status << endl;

    robot->set_vel(2.0, 2.0);
    cout << "Noise is: " << robot->noise << endl;
    return;
}

int
main(int argc, char* argv[])
{

    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
