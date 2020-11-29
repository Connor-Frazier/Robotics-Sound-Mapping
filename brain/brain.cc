
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
#define wind 15
using std::cout;
using std::endl;
using namespace std;

map<pair<int, int>,int>occgrid;

int maxsound = 0;
bool turning = false ;
string lastdir = "forward";
int lastsound = 0;
double pos_t;
double pos_x;
double pos_y;
int spacecounter = 0;

void  mapviz(int x, int y, int sound ){
    gfx_color(0 , 255  /*sound*4.25*/ , 0);
    for(int t = x*10-5; t <= x*10+5; t++ ){
        for(int z = y*10-5; z<= y*10+5; z++ ){
            gfx_point(z,t);

        }
    }
}
int tcounter = 0;

int ind= 0;
int val = 0;
int su = 0;
int store[wind];
int aved = 0;



    void
callback(Robot* robot)
{

    pos_t = robot->get_robot_theta();
    pos_x = robot->get_robot_x();
    pos_y = robot->get_robot_y();

    int intx = round(pos_x);
    int inty = round(pos_y);
    int currentsound = robot->get_noise_sensor();
    mapviz(round(pos_x),round(pos_y),currentsound);

    //Occupancy grid contain current sound heuristic.
    occgrid.insert(make_pair(make_pair(intx,inty),currentsound));

    // Sound Sensor Moving Average Logic
    su = su - store[ind];
    val = currentsound;
    store[ind] = val;
    su = su + val;
    ind = (ind+1) % wind;

    currentsound = su / wind;
    int linestatus = robot->get_line_status();

    cout << "Line Status " << linestatus <<" Current Sound " << currentsound << " Max Sound " << maxsound<<   " Pos_t " << pos_t <<
    " Last Dir " << lastdir << endl;

    /*Movement Logic (Line Sensors):
    Line Status == 0, robot is in intersection.
    Line Status == 1, robot is on line edge.
    Line Status  == 2, robot is on line edge.
    Line Statis ==3, robot is directly above line.
    Movement Logic (Sound):
    If the sound is getting louder or the sound is equal to the loudest it has been go forward at the intersection.
    If the sound is getting quieter turn or trun around at the next intersection depending on the previous action.
    */

    if(linestatus==0){

        if (currentsound >= maxsound) {
            robot->set_vel(3, 3);
            maxsound= currentsound;
        } else if (currentsound < maxsound  && lastdir == "forward" && lastsound != currentsound) {
            turning =  true ;
            if(abs(pos_t) > 2.4){
                spacecounter = 0;
                robot->set_vel(3, 3);
                lastdir = "backward";
            } else {
                if (spacecounter ==0) {
                    robot->set_vel(3, 3);
                    spacecounter++;

                } else {
                    robot->set_vel(4, -3.5);
                }
            }
        } else if(currentsound <= maxsound && lastdir == "backward" &&  lastsound != currentsound ) {
            if(pos_t > 1.4 && pos_t < 1.7) {
                robot->set_vel(3, 3);
                lastdir = "left";
                spacecounter = 0;
            } else {
                if (spacecounter <=0) {
                    robot->set_vel(3, 3);
                    spacecounter++;

                } else {
                    robot->set_vel(3.5,-3.5);
                }
            }
        } else if(currentsound <= maxsound && lastdir == "left" && lastsound != currentsound) {
            if(pos_t < -1.25 && pos_t > -1.75) {
                robot->set_vel(3, 3);
                lastdir = "right";
                spacecounter = 0;
            } else{
                if (spacecounter <=1) {
                    robot->set_vel(3, 3);
                    spacecounter++;
                } else {
                    robot->set_vel(-3.5, 3.5);
                }
            }
        } else if (currentsound < maxsound  && lastdir == "right" && lastsound != currentsound ) {
            turning =  true ;
            if(abs(pos_t) < 1){
                robot->set_vel(3, 3);
                lastdir = "backward";
            } else{
                robot->set_vel(-3.5,3.5);
            }
        } else {
            cout << "Going Straight No Condition Met" <<endl;
            robot->set_vel(3 , 3);
        }
    } else if(linestatus==1){
        //If On Right Side Of Line Turn Left
        robot->set_vel(-1.5,2);
    } else if(linestatus==2){
        //If On Left Side Of Line Turn Right
        robot->set_vel(3,-2.5);
    } else{
        //If On Line Go Straight
        cout << "Going Straight On Line" <<endl;
        robot->set_vel(3,3);
    } 

    return;
}

int
main(int argc, char* argv[])
{
    Robot* robot = 0;

    std::string bname(basename(argv[0]));
    std::cout << "bin: [" << bname << "]" << endl;
    int ysize = 1000;
    int xsize = 1000;
    char c;

    gfx_open(xsize,ysize,"Sound Localization");

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
        if(c=='q') break;}
    delete robot;
    return 0;
}
