// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup testNanoBot testNanoBot
 *
 * Creates an instance of NanoBot wrapped in a YARP controlboard. This means that it may be used
 * through a YARP remote_controlboard or directly through low-level YARP controlboard RPC commands.
 *
 * <b> Legal </b>
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses);
 *          Rosen Diankov (OpenRAVE sample code, email responses)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_testNanoBot is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] $ASIBOT_DIR/bin/testNanoBot
\endverbatim
 *
 * <b>Interfacing with the testNanoBot</b>
 *
 * The \ref testNanoBot module acts as the server part of a network wrapper of the NanoBot class.
 * The implementation maps certain YARP rpc's to NanoBot function calls. Therefore, we can interface
 * with the class from the command-line by typing:
\verbatim
[on terminal 3] yarp rpc /ravebot/rpc:i
\endverbatim
 * We can send an absolute position joint space movement such as: 
\verbatim
[on terminal 3] set poss (45.0 -80.455885 110.552447 59.903438 0.0)
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: [ok]
\endverbatim
 *
 * <b>Modify</b>
 * 
 * This file can be edited at 
 * $ASIBOT_DIR/main/src/modules/testNanoBot/main.cpp
 *
 */

#include "TestNanoBot.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(AsrobPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(AsrobPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("testNanoBot/conf");
    rf.setDefaultConfigFile("testNanoBot.ini");
    rf.configure("ECRO_ROOT", argc, argv);

    TestNanoBot mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"testNanoBot --help\" for options.\n");
    printf("testNanoBot checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\ntestNanoBot found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

