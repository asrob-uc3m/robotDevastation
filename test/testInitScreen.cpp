// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testInitScreen
 *
 * Testing InitScreen class, which shows the splash screen and a message to press any key.
 * This test requires a SDLScreenManager
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include "ScreenManager.hpp"
#include "SDLScreenManager.hpp"
#include "Screen.hpp"
#include "InitScreen.hpp"

#include <yarp/os/Time.h>

using namespace rd;

int main()
{
    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");
    screenManager->start();

    Screen * screen = new InitScreen();

    if(!screen->init())
        return 1;
    screenManager->setCurrentScreen(screen);

    for (int i = 0; i < 200; i++)
    {
        if(!screenManager->show())
            return 1;
        yarp::os::Time::delay(0.002);
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
