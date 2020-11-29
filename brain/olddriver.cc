if(robot->get_line_status() == 0){

    // Case: we're going in the right direction
    if (currentsound >= maxsound && lastdir != "backward") {
        robot->set_vel(2, 2);
        maxsound= currentsound;
    }

    // Case: we're going in the wrong "forward" direction
    else if (currentsound < maxsound  && lastdir == "forward" && lastsound != currentsound)
    {
        turning =  true ;
        if(abs(pos_t) > 3.0)
        {
            robot->set_vel(2, 2);
            lastdir = "backward";
            lastsound= currentsound;
        }
        else
        {
            robot->set_vel(-1.5, 1.5);
        }

    }

    // Case: we're going in the wrong "backward" direction
    else if(currentsound <= maxsound && lastdir == "backward" &&  lastsound != currentsound)
    {
        if(abs(pos_t) > 1.25 && abs(pos_t) < 2.2) {
            robot->set_vel(2, 2);
            lastdir = "left";
            lastsound = currentsound;

        }
        else
        {
            robot->set_vel(-1, 1);
        }


    }

    // Case: we're going in the wrong "left" direction (relative to start)
    else if(currentsound <= maxsound && lastdir == "left" && lastsound != currentsound)
    {
        if(pos_t < -1.25 && pos_t > -2.3)
        {
            robot->set_vel(2, 2);
            lastdir = "right";
            lastsound= currentsound;

        }
        else {
            robot->set_vel(-1, 1);
        }
    }

    // Case: we're going in the wrong "right" direction (relative to start)
    else if (currentsound < maxsound  && lastdir == "right" && lastsound != currentsound )
    {
        turning =  true ;
        if (pos_t > 1.5)
        {
            robot->set_vel(2, 2);
            lastdir = "backward";
            lastsound = currentsound;

        }
        else
        {
            robot->set_vel(-1.5, 1.5);
        }
    }
    else
    {
        robot->set_vel(2, 2);

    }
}
