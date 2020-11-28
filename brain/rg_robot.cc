#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "robot.hh"
#include "ranger-library.hh"

double xcount = 200;
double ycount = 200;
double calls = 0;



RgRobot::RgRobot(int argc, char* argv[], void (*cb)(Robot*))
    : on_update(cb)
{
    create_robot_connection("/dev/ttyUSB1");
}

RgRobot::~RgRobot()
{

}

int
RgRobot::get_line_status(){
  int line = read_line_sensor();
  if(line == 0){
      line = 3;
  }
  else if(line == 3) {
      line = 0;
  }
  return line;
}

double
RgRobot::get_noise_sensor(){
  return round(read_sound_sensor()/10);
}

double
RgRobot::get_robot_theta(){
  double angle = read_gyroscope_z();
  return (angle * (3.14/180));
}

double
RgRobot::get_robot_y(){
    double angle = read_gyroscope_z();
    double helperangle = angle * (3.14/180);
    if (abs(helperangle) <=.75 ){
        ycount++;
    }
    else if (abs(helperangle) >=2.25 ){
        ycount--;
    }

    return ycount;
}

double
RgRobot::get_robot_x(){
    double angle = read_gyroscope_z();
    double  helperangle = angle * (3.14/180);

    if (helperangle >=-2.2 && helperangle < -.75){
        xcount++;
    }
    else if (helperangle <=2.2 && helperangle > .75 ){
        xcount--;
    }

	return xcount;
}



void
RgRobot::set_vel(double lvel, double rvel)
{
    cout << "Tank drive " << lvel << " , " << rvel << endl;
    lvel = lvel * 50;
    rvel = rvel * 50;
    cout << "Tank drive " << lvel << " , " << rvel << endl;
    tank_drive(rvel, lvel);
}

void
RgRobot::do_stuff()
{
    while (1) {
        cout << "\n == iterate ==" << endl;
        this->on_update(this);
    }
}
