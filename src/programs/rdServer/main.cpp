// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup rd_programs
 * \defgroup rdServer rdServer
 *
 * @brief The Robot Devastation PC server. Creates an instance of the RdServer class.
 *
 * @section legal Legal
 *
 * Copyright: ASROB 2012-2014 (C) Robotics Society of the Universidad Carlos III de Madrid (http://asrob.uc3m.es)
 * Robot Devastation project
 *
 * Authors:
 * David Estevez,
 * <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>,
 * <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a>.
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
 * @section interfacing Interfacing with rdServer
 *
 * @section modify Modify
 * 
 * This file can be edited at 
 * $RD_ROOT/src/programs/rdServer/main.cpp
 *
 */

#include <iostream>

#include <yarp/os/Network.h>

#include "RdMacros.hpp"
#include "RdServer.hpp"

int main(int argc, char *argv[]) {

    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "Robot Devastation @ ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid" << std::endl;
    std::cout << "Welcome to Robot Devastation v0.2, developed by David Estevez and Juan G Victores." << std::endl;
    std::cout << "Based on Robot Devastation v0.1, developed by Santiago Morante and Juan G Victores." << std::endl;
    printf(GREEN);
    std::cout << " ____       _           _   ____                      _        _   _             " << std::endl;
    std::cout << "|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  " << std::endl;
    std::cout << "| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ " << std::endl;
    std::cout << "|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |" << std::endl;
    std::cout << "|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|" << std::endl;
    printf(RESET);
    std::cout << std::endl;
    std::cout << "Fire with 'space'. Reload with 'r'. Move with 'a,s,d,w,x'. Run \"robotDevastation --help\" for options." << std::endl;
    std::cout << "For a full description, please visit http://asrob.uc3m.es/rddoc/group__robotDevastation.html." << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;

    yarp::os::ResourceFinder rf;
    rf.setVerbose(false);
    rf.setDefaultContext("robotDevastation/conf");
    rf.setDefaultConfigFile("robotDevastation.ini");
    rf.configure("RD_ROOT", argc, argv);

    rd::RdServer rdServer;
    if(rf.check("help")) {
        return rdServer.runModule(rf);
    }

    printf("Run \"%s --help\" for options.\n",argv[0]);
    printf("%s checking for yarp network... ",argv[0]);
    fflush(stdout);
    yarp::os::Network yarp;
    if (!yarp.checkNetwork()) {
        fprintf(stderr,"[fail]\n%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return -1;
    } else printf("[ok]\n");

    return rdServer.runModule(rf);

}

