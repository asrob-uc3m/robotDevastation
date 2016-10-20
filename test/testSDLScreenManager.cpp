/***
 * testSDLScreenManager
 *
 * Testing SDLScreenManager class, which deals with SDL output and RdScreen management
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

//#include "ScreenManager.hpp"
//#include "SDLScreenManager.hpp"
#include "RdScreen.hpp"
//#include "MockupRdScreen.hpp"

using namespace rd;

int main()
{
    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::initSDL();
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");


    //-- Create a RdScreen to check the ScreenManager
    RdScreen * screen = new MockupRdScreen();
    screen->update(MockupRdScreen::MESSAGE, "Test screen");

    RdScreen * screen2 = new MockupRdScreen();
    screen2->update(MockupRdScreen::MESSAGE, "Another test screen");

    //-- Set the first Screen in the ScreenManager
    screenManager->setCurrentScreen(screen);

    //-- Lopp to display the first Screen

    //-- (loop goes here)
    screenManager->show();

    //-- Set the second Screen in the ScreenManager
    screenManager->setCurrentScreen(screen2);

    //-- Lopp to display the first second

    //-- (loop goes here)
    screenManager->show();

    SLDScreenManager::cleanSDL();

    return 0;
}
