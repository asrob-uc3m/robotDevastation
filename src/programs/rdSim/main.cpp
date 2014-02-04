// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * @ingroup rd_programs
 * \defgroup rdSim rdSim
 *
 * @brief The Robot Devastation PC simulated environment. It is currently based on OpenRAVE.
 *
 * @section rdSim_legal Legal
 *
 * Copyright: 2014 (C) ASROB @ Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses); Rosen Diankov (OpenRAVE)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * @section rdSim_install Installation
 *
 * The module is compiled when ENABLE_rdSim is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section ecroRave_running Running (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] $RD_DIR/bin/rdSim
\endverbatim
 *
 * <hr>
 *
 * This file can be edited at $RD_ROOT/src/modules/rdSim/main.cpp
 *
 */

#include "RdSim.hpp"

//YARP_DECLARE_PLUGINS(RdPlugins);

int main(int argc, char *argv[]) {

    //YARP_REGISTER_PLUGINS(RdPlugins);

    cout << "--------------------------------------------------------------" << endl;
    cout << "Robot Devastation @ ASROB 2014 (C) Robotics Society of the Universidad Carlos III de Madrid" << endl;
    cout << "Welcome to Robot Devastation v0.1, developed by Juan G Victores and Santiago Morante." << endl;
    cout << " ____       _           _   ____                      _        _   _             " << endl;
    cout << "|  _ \\ ___ | |__   ___ | |_|  _ \\  _____   ____ _ ___| |_ __ _| |_(_) ___  _ __  " << endl;
    cout << "| |_) / _ \\| '_ \\ / _ \\| __| | | |/ _ \\ \\ / / _` / __| __/ _` | __| |/ _ \\| '_ \\ " << endl;
    cout << "|  _ < (_) | |_) | (_) | |_| |_| |  __/\\ V / (_| \\__ \\ || (_| | |_| | (_) | | | |" << endl;
    cout << "|_| \\_\\___/|_.__/ \\___/ \\__|____/ \\___| \\_/ \\__,_|___/\\__\\__,_|\\__|_|\\___/|_| |_|" << endl;
    cout << endl;
    cout << "Fire with 'space'. Reload with 'r'. Move with 'a,s,d,w,x'. Run \"robotDevastation --help\" for options." << endl;
    cout << "For a full description, please visit http://asrob.uc3m.es/rddoc/group__robotDevastation.html." << endl;
    cout << "--------------------------------------------------------------" << endl;

    yarp::os::ResourceFinder rf;  // ResourceFinder is a YARP helper class we use for parsing command line arguments and conf file.
    rf.setVerbose(true);
    rf.setDefaultContext("rdSim/conf");
    rf.setDefaultConfigFile("rdSim.ini");
    rf.configure("RD_ROOT", argc, argv);

    rdsim::RdSim mod;  // This is the rdSim program main class.
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"rdSim --help\" for options.\n");
    printf("rdSim checking for yarp network... ");
    fflush(stdout);
    yarp::os::Network yarp;  // Must create this object in order to connect to the YARP network.
    if (!yarp.checkNetwork()) {
        printf("[fail]\nrdSim found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);  // JUST RUN THE MAIN CLASS (blocking function).
}

