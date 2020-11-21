
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

    cout << robot->get_robot_theta() << endl;

    robot->set_vel(-5.0, 5.0);

    stateCount++;
    // std::cout << "state count: " << stateCount << '\n';
    // if(stateCount < 3){
    //      robot->set_vel(5.0, 5.0);
    // } else if(stateCount < 6){
    //   robot->set_vel(-5.0, -5.0);
    // } else if(stateCount > 6 ){
    //   stateCount = 0;
    //   robot->set_vel(-5.0, 5.0);
    // }

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
