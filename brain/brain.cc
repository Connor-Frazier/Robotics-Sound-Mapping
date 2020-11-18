
#include <iostream>
#include <math.h>
#include <unistd.h>

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
    cout << robot->get_line_status() << endl;
    //
    // cout << robot->get_noise_sensor() << endl;

    // robot->set_vel(200.0, 200.0);

    sleep(5);

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

    if(bname == "rg_brain") {
      std::cout << "making robot: Ranger mode" << '\n';
      robot = new RgRobot(argc, argv, callback);
    }

    sleep(2);

    robot->do_stuff();
    delete robot;
    return 0;
}
