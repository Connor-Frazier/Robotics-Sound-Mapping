
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "ranger-library.hh"

using namespace std;

int
main(int argc, char* argv[])
{
	create_robot_connection("/dev/ttyUSB0");
	sleep(2);
	set_all_lights(0, 0, 255);
	sleep(2);
	lights_off();
	sleep(2);
	set_single_light(1, 0, 255, 0);
	sleep(2);
	single_light_off(1);
	sleep(2);
	move_forward();
	sleep(2);
	stop();
	sleep(2);
	move_backward();
	sleep(2);
	stop();
	sleep(2);
	turn_left();
	sleep(2);
	stop();
	sleep(2);
	turn_right();
	sleep(2);
	stop();
	sleep(2);
	move_forward(250);
	sleep(2);
	stop();
	sleep(2);
	move_backward(250);
	sleep(2);
	stop();
	sleep(2);
	turn_left(250);
	sleep(2);
	stop();
	sleep(2);
	turn_right(250);
	sleep(2);
	stop();
	sleep(2);
	tank_drive(200, -200);
	sleep(2);
	stop();
	sleep(2);
	tank_drive(-200, 200);
	sleep(2);
	stop();
	sleep(2);
	tank_drive(200, 200);
	sleep(2);
	stop();
	sleep(2);
	tank_drive(-200, -170);
	sleep(2);
	stop();
	sleep(2);
	make_noise(1000, 100);
	sleep(2);
	cout << "line sensor: " << read_line_sensor() << endl;
	sleep(2);
	cout << "light sensor: " << read_light_sensor() << endl;
	sleep(2);
	cout << "temperature: " << read_temperature_sensor() << endl;
	sleep(2);
	cout << "sound sensor: " << read_sound_sensor() << endl;
	sleep(2);
	cout << "sonar sensor: " << read_sonar_sensor() << endl;
	sleep(2);
	cout << "Gyro X: " << read_gyroscope_x() << endl;
	sleep(2);
	cout << "Gyro Y: " << read_gyroscope_y() << endl;
	sleep(2);
	cout << "Gyro Z: " << read_gyroscope_z() << endl;
	sleep(2);



}
