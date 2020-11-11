#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

#include "robot.hh"

int port;

RgRobot::RgRobot(int argc, char* argv[], void (*cb)(Robot*))
    : on_update(cb)
{
    int rv;
    port = open("/dev/ttyUSB0", O_RDWR);
    assert(port > 0);

    struct termios cfg;
    rv = tcgetattr(port, &cfg);
    assert(rv != -1);

    cfsetispeed(&cfg, B9600);
    cfsetospeed(&cfg, B9600);

    rv = tcsetattr(port, TCSANOW, &cfg);
    assert(rv != -1);
}

RgRobot::~RgRobot()
{

}

float
RgRobot::get_range()
{
    return range;
}

void
RgRobot::set_vel(double lvel, double rvel)
{
    lvel = clamp(-4, lvel, 4);
    rvel = clamp(-4, rvel, 4);

    char temp[100];
    int s0 = (int)round(lvel * 50);
    int s1 = (int)round(rvel * 50);

    snprintf(temp, 96, "%d %d\n", s0, s1);
    write(port, temp, strlen(temp));
    cout << "cmd: " << temp << endl;
}

void
RgRobot::read_range()
{
    char temp[100];
    int ii = 0;

    for (; ii < 96; ++ii) {
        read(port, temp + ii, 1);
        if (temp[ii] == '\n') {
            break;
        }
    }

    temp[ii+1] = 0;

    this->range = atoi(temp) / 100.0f;
    cout << "range = " << this->range << endl;
}

void
RgRobot::do_stuff()
{
    while (1) {
        this->read_range();
        this->on_update(this);
    }
}
