/*
 * Copyright: (C) 2010 RobotCub Consortium
 * Author: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "RdMacros.hpp"
#include "RunBot.hpp"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(PiYarp);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(PiYarp);

    ResourceFinder rf;
    rf.setVerbose(false);
    rf.setDefaultContext("runBot/conf");
    rf.setDefaultConfigFile("runBot.ini");
    rf.configure("PI_YARP_ROOT", argc, argv);

    RD_INFO("Checking for yarp network...\n");
    Network yarp;
    if (!yarp.checkNetwork()) {
        RD_ERROR("Found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    }
    RD_SUCCESS("Found yarp network.\n");

    RunBot mod;
    return mod.runModule(rf);
}

