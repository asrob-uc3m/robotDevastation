// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RobotDevastation.hpp"

/************************************************************************/
bool RobotDevastation::configure(ResourceFinder &rf)
{

    //printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RobotDevastation options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("VisionSegmentor using no additional special options.\n");

    segmentorThread.setInImg(&inImg);
    segmentorThread.setOutImg(&outImg);
    segmentorThread.setOutPort(&outPort);

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inImg.open("/img:i");
    outImg.open("/img:o");
    outPort.open("/features:o");

    return true;
}

/*****************************************************************/
double RobotDevastation::getPeriod()
{
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/

bool RobotDevastation::updateModule()
{
    printf("RobotDevastation alive...\n");
    return true;
}

/************************************************************************/

bool RobotDevastation::interruptModule() {
    printf("RobotDevastation closing...\n");
    //outPort.interrupt();
    //inImg.interrupt();
    //outPort.close();
    //inImg.close();
    return true;
}

/************************************************************************/

