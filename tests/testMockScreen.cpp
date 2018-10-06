// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testMockScreen
 *
 * Testing MockScreen class, which offers a generic Screen for testing purposes
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include <yarp/os/Time.h>

#include "MockScreen.hpp"
#include "SDLUtils.hpp"

using namespace rd;

int main(int argc, char *argv[])
{
    //-- Initialization
    initSDL();

    MockScreen screen;
    screen.init();

    //-- Setup message
    screen.update(MockScreen::PARAM_MESSAGE, "This is a test!");

    //-- Show it for a few seconds
    screen.show();
    yarp::os::Time::delay(2);

    //-- Clean stuff
    screen.cleanup();
    cleanupSDL();

    return 0;
}
