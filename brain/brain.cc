
#include <iostream>
#include <math.h>
#include <unistd.h>

#include "robot.hh"

using std::cout;
using std::endl;

int stateCount;
int state = 0;
float heading = 0.0;
float loudness = 0.0;
int loudest = 0;
int turn = 1; 
void
callback(Robot* robot)
{
    cout << robot->get_line_status() << endl;
    cout << "State " << state << endl;
    if(state == 1){
    	robot->set_vel(-0.7,0.7);
    	if(robot->get_line_status()==1||robot->get_line_status()==3){
    		if(fmod(heading-robot->get_robot_theta(),6.28)<0.5){
    			state = 0;
    		}else{
    			state = 2;
    		}
    	}
    }else if(state==2){
    	robot->set_vel(0.5,0.5);
    	sleep(1);
    	state = 3;
    }else if(state==3){
    	if(robot->get_noise_sensor()>loudness){
    		loudness = robot->get_noise_sensor();
    		loudest = turn;
    	}
    	robot->set_vel(-0.5,-0.5);
    	sleep(1);
    	if(turn==3){
    		turn = 1;
    		state = 6;
    		robot->set_vel(-0.7,0.7);
    	}else{
    		turn = turn+1;
    		state = 4;
    		robot->set_vel(0.7,-0.7);
    	}    	
    }else if(state==4){
    	if(robot->get_line_status()==0){
    		state = 5;
    	}
    }else if(state==5){
    	if(robot->get_line_status()==2||robot->get_line_status()==3){
    		state = 2;
    	}
    }else if(state==6){
    	if(abs(fmod(robot->get_robot_theta()-heading-1.57*loudest,6.28))<0.1){
    		state = 0;
    		loudest = 0;
    		loudness = 0.0;
    	}
    }else if(state==0){   
    	if(robot->get_line_status()==0){
    	 	heading = robot->get_robot_theta();
    	 	sleep(1);
    	 	state = 1;
    	}else if(robot->get_line_status()==1){
    		//If On Right Side Of Line Turn Left
    		robot->set_vel(1.0,1.5);
    	}else if(robot->get_line_status()==2){
    		//If On Left Side Of Line Turn Right
    		robot->set_vel(1.5,1.0);
    	}else{
    		//If On Line Go Straight
    		robot->set_vel(1.5,1.5);
    	}
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
