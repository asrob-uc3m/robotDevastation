// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testSDLScreenManager
 *
 * Testing SDLScreenManager class, which deals with SDL output and Screen management
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include "ScreenManager.hpp"
#include "SDLScreenManager.hpp"
#include "Screen.hpp"
#include "MockScreen.hpp"

#include <yarp/os/Time.h>

using namespace rd;

int main()
{
    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");
    screenManager->start();

    //-- Create a Screen to check the ScreenManager
    Screen * screen = new MockScreen();
    screen->init();
    Screen * screen2 = new MockScreen();
    screen2->init();

    //-- Set the first Screen in the ScreenManager
    screenManager->setCurrentScreen(screen);
    screenManager->update(MockScreen::PARAM_MESSAGE, "Test screen");

    //-- Loop to display the first Screen
    for (int i = 0; i < 10; i++)
    {
        screenManager->show();
        yarp::os::Time::delay(0.2);
    }

    //-- Set the second Screen in the ScreenManager
    screenManager->setCurrentScreen(screen2);
    screenManager->update(MockScreen::PARAM_MESSAGE, "Another test screen");

    //-- Loop to display the first second
    for (int i = 0; i < 10; i++)
    {
        screenManager->show();
        yarp::os::Time::delay(0.2);
    }

    screenManager->stop();

    screen->cleanup();
    screen2->cleanup();

    delete screen;
    screen = NULL;
    delete screen2;
    screen2 = NULL;

    SDLScreenManager::destroyScreenManager();

    return 0;
}
