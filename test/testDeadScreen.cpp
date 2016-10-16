#include <SDL.h>
#include "SDLUtils.hpp"
#include <yarp/sig/all.h>
#include <yarp/os/ResourceFinder.h>

#include "DeadScreen.hpp"
#include "RdUtils.hpp"

using namespace rd;

int main()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");
    std::string file_path = rf.findFileByName("../images/test_frame.ppm");

    //-- Initialization
    initSDL();

    DeadScreen screen;
    screen.init();

    //-- Load test image
    RdImage test_frame;
    yarp::sig::file::read(test_frame, file_path.c_str());
    screen.update(DeadScreen::PARAM_LAST_CAMERA_FRAME, test_frame);

    //-- Setup elapsed time
    screen.update(DeadScreen::PARAM_REMAINING_TIME, "10");

    for (int i = 0; i < 10; i++)
    {
        screen.update(DeadScreen::PARAM_REMAINING_TIME, number2str(10-i));
        if( ! screen.show() )
            return 1;
        SDL_Delay(1000); //Wait a bit :)
    }

    screen.cleanup();

    cleanupSDL();

    return 0;
}
