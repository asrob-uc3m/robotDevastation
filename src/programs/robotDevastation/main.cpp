// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/**
 * 
 * @ingroup rd_programs
 * \defgroup robotDevastation robotDevastation
 *
 * @brief The Robot Devastation PC client. Creates an instance of the rd::RobotDevastation class.
 *
 * @section rd_robot_devastation_installation Installation
 *
 * The module is compiled when ENABLE_robotDevastation is activated (default: ON). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section rd_robot_devastation_running Running (assuming correct installation, see previous section)
 * 
 * It should be straightforward to run the default test mode. Just click on the icon or open a terminal and write:
 *
\verbatim
robotDevastation
\endverbatim
 *
 * @section rd_robot_devastation_interfacing Interfacing with robotDevastation
 *
 * Press 'space' to shoot!
 *
 * @section rd_robot_devastation_modify Modify
 * 
 * This file can be edited at 
 * $RD_ROOT/src/programs/robotDevastation/main.cpp
 *
 */

#include <cstdio>
#include <iostream>

#include <yarp/os/Network.h>
#include <yarp/os/ResourceFinder.h>

#include <ColorDebug.h>

#include "RobotDevastation.hpp"

int main(int argc, char *argv[])
{
    std::printf("--------------------------------------------------------------\n");
    std::printf("Robot Devastation @ ASROB 2019 (C) Robotics Society of the Universidad Carlos III de Madrid\n");
    std::printf("Welcome to Robot Devastation v0.2, developed by David Estevez and Juan G Victores.\n");
    std::printf("Based on Robot Devastation v0.1, developed by Santiago Morante and Juan G Victores.\n");
    std::printf(CD_GREEN);
    std::printf(" ____       _           _   ____                      _        _   _             \n");
    std::printf("|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  \n");
    std::printf("| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ \n");
    std::printf("|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |\n");
    std::printf("|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|\n");
    std::printf(CD_RESET);
    std::printf("\n");
    std::printf("Fire with 'space'. Reload with 'r'. Move with 'Left, Up, Down, Right'. Run \"robotDevastation --help\" for help.\n");
    std::printf("For a full description, please visit http://wiki.asrob.uc3m.es/rddoc/group__robotDevastation.html.\n");
    std::printf("--------------------------------------------------------------\n");

    yarp::os::ResourceFinder rf;
    rf.setVerbose(false);
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");
    rf.configure(argc, argv);

    if(rf.check("help"))
    {
        std::printf("RobotDevastation optional parameters:\n");
        std::printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        std::printf("\t--fakeRobotManager  //-- Fake robot motors\n");
        std::printf("\t--fakeImageManager  //-- Fake robot camera\n");
        std::printf("\t--yarpLocalImageManager  //-- Local webcam as camera\n");
        std::printf("\t--fullscreen //-- Fullscreen mode (experimental)\n");
        return 0;
    }

    CD_INFO_NO_HEADER("Checking for yarp network... ");
    fflush(stdout);
    yarp::os::Network yarp;
    if ( ! yarp::os::Network::checkNetwork() )
    {
        CD_ERROR_NO_HEADER("[fail]\n");
        CD_INFO_NO_HEADER("%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return 1;
    }
    CD_SUCCESS_NO_HEADER("[ok]\n");

    rd::RobotDevastation robotDevastation;
    return robotDevastation.runModule(rf);  //-- Internally calls rd::RobotDevastation::configure(yarp::os::ResourceFinder &rf)

}
