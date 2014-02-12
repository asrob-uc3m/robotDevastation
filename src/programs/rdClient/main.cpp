// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup rd_programs
 * \defgroup rdClient rdClient
 *
 * @brief The Robot Devastation PC client.
 * It can either connect to a PC webcam, or to a YARP image stream.
 * The current interface is made with Highgui, as OpenCV is used for face detection.
 * For robot wheel control, rdClient can either connect to a running instance of a YARP
 * controlboard or create its own fake one for developing. Simulators will also expose controlboard
 * interfaces in the future.
 *
 * @section legal Legal
 *
 * Copyright: ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid (http://asrob.uc3m.es)
 * Robot Devastation project
 *
 * Authors:
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
rdColient
\endverbatim
 *
 * <b>OPTIONAL:</b> If you prefer to connect to a real robot, for example the ECRO, indicate it with the robot parameter:
\verbatim
rdClient --robot ECRO
\endverbatim
 *
 * @section interfacing Interfacing with robotDevastation
 *
 * Press 'space' to fire! Use <b>r</b> for reload, and move using <b>w</b>, <b>a</b>, <b>s</b> and <b>d</b>.
 *
 * @section modify Modify
 * 
 * This file can be edited at 
 * $RD_ROOT/src/modules/robotDevastation/main.cpp
 *
 */

#include <iostream>

#include "RdClient.hpp"

int main(int argc, char *argv[]) {

    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "Robot Devastation @ ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid" << std::endl;
    std::cout << "Welcome to Robot Devastation v0.1, developed by Juan G Victores and Santiago Morante." << std::endl;
    std::cout << " ____       _           _   ____                      _        _   _             " << std::endl;
    std::cout << "|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  " << std::endl;
    std::cout << "| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ " << std::endl;
    std::cout << "|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |" << std::endl;
    std::cout << "|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|" << std::endl;
    std::cout << std::endl;
    std::cout << "Fire with 'space'. Reload with 'r'. Move with 'a,s,d,w,x'. Run \"robotDevastation --help\" for options." << std::endl;
    std::cout << "For a full description, please visit http://asrob.uc3m.es/rddoc/group__robotDevastation.html." << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;

    yarp::os::ResourceFinder rf;  // ResourceFinder is a YARP helper class we use for parsing command line arguments and conf file.
    rf.setVerbose(false);
    rf.setDefaultContext("rdClient/conf");
    rf.setDefaultConfigFile("rdClient.ini");
    rf.configure("RD_ROOT", argc, argv);

    yarp::os::Network yarp;  // Must create this object in order to connect to the YARP network.

    if(rf.check("robot") && !rf.check("help")) {  // Unless we have no robot, or just asking for help, we'll need to check connectivity.
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "rdClient checking for yarp network... ";
        fflush(stdout);
        if (!yarp.checkNetwork()) {
            std::cerr << "[fail]" << std::endl
                 << "rdClient found no yarp network (try running \"yarpserver &\"), bye!" << std::endl;
            return -1;
        } else std::cout << "[ok]" << std::endl;
    }

    rdclient::RdClient mod;  // This is the rdClient program main class.
    return mod.runModule(rf);  // JUST RUN THE MAIN CLASS (blocking function).
}
