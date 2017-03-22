/***
 * testGameScreen
 *
 * Testing GameScreen class, which shows the game screen with HUD and webcam image.
 * This test requires a SDLScreenManager.
 *
 * This test is NOT AUTOMATIC, not suitable for running it with a CI server
 *
 ***/

#include <yarp/sig/all.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Time.h>
#include <string>
#include <vector>

#include "MentalMap.hpp"
#include "Target.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "ScreenManager.hpp"
#include "SDLScreenManager.hpp"
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace rd;

int main(void)
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Start SDL, Create SDLScreen Manager
    SDLScreenManager::RegisterManager();
    ScreenManager * screenManager = ScreenManager::getScreenManager("SDL");
    screenManager->start();

    RdScreen * screen = new GameScreen();
    if(!screen->init())
        return 1;
    screenManager->setCurrentScreen(screen);

    //-- Create a mental map with some info:
    RdMentalMap * mentalMap = RdMentalMap::getMentalMap();
    mentalMap->configure(0);

    std::vector<RdPlayer> players;
    players.push_back(RdPlayer(0, "Myself", 50, 100, 0, 0));
    players.push_back(RdPlayer(1, "Enemy1", 50, 100, 1, 0));
    players.push_back(RdPlayer(2, "Enemy2", 75, 100, 1, 0));
    mentalMap->updatePlayers(players);

    std::vector<RdTarget> targets;
    targets.push_back(RdTarget(1, RdVector2d(100, 100), RdVector2d(50, 50)));
    targets.push_back(RdTarget(2, RdVector2d(400, 200), RdVector2d(100, 100)));
    targets.push_back(RdTarget());
    mentalMap->updateTargets(targets);

    mentalMap->addWeapon(RdWeapon("Machine gun", 10, 250));

    //-- Set info elements on GameScreen
    screenManager->update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screenManager->update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screenManager->update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screenManager->update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());

    //-- Load test image
    RdImage frame;
    yarp::sig::file::read(frame, rf.findFileByName("../images/test_frame.ppm"));
    screenManager->update(GameScreen::PARAM_CAMERA_FRAME, frame);

    for (int i = 0; i < 200; i++)
    {
        screenManager->show();
        if(!screenManager->show())
            return 1;
        yarp::os::Time::delay(0.02);
    }

    if(!screenManager->stop())
        return 1;

    if(!screen->cleanup() )
        return 1;

    delete screen;
    screen = NULL;

    SDLScreenManager::destroyScreenManager();
    RdMentalMap::destroyMentalMap();
    return 0;
}



