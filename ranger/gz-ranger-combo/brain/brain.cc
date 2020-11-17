
#include <iostream>
#include <string>
#include <filesystem>
#include <math.h>
#include <string.h>

#include "robot.hh"

using std::cout;
using std::endl;

void
callback(Robot* robot)
{
    float speed = 6 * clamp(0.0, robot->get_range() - 0.25, 1.0);
    cout << "speed: " << speed << endl;
    robot->set_vel(speed, speed);
}

int
main(int argc, char* argv[])
{
    Robot* robot = 0;

    std::string bname(basename(argv[0]));
    cout << "bin: [" << bname << "]" << endl;

    if (bname == "gz_brain") {
        cout << "making robot: Gazebo mode" << endl;
        robot = new GzRobot(argc, argv, callback);
    }

    if (bname == "rg_brain") {
        cout << "making robot: Ranger mode" << endl;
        robot = new RgRobot(argc, argv, callback);
    }

    robot->do_stuff();

    delete robot;
    return 0;
}
