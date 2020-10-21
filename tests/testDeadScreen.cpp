// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testDeadScreen
 *
 * Testing DeadScreen class, which shows the dead screen with a skull and a countdown.
 * This test requires a SDLScreenManager.
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include <string>
#include <sstream>

#include <yarp/sig/ImageFile.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Time.h>

#include "ScreenManager.hpp"
#include "SDLScreenManager.hpp"
#include "Screen.hpp"
#include "DeadScreen.hpp"
#include "ImageManager.hpp"

using namespace rd;

int main()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");
    std::string file_path = rf.findFileByName("../images/test_frame.ppm");

    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");
    screenManager->start();

    Screen * screen = new DeadScreen();
    if(!screen->init())
        return 1;
    screenManager->setCurrentScreen(screen);

    //-- Load test image
    Image test_frame;
    yarp::sig::file::read(test_frame, file_path.c_str());
    screenManager->update(DeadScreen::PARAM_LAST_CAMERA_FRAME, test_frame);

    //-- Setup elapsed time
    screenManager->update(DeadScreen::PARAM_REMAINING_TIME, "10");

    for (int i = 0; i < 10; i++)
    {
        std::stringstream sstream;
        sstream << (10-i);
        screenManager->update(DeadScreen::PARAM_REMAINING_TIME, sstream.str());
        if(!screenManager->show())
            return 1;
        yarp::os::Time::delay(1);
    }

    if(!screenManager->stop())
        return 1;

    if(!screen->cleanup() )
        return 1;

    delete screen;
    screen = NULL;

    SDLScreenManager::destroyScreenManager();

    return 0;
}
