
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

    //cout << robot->get_noise_sensor() << endl;

    //cout << robot->get_robot_theta() << endl;

    //robot->set_vel(-5.0, 5.0);

    /*stateCount++;
    std::cout << "state count: " << stateCount << '\n';
    if(stateCount < 3){
         robot->set_vel(1.5, 1.5);
    } else if(stateCount < 6){
      robot->set_vel(-1.5, -1.5);
    } else if(stateCount > 6 ){
      stateCount = 0;
      robot->set_vel(-1.5, 1.5);
    }*/
    if(robot->get_line_status()==0){
    	//Engage Test Procedure If Not On Line
    	//Record Start Direction
    	float heading = robot->get_robot_theta();
    	int test = robot->get_line_status();
    	//Turn Left Until Hit Another Line
    	while(test==0){
    		//****************ERROR OCCURS HERE, ROBOT STARTS BY LOOPING THIS FOREVER WITHOUT TURNING
    		robot->set_vel(-1.0,1.0);
    		test = robot->get_line_status();
    		cout << "Turn Left " << test << endl;
    	}
    	float loudness = 0.0;
    	int loudest = 0;
    	//Test Procedure for Each Possible Branch
    	for(int i=1;i<4;i++){
    		//Go Forward
    		robot->set_vel(0.5,0.5);
    		sleep(1);
    		//Check If Noise Down This Path Is Loudest Available So Far
    		if(robot->get_noise_sensor()>loudness){
    			loudness = robot->get_noise_sensor();
    			loudest = i;
    		}
    		//Return To Start of Test
    		robot->set_vel(-0.5,-0.5);
    		sleep(1);
    		//If Not Testing Last Leg, Turn To Next Leg
    		if(i!=3){
    			//Turn Right Until Off Current Line
    			if(robot->get_line_status()==1){
    				while(robot->get_line_status()!=0){
    					robot->set_vel(1.0,-1.0);
    				}
    			}
    			//Turn Right Until On Next Line
    			while(robot->get_line_status()!=2){
    				robot->set_vel(1.0,-1.0);
    			}
    		}
    	}
	//If No Other Direction Return Back (I don't think this is currently right)
    	if(loudest==0){
    		while(abs(robot->get_robot_theta()-heading)>0.1){
    			robot->set_vel(1.0,-1.0);
    		}
    	} else {
    	//Turn To Loudest Direction
    		while(abs(fmod(robot->get_robot_theta()-heading-1.57*loudest,6.28))>0.1){
    			robot->set_vel(1.0,-1.0);
    		}    		
    	}
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
