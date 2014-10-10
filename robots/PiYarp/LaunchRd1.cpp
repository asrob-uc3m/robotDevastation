// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "LaunchRd1.hpp"

/************************************************************************/
LaunchRd1::LaunchRd1() { }

/************************************************************************/
bool LaunchRd1::configure(ResourceFinder &rf) {

    printf(BOLDBLUE);
    printf("LaunchRd1 options:\n");

    std::string type = rf.check("type",Value(DEFAULT_TYPE),"Name of robot type").asString();
    printf("\t--type %s (Name of robot type)\n", type.c_str() );

    std::string prefix = rf.check("prefix",Value(DEFAULT_PREFIX),"Name of robot without slash").asString();
    printf("\t--prefix %s (Prefix for robot port name, without slash)\n", prefix.c_str() );
    std::string prefixWithSlash("/");
    prefixWithSlash += prefix;

    camera = rf.check("camera",Value(DEFAULT_CAMERA),"Camera on or off").asString();
    if( camera == "on" )
        printf("\t--camera on (Camera on or off)\n" );
    else
        printf("\t--camera off (Camera on or off)\n" );

    printf(RESET);

    if( camera == "on" )
    {
        std::string cameraPortName(prefixWithSlash);
        cameraPortName += "/img:o";
        Property cameraOptions;
        cameraOptions.put("device","grabber");
        cameraOptions.put("subdevice","opencv_grabber");
        cameraOptions.put("name",cameraPortName);
        cameraDevice.open(cameraOptions);

        if (!cameraDevice.isValid())
        {
            RD_ERROR("Camera class instantiation not worked.\n");
            // cameraDevice.close();  // un-needed?
            return false;
        }
    }    

    Property robotOptions;
    robotOptions.fromString(rf.toString());
    robotOptions.put("device","controlboard");
    robotOptions.put("subdevice",type);
    robotOptions.put("name",prefixWithSlash);

    robotDevice.open(robotOptions);
    
    if (!robotDevice.isValid())
    {
        RD_ERROR("Robot class instantiation not worked.\n");
        RD_ERROR("Be sure CMake \"ENABLE_PiYarp_%s\" variable is set \"ON\"\n",type.c_str());
        RD_ERROR("\"SKIP_%s is set\" --> should be --> \"ENABLE_%s is set\"\n",type.c_str(),type.c_str());
        RD_ERROR("Also check raspi-motor communications.\n");
        // robotDevice.close();  // un-needed?
        return false;
    }

    return true;
}

/************************************************************************/

bool LaunchRd1::updateModule() {
    RD_INFO("Alive\n");
    return true;
}

/************************************************************************/

bool LaunchRd1::close() {
    if( camera == "on" )
    {
        cameraDevice.close();
    }    

    robotDevice.close();

    return true;
}

/************************************************************************/

