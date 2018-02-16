// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/**
 * 
 * @ingroup rd_programs
 * \defgroup rdServer rdServer
 *
 * @brief The Robot Devastation PC server. Creates an instance of the rd::Server class.
 *
 * @section rd_server_installation Installation
 *
 * The module is compiled when ENABLE_rdServer is activated (default: ON). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section rd_server_running Running (assuming correct installation, see previous section)
 * 
 * It should be straightforward to run the default mode. Just click open a terminal and write:
 *
\verbatim
rdServer
\endverbatim
 *
 * @section rd_server_modify Modify
 * 
 * This file can be edited at 
 * $RD_ROOT/src/programs/rdServer/main.cpp
 *
 */

#include <cstdio>
#include <iostream>

#include <yarp/os/Network.h>
#include <yarp/os/ResourceFinder.h>

#include <ColorDebug.hpp>

#include "Server.hpp"

int main(int argc, char *argv[]) {

    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "Robot Devastation @ ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid" << std::endl;
    std::cout << "Welcome to Robot Devastation v0.2, developed by David Estevez and Juan G Victores." << std::endl;
    std::cout << "Based on Robot Devastation v0.1, developed by Santiago Morante and Juan G Victores." << std::endl;
    std::printf(GREEN);
    std::cout << " ____       _           _   ____                      _        _   _             " << std::endl;
    std::cout << "|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  " << std::endl;
    std::cout << "| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ " << std::endl;
    std::cout << "|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |" << std::endl;
    std::cout << "|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|" << std::endl;
    std::printf(RESET);
    std::cout << std::endl;
    std::cout << "Fire with 'space'. Reload with 'r'. Move with 'a,s,d,w,x'. Run \"robotDevastation --help\" for options." << std::endl;
    std::cout << "For a full description, please visit http://asrob.uc3m.es/rddoc/group__robotDevastation.html." << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;

    yarp::os::ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("rdServer");
    rf.setDefaultConfigFile("rdServer.ini");
    rf.configure(argc, argv);

    rd::Server rdServer;
    if(rf.check("help")) {
        std::printf("RobotDevastation server optional parameters:\n");
        std::printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        std::printf("\t--quiet  //-- Suppress all console output\n");
        return 0;
    }

    std::printf("Run \"%s --help\" for options.\n",argv[0]);
    std::printf("%s checking for yarp network... ",argv[0]);
    std::fflush(stdout);
    yarp::os::Network yarp;
    if (!yarp.checkNetwork()) {
        std::fprintf(stderr,"[fail]\n%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return 1;
    } else printf("[ok]\n");

    return rdServer.runModule(rf);

}
