
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
    cout << "Line status: " << robot->get_line_status() << endl;

    cout << "Noise is: " << robot->get_noise_sensor() << endl;

    robot->set_vel(2.0, 2.0);

    return;
}

int
main(int argc, char* argv[])
{
    Robot* robot = 0;

    std::string bname(basename(argv[0]));
    std::cout << "bin: [" << bname << "]" << endl;

    if(bname == "gz_brain") {
      std::cout << "making robot: Gazebo mode" << '\n';
      robot = new GzRobot(argc, argv, callback);
    }

    robot->do_stuff();
    delete robot;
    return 0;
}
