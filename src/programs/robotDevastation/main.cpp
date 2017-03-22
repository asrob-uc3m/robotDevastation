// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup rd_programs
 * \defgroup robotDevastation robotDevastation
 *
 * @brief The Robot Devastation PC client. Creates an instance of the RobotDevastation class.
 *
 * @section legal Legal
 *
 * Copyright: ASROB 2012-2014 (C) Robotics Society of the Universidad Carlos III de Madrid (http://asrob.uc3m.es)
 * Robot Devastation project
 *
 * Authors:
 * <a href="https://plus.google.com/+davidestevezfernandez">David Estevez</a> (2014-present),
 * <a href="http://roboticslab.uc3m.es/roboticslab/people/jg-victores">Juan G. Victores</a> (2012-present),
 * <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a> (2012-2014).
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see $RD_ROOT/main/doc/LGPL.TXT
 *
 * @section installation Installation
 *
 * The module is compiled when ENABLE_robotDevastation is activated (default: ON). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section running Running (assuming correct installation, see previous section)
 * 
 * It should be straightforward to run the default test mode. Just click on the icon or open a terminal and write:
 *
\verbatim
rdClient
\endverbatim
 *
 * @section interfacing Interfacing with robotDevastation
 *
 * Press 'space' to shoot!
 *
 * @section modify Modify
 * 
 * This file can be edited at 
 * $RD_ROOT/src/programs/robotDevastation/main.cpp
 *
 */

#include <iostream>

#include <yarp/os/Network.h>

#include "Utils.hpp"
#include "RobotDevastation.hpp"

int main(int argc, char *argv[]) {

    printf("--------------------------------------------------------------\n");
    printf("Robot Devastation @ ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid\n");
    printf("Welcome to Robot Devastation v0.2, developed by David Estevez and Juan G Victores.\n");
    printf("Based on Robot Devastation v0.1, developed by Santiago Morante and Juan G Victores.\n");
    printf(GREEN);
    printf(" ____       _           _   ____                      _        _   _             \n");
    printf("|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  \n");
    printf("| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ \n");
    printf("|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |\n");
    printf("|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|\n");
    printf(RESET);
    printf("\n");
    printf("Fire with 'space'. Reload with 'r'. Move with 'Left, Up, Down, Right'. Run \"robotDevastation --help\" for help.\n");
    printf("For a full description, please visit http://asrob.uc3m.es/rddoc/group__robotDevastation.html.\n");
    printf("--------------------------------------------------------------\n");

    yarp::os::ResourceFinder rf;
    rf.setVerbose(false);
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");
    rf.configure(argc, argv);

    if(rf.check("help"))
    {
        printf("RobotDevastation optional parameters:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--mockupRobotManager  //-- Fake robot motors\n");
        printf("\t--mockupImageManager  //-- Fake robot camera\n");
        printf("\t--yarpLocalImageManager  //-- Local webcam as camera\n");
        printf("\t--fullscreen //-- Fullscreen mode (experimental)\n");
        return 0;
    }

    rd::RobotDevastation robotDevastation;
    return robotDevastation.runModule(rf);  //-- Internally calls rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)

}

