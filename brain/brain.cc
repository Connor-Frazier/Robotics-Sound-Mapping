
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
map<pair<int, int>,vector<int>>intersections;
pair<int, int> lastIntersection = make_pair(-2, 3); // this is where we start

int maxsound = 0;
bool turning = false ;
string lastdir = "forward";
int lastsound = 0;
int lastchoice = 0;
bool online = true;
bool firstIter = true;
double pos_t;
int stateCount;
double pos_x;
double pos_y;

void mapviz(int x, int y, int sound ){
	//cout << "Map Viz "<<y<< "," <<x << endl;
	gfx_color(0 , 255  /*sound*4.25*/ , 0);
	for(int t = x*10-5; t <= x*10+5; t++ ){
		for(int z = y*10-5; z<= y*10+5; z++ ){
			gfx_point(z+200,t+200);
	//		cout << "Map Viz "<<y<< "," <<x << endl;

		}
	}
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
	if (firstIter)
	{
		vector<int> first = {0,0,0,0};
		intersections.insert(make_pair(lastIntersection, first));
		firstIter = false;
	}
	//cout << robot->get_noise_sensor() <<" "  << robot->get_robot_theta() << " "<< lastdir <<  endl;
	pos_t = robot->get_robot_theta();
	pos_x = robot->get_robot_x();
	pos_y = robot->get_robot_y();
	//cout << pos_x <<" "  << pos_y << " xy working?  "<< lastdir <<  endl;
	//mapviz (round(pos_x),round(pos_y));

	int intx = round(pos_x);
	int inty = round(pos_y);
	int currentsound = robot->get_noise_sensor();
	mapviz(round(pos_x),round(pos_y),currentsound);
	occgrid.insert(make_pair(make_pair(intx,inty),currentsound));


	SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
	VALUE = currentsound;        // Read the next sensor value
	READINGS[INDEX] = VALUE;           // Add the newest reading to the window
  	SUM = SUM + VALUE;                 // Add the newest reading to the sum
  	INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  	currentsound = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
	if (currentsound > maxsound)
	{
		maxsound = currentsound;
	}

 	//cout << "Line state: " << robot->get_line_status() << " || Current sound: " << currentsound << " || Last sound: " << lastsound << " || Max sound: " << maxsound <<  " || Heading: " << pos_t << " || Position: (" << intx << ", " << inty << ")" << endl;


	if(robot->get_line_status() == 0){

		pair<int, int> position = make_pair(intx, inty);
		//cout << "Position: " << intx << ", " << inty << endl;

		if (intersections.find(position) == intersections.end())
		{
			// not found
			cout << "New intersection: " << position.first << ", " << position.second << endl;
			vector<int> dirs = {0, 0, 0, 0};
			intersections.insert(make_pair(position, dirs));
		}

		// handle relative sound logic ONLY when we are NEWLY off a line
		if (online && pos_t != 0)
		{
			if (intx == 0 && inty == 0)
			{
				return;
			}
			cout << "Updating past line" << endl;
			cout << "Line state: " << robot->get_line_status() << " || Current sound: " << currentsound << " || Last sound: " << lastsound << " || Max sound: " << maxsound <<  " || Heading: " << pos_t << " || Position: (" << intx << ", " << inty << ")" << endl;
			cout << "Position: " << intx << ", " << inty << endl;

			vector<int> last = intersections[lastIntersection];
			vector<int> curr = intersections[position];
			int score;
			if (currentsound < lastsound)
			{
				// Case: we've gone in the wrong direction
				score = -1;
			}
			else if (currentsound > lastsound)
			{
				// Case: we've gone in the right direction
				score = 1;
			}
			else
			{
				// Case: error case, really, no change in sound
				score = 0;
			}
			last[lastchoice] = score;
			intersections[lastIntersection] = last;

			// Choose how to update the current location
			int currscore = score * -1;
			if (lastchoice == 0)
			{
				// Case: update down from curr
				curr[1] = currscore;
			}
			else if (lastchoice == 1)
			{
				// Case: update up of curr
				curr[0] = currscore;
			}
			else if (lastchoice == 2)
			{
				// Case: update right of curr
				curr[3] = currscore;
			}
			else if (lastchoice == 3)
			{
				// Case: update left of curr
				curr[2] = currscore;
			}
			intersections[position] = curr;

			cout << "Last intersection: " << lastIntersection.first << ", " << lastIntersection.second << endl;
			cout << "Lastchoice: " << lastchoice << endl;
			cout << "Updated last: " << intersections[lastIntersection][0] << intersections[lastIntersection][1] << intersections[lastIntersection][2] << intersections[lastIntersection][3] << endl;

			cout << "Updated curr: " << intersections[position][0] << intersections[position][1] << intersections[position][2] << intersections[position][3] << endl;
			cout << "==================================" << endl;
		}
		online = false;
		lastIntersection = position;

		// Now handle intersection logic
		vector<int> dirs = intersections[position];
		int up = dirs[0];
		int down = dirs[1];
		int left = dirs[2];
		int right = dirs[3];

		//cout << "Directions: " << up << down << left << right << endl;

		if (up >= down && up >= left && up >= right)
		{
			// Case: up is the best guess direction
			//cout << "Going UP" << endl;
			if (pos_t < 0.1 && pos_t > -0.1)
			{
				// Case: we're at the right heading
				robot->set_vel(1.5, 1.5);
			}
			else
			{
				// Case: we're not at the right heading
				robot->set_vel(-1.5, 1.5);
			}
			lastIntersection = position;
			lastsound = currentsound;
			lastchoice = 0;
		}
		else if (down >= up && down >= left && down >= right)
		{
			// Case: down is the best guess direction
			// Case: up is the best guess direction
			//cout << "Going UP" << endl;
			if (abs(pos_t) > 3)
			{
				// Case: we're at the right heading
				robot->set_vel(1.5, 1.5);
			}
			else
			{
				// Case: we're not at the right heading
				robot->set_vel(-1.5, 1.5);
			}
			lastIntersection = position;
			lastsound = currentsound;
			lastchoice = 1;
		}
		/*
		else if ()
		{
			// Case: left is the best guess direction
		}
		else if ()
		{
			// Case: right is the best guess direction
		}
		*/

	}
	else if (robot->get_line_status() == 1)
	{
		// 1 is the left sensor if facing forward
		online = true;
		robot->set_vel(-1.5, 1.5);
	}
	else if(robot->get_line_status() == 2)
	{
		online = true;
		// 2 is the right sensor if facing forward
		robot->set_vel(1.5, -1.5);
	}
	else
	{
		//If On Line Go Straight
		online = true;
		robot->set_vel(1.0, 1.0);
	}

	//cout << "==================================" << endl;
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

	gfx_open(xsize,ysize,"Example Graphics Program");

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

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c=='q') break;
	}

	delete robot;
	return 0;
}
