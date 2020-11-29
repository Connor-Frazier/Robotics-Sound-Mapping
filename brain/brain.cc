
#include <iostream>
#include <math.h>
#include <string>
#include <unistd.h>
#include <iterator>
#include <map>
#include "robot.hh"
extern "C"{
#include "gfx.c"
}
#define WINDOW_SIZE 15
using std::cout;
using std::endl;
using namespace std;

map<pair<int, int>,int>occgrid;

int maxsound = 0;
bool turning = false ;
string lastdir = "forward";
int lastsound = 0;
double pos_t;
int stateCount;
double pos_x;
double pos_y;
int spacecounter = 0;

void  mapviz(int x, int y, int sound ){
//	cout << "Map Viz "<<y<< "," <<x << endl;
	gfx_color(0 , 255  /*sound*4.25*/ , 0);
	for(int t = x*10-5; t <= x*10+5; t++ ){
		for(int z = y*10-5; z<= y*10+5; z++ ){
			gfx_point(z,t);
	//		cout << "Map Viz "<<y<< "," <<x << endl;

		}}
}
int tcounter = 0;

int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;



	void
callback(Robot* robot)
{

	pos_t = robot->get_robot_theta();
	pos_x = robot->get_robot_x();
	pos_y = robot->get_robot_y();

	int intx = round(pos_x);
	int inty = round(pos_y);
	int currentsound = robot->get_noise_sensor();
/*	mapviz(round(pos_x),round(pos_y),currentsound);
	occgrid.insert(make_pair(make_pair(intx,inty),currentsound));
*/

	SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  	VALUE = currentsound;        // Read the next sensor value
  	READINGS[INDEX] = VALUE;           // Add the newest reading to the window
  	SUM = SUM + VALUE;                 // Add the newest reading to the sum
  	INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  	currentsound = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
 	int linestatus = robot->get_line_status();

	cout << "Line Status " << linestatus <<" Current Sound " << currentsound << " Max Sound " << maxsound<<   " Pos_t " << pos_t <<
	" Last Dir " << lastdir << endl;

	if(linestatus==0){

    if (currentsound >= maxsound) {
			 robot->set_vel(3, 3);
			 maxsound= currentsound;
		}

		else if (currentsound < maxsound  && lastdir == "forward" && lastsound != currentsound  ) {
			turning =  true ;
			if(abs(pos_t - 3.14) > 0.2){
				robot->set_vel(3, 3);
				lastdir = "backward";
			}
			else{
				robot->set_vel(3, -3);
			}

		}
		else if(currentsound <= maxsound && lastdir == "backward" &&  lastsound != currentsound ) {
			if(abs(pos_t - 1.57) > 0.2) {
				robot->set_vel(3, 3);
				lastdir = "left";
			}
			else{
				robot->set_vel(3, -3);
			}
		}
		else if(currentsound <= maxsound && lastdir == "left" && lastsound != currentsound) {
			if(abs(pos_t - -1.57) > 0.2) {
				robot->set_vel(3, 3);
				lastdir = "right";
			}
			else{
				robot->set_vel(3, -3);
			}
		}
		else if (currentsound < maxsound  && lastdir == "right" && lastsound != currentsound ) {
			turning =  true ;
			if(abs(pos_t - 0) > 0.2){
				robot->set_vel(3, 3);
				lastdir = "backward";
			//	lastsound= currentsound;

			}
			else{
				robot->set_vel(-3,3);
			}

		}
		else {
			cout << "Going Straight No Condition Met" <<endl;
			robot->set_vel(3 , 3);
		}
	}
    	else if(linestatus==1){
    		//If On Right Side Of Line Turn Left
    		robot->set_vel(-1.5,2);
    	}else if(linestatus==2){
    		//If On Left Side Of Line Turn Right
    		robot->set_vel(2,-1.5);
    	}else{
    		//If On Line Go Straight
		cout << "Going Straight On Line" <<endl;
    		robot->set_vel(3,3);

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
	int ysize = 1000;
	int xsize = 1000;
	char c;

/*	gfx_open(xsize,ysize,"Example Graphics Program");*/

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
	/*while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;}*/
	delete robot;
	return 0;
}
