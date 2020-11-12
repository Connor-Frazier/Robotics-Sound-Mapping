#ifndef RGLIBRARY_HH
#define RGLIBRARY_HH

#include <string>

void create_robot_connection();

void set_all_lights(int r, int g, int b);

void set_single_light(int index, int r, int g, int b);

void lights_off();

void single_light_off(int index);

void move_forward();

void move_backward();

void turn_left();

void turn_right();

void move_forward(int speed);

void move_backward(int speed);

void turn_left(int speed);

void turn_right(int speed);

void stop();

void tank_drive(int rspeed, int lspeed);

void make_noise(int frequency, int duration);

double read_line_sensor();

double read_light_sensor();

double read_temperature_sensor();

double read_sound_sensor();

double read_sonar_sensor();

double read_gyroscope_x();

double read_gyroscope_y();

double read_gyroscope_z();

std::string read_serial();




#endif
