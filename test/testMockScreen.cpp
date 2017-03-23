/***
 * testMockupScreen
 *
 * Testing MockupScreen class, which offers a generic Screen for testing purposes
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include <yarp/os/Time.h>

#include "MockScreen.hpp"

using namespace rd;

int main()
{
    //-- Initialization
    initSDL();

    MockupScreen screen;
    screen.init();

    //-- Setup message
    screen.update(MockupScreen::PARAM_MESSAGE, "This is a test!");

    //-- Show it for a few seconds
    screen.show();
    yarp::os::Time::delay(2);

    //-- Clean stuff
    screen.cleanup();
    cleanupSDL();

    return 0;
}
