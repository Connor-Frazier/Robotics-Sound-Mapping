
#include <iostream>
#include <math.h>

#include "robot.hh"
#include "cam.hh"

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
    cout << "here2" <<endl;
    cam_show(robot->frame);
    
    cout << "the range data : " << robot->range << endl;
    if (robot->range < 1.7)
    {
        robot->set_vel(-0.3, 0.3);
        return;
    }
    robot->set_vel(0.5, 0.5);
}

int
main(int argc, char* argv[])
{
    cout << "here1" <<endl;
    cam_init();

    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
