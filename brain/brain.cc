
#include <iostream>
#include <math.h>

#include "robot.hh"

using std::cout;
using std::endl;

/*
To view the camera image in time, you could press CTRL-T in Gazebo
, choosing the Topic-"~/tankbot0/tankbot/camera_sensor/link/camera/image",
then a Image Window will pop up, in order to view the Image in time.
*/

void
callback(Robot* robot)
{
    cout << robot->pos_t << endl;
    //cout << "here2" <<endl;
    //cam_show(robot->frame);
    if (robot->range < 1){
        robot->set_vel(-1, 1);
        return;
    }

    robot->set_vel(2.0, 2.0);
    return;
}

int
main(int argc, char* argv[])
{
    cout << "here1" <<endl;


    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
