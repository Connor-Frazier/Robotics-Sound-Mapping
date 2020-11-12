#include <stdio.h>
#include <iostream>
#include <SerialStream.h>

#include "ranger-library.hh"

using namespace LibSerial;
using namespace std;

static SerialStream port;

void create_robot_connection(){
  port.Open("/dev/ttyUSB0");
  port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
  port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
  port.SetParity(LibSerial::Parity::PARITY_NONE);
  port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
}

void set_all_lights(int r, int g, int b){
  string command = "light " + to_string(r) + " " + to_string(g) + " " + to_string(b);
  port.write(command.c_str(), command.length());
  // readSerial();
}

void set_single_light(int index, int r, int g, int b){
  string command = "light " + to_string(index) + " " + to_string(r) + " " + to_string(g) + " " + to_string(b);
  port.write(command.c_str(), command.length());
  // readSerial();
}

void lights_off(){
  string command = "light_off";
  port.write(command.c_str(), command.length());
  // readSerial();
}

void single_light_off(int index){
  string command = "light_off " + to_string(index);
  port.write(command.c_str(), command.length());
}

void move_forward(){
  string command = "forward";
  port.write(command.c_str(), command.length());
}

void move_backward(){
  string command = "backward";
  port.write(command.c_str(), command.length());
}

void turn_left(){
  string command = "left";
  port.write(command.c_str(), command.length());
}

void turn_right(){
  string command = "right";
  port.write(command.c_str(), command.length());
}

void move_forward(int speed){
  string command = "forward " + to_string(speed);
  port.write(command.c_str(), command.length());
}

void move_backward(int speed){
  string command = "backward " + to_string(speed);
  port.write(command.c_str(), command.length());
}

void turn_left(int speed){
  string command = "left " + to_string(speed);
  port.write(command.c_str(), command.length());
}

void turn_right(int speed){
  string command = "right " + to_string(speed);
  port.write(command.c_str(), command.length());
}

void stop(){
  string command = "stop";
  port.write(command.c_str(), command.length());
}

void tank_drive(int rspeed, int lspeed){
  string command = "tank_drive " + to_string(rspeed) + " " + to_string(lspeed);
  port.write(command.c_str(), command.length());
}

void make_noise(int frequency, int duration){
  string command = "sound " + to_string(frequency) + " " + to_string(duration);
  port.write(command.c_str(), command.length());
}

double read_line_sensor(){
  string command = "read_line";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_light_sensor(){
  string command = "read_light";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_temperature_sensor(){
  string command = "read_temperature";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_sound_sensor(){
  string command = "read_sound";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_sonar_sensor(){
  string command = "read_sonar";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_gyroscope_x(){
  string command = "read_gyro_x";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_gyroscope_y(){
  string command = "read_gyro_y";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

double read_gyroscope_z(){
  string command = "read_gyro_z";
  port.write(command.c_str(), command.length());
  string response = read_serial();
  return stod(response);
}

std::string read_serial() {
  char temp[100];
  int ii = 0;
  for(; ii < 96; ++ii){
    port.read(temp + ii, 1);
    if(temp[ii] == '\n') {
      break;
    }
  }

  temp[ii+1] = 0;
  // cout << "Data: " << temp << endl;
  string data = temp;
  return data;

}
