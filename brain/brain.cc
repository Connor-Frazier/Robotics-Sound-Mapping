
#include <iostream>
#include <math.h>
#include <unistd.h>

#include "robot.hh"

using std::cout;
using std::endl;

int stateCount;
void
callback(Robot* robot)
{
    cout << robot->get_line_status() << endl;

    cout << robot->get_noise_sensor() << endl;

    stateCount++;
    if(stateCount < 5){
         robot->set_vel(200.0, 200.0);
    } else if(stateCount < 10){
      robot->set_vel(-200.0, -200.0);
    } else if(stateCount < 10){
      stateCount = 0;
      robot->set_vel(-200.0, 200.0);
    }

    return;
}

int
main(int argc, char* argv[])
{
    stateCount = 0;
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
