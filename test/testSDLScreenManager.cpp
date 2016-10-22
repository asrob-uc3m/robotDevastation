/***
 * testSDLScreenManager
 *
 * Testing SDLScreenManager class, which deals with SDL output and RdScreen management
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include "ScreenManager.hpp"
//#include "SDLScreenManager.hpp"
#include "RdScreen.hpp"
#include "MockupScreen.hpp"

#include <yarp/os/Time.h>

using namespace rd;

int main()
{
    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::initSDL();
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");


    //-- Create a RdScreen to check the ScreenManager
    RdScreen * screen = new MockupScreen();
    screen->init();
    screen->update(MockupScreen::PARAM_MESSAGE, "Test screen");

    RdScreen * screen2 = new MockupScreen();
    screen2->init();
    screen2->update(MockupScreen::PARAM_MESSAGE, "Another test screen");

    //-- Set the first Screen in the ScreenManager
    screenManager->setCurrentScreen(screen);

    //-- Loop to display the first Screen
    for (int i = 0; i < 10; i++)
    {
        screenManager->show();
        yarp::os::Time::delay(0.2);
    }

    //-- Set the second Screen in the ScreenManager
    screenManager->setCurrentScreen(screen2);

    //-- Loop to display the first second
    for (int i = 0; i < 10; i++)
    {
        screenManager->show();
        yarp::os::Time::delay(0.2);
    }

    screen->cleanup();
    screen2->cleanup();
    SLDScreenManager::cleanSDL();

    return 0;
}
